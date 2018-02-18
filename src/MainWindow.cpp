#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ApplicationUtilities.h"
#include "QSerialTerminalLineEdit.h"
#include "ApplicationStrings.h"
#include "ApplicationIcons.h"
#include "GlobalDefinitions.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QtCore/QTimer>
#include <QtWidgets/QStatusBar>
#include <QLabel>

#include "IByteStream.h"
#include "SerialPort.h"
#include "AboutApplicationWidget.h"

const int MainWindow::CHECK_PORT_DISCONNECT_TIMEOUT{750};
const int MainWindow::CHECK_PORT_RECEIVE_TIMEOUT{1};
const int MainWindow::NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT{5000};
const int MainWindow::SERIAL_READ_TIMEOUT{5000};
const CppSerialPort::BaudRate MainWindow::DEFAULT_BAUD_RATE{CppSerialPort::BaudRate::Baud9600};
const CppSerialPort::Parity MainWindow::DEFAULT_PARITY{CppSerialPort::Parity::ParityNone};
const CppSerialPort::StopBits MainWindow::DEFAULT_STOP_BITS{CppSerialPort::StopBits::StopOne};
const CppSerialPort::DataBits MainWindow::DEFAULT_DATA_BITS{CppSerialPort::DataBits::DataEight};
const CppSerialPort::FlowControl MainWindow::DEFAULT_FLOW_CONTROL{CppSerialPort::FlowControl::FlowOff};
const int MainWindow::STATUS_BAR_FONT_POINT_SIZE{12};
const char *MainWindow::CARRIAGE_RETURN_LINE_ENDING{R"(\r)"};
const char *MainWindow::NEW_LINE_LINE_ENDING{R"(\n)"};
const char *MainWindow::CARRIAGE_RETURN_NEW_LINE_LINE_ENDING{R"(\r\n)"};
const std::list<std::string> MainWindow::AVAILABLE_LINE_ENDINGS{CARRIAGE_RETURN_LINE_ENDING,
                                                                  NEW_LINE_LINE_ENDING,
                                                                  CARRIAGE_RETURN_NEW_LINE_LINE_ENDING};
const std::string MainWindow::DEFAULT_LINE_ENDING{MainWindow::NEW_LINE_LINE_ENDING};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    m_ui{new Ui::MainWindow{}},
    m_aboutApplicationWidget{std::make_shared<AboutApplicationWidget>()},
    m_statusBarLabel{new QLabel{""}},
    m_checkPortDisconnectTimer{new QTimer{}},
    m_checkSerialPortReceiveTimer{new QTimer{}},
    m_serialPortNames{CppSerialPort::SerialPort::availableSerialPorts()},
    m_currentLinePushedIntoCommandHistory{false},
    m_currentHistoryIndex{0}
{
    using namespace ApplicationStrings;
    this->m_ui->setupUi(this);

    QFont tempFont{this->m_statusBarLabel->font()};
    tempFont.setPointSize(MainWindow::STATUS_BAR_FONT_POINT_SIZE);
    this->m_statusBarLabel->setFont(tempFont);
    this->m_ui->statusBar->addWidget(this->m_statusBarLabel.get());
    qApp->installEventFilter(this);

    setupAdditionalUiComponents();

    connect(this->m_ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(this->m_ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::onApplicationAboutToClose);
    connect(this->m_ui->actionAboutQSerialTerminal, &QAction::triggered, this, &MainWindow::onAboutQSerialTerminalActionTriggered);

    connect(this->m_ui->sendBox, &QSerialTerminalLineEdit::commandHistoryContextMenuRequested, this, &MainWindow::onCommandHistoryContextMenuRequested);
    connect(this->m_ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);

    connect(this->m_ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(this->m_ui->sendBox, &QSerialTerminalLineEdit::returnPressed, this, &MainWindow::onReturnKeyPressed);

    /* initialize all strings and stuff for the AboutApplicationWidget */
    this->m_aboutApplicationWidget->addLicenseTab(QCoreApplication::applicationName(), ApplicationStrings::LICENSE_PATH);
    this->m_aboutApplicationWidget->addLicenseTab(ApplicationStrings::MINGW_NAME, ApplicationStrings::MINGW_LICENSE_PATH);
    this->m_aboutApplicationWidget->addLicenseTab(ApplicationStrings::QT_NAME, ApplicationStrings::QT_LICENSE_PATH);

    //this->m_aboutApplicationWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_aboutApplicationWidget->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    this->m_aboutApplicationWidget->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
    this->connect(this->m_ui->actionAboutQSerialTerminal, &QAction::triggered, this, &MainWindow::onAboutQSerialTerminalActionTriggered);
    this->connect(this->m_aboutApplicationWidget.get(), &AboutApplicationWidget::aboutToClose, this, &MainWindow::onAboutApplicationWidgetWindowClosed);

    this->m_checkPortDisconnectTimer->setInterval(MainWindow::CHECK_PORT_DISCONNECT_TIMEOUT);
    this->m_checkSerialPortReceiveTimer->setInterval(MainWindow::CHECK_PORT_RECEIVE_TIMEOUT);

    connect(this->m_checkPortDisconnectTimer.get(), &QTimer::timeout, this, &MainWindow::checkDisconnectedSerialPorts);
    connect(this->m_checkSerialPortReceiveTimer.get(), &QTimer::timeout, this, &MainWindow::launchSerialReceiveAsync);

    this->show();
    this->m_checkPortDisconnectTimer->start();
    this->m_checkSerialPortReceiveTimer->start();
}

void MainWindow::onAboutApplicationWidgetWindowClosed()
{
    this->setEnabled(true);
    this->show();
    this->m_aboutApplicationWidget->hide();
}

void MainWindow::onAboutQSerialTerminalActionTriggered(bool checked)
{
    Q_UNUSED(checked);
    this->setEnabled(false);
    this->m_aboutApplicationWidget->show();
    this->m_aboutApplicationWidget->move(this->rect().center() - this->m_aboutApplicationWidget->rect().center());
}

void MainWindow::setStatusBarLabelText(const QString &str)
{
    this->m_statusBarLabel->setText(str);
}

void MainWindow::setLineEnding(const std::string &str)
{
    std::string lineEnding{escapeLineEnding(str)};
    using namespace ApplicationStrings;
    if (lineEnding == escapeLineEnding(MainWindow::CARRIAGE_RETURN_NEW_LINE_LINE_ENDING)) {
        this->m_lineEnding = MainWindow::CARRIAGE_RETURN_NEW_LINE_LINE_ENDING;
    } else if (lineEnding == escapeLineEnding(MainWindow::NEW_LINE_LINE_ENDING)) {
        this->m_lineEnding = MainWindow::NEW_LINE_LINE_ENDING;
    } else if (lineEnding == escapeLineEnding(MainWindow::CARRIAGE_RETURN_LINE_ENDING)) {
        this->m_lineEnding = MainWindow::CARRIAGE_RETURN_LINE_ENDING;
    } else {
        throw std::runtime_error(QString{"MainWindow::setLineEnding(const std::string &): (invalid line ending \"%1\")"}.arg(lineEnding.c_str()).toStdString());
    }
}

void MainWindow::autoSetLineEnding()
{
    for (auto &it : this->m_availableLineEndingActions) {
        if (it->isChecked()) {
            this->setLineEnding(it->text().toStdString());
            if (this->m_byteStream) {
                this->m_byteStream->setLineEnding(this->m_lineEnding);
            }
        }
    }
}

void MainWindow::appendReceivedString(const std::string &str)
{
    using namespace ApplicationStrings;
    if (str.length() > 0) {
        this->printRxResult(str);
    }
}

void MainWindow::appendTransmittedString(const QString &str)
{
    using namespace ApplicationStrings;
    if (this->m_byteStream) {
        if (!this->m_byteStream->isOpen()) {
            this->openSerialPort();
        }
        if (str.toStdString() != "") {
            this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_ui->sendBox->text());
            resetCommandHistory();
        }
        this->m_byteStream->writeLine(str.toStdString());
        this->printTxResult(str.toStdString());
        this->m_ui->sendBox->clear();
    }
}


void MainWindow::checkDisconnectedSerialPorts()
{
    /*
    auto names = CppSerialPort::SerialPort::availableSerialPorts();
    auto maybeUpdate = std::vector<std::string>{};
    auto currents = QActionSet{};

    for (auto &it : names) {
        maybeUpdate.push_back(it);
    }
    for (auto &it : this->m_availablePortNamesActions) {
        currents.insert(it);
    }
    for (auto &it : maybeUpdate) {
        QAction temp{it.c_str()};
        if (currents.find(&temp) == currents.end()) {
            this->addNewSerialPortInfoItem(SerialPortItemType::PortName, it);
        }
    }
    for (auto &it : currents) {
        if (std::find(maybeUpdate.begin(), maybeUpdate.end(), it) == maybeUpdate.end()) {
            this->removeOldSerialPortInfoItem(SerialPortItemType::PortName, it);
        }
    }
     */
}

std::string MainWindow::escapeLineEnding(const std::string &lineEnding) {
    std::string copyString{""};
    for (const auto &it : lineEnding) {
        if (it != '\\') {
            copyString += it;
        }
    }
    std::string returnString{""};
    for (const auto &it : copyString) {
        if (it == '\r') {
            returnString += 'r';
        } else if (it == '\n') {
            returnString += 'n';
        } else {
            returnString += it;
        }

    }
    return returnString;
}

void MainWindow::addNewLineEndingItem(const std::string &lineEnding) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{lineEnding.c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::LINE_ENDING_ACTION_KEY, QVariant{lineEnding.c_str()});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionLineEndingsChecked);
    this->m_availableLineEndingActions.insert(tempAction);
    this->m_ui->menuLineEndings->addAction(tempAction);
    if (escapeLineEnding(lineEnding) == escapeLineEnding(MainWindow::DEFAULT_LINE_ENDING)) {
        this->setLineEnding(tempAction);
    }
}

void MainWindow::addNewPortNameItem(const std::string &str) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{str.c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::PORT_NAME_ACTION_KEY, QVariant{str.c_str()});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionPortNamesChecked);
    this->m_availablePortNamesActions.insert(tempAction);
    this->m_ui->menuPortNames->addAction(tempAction);
}

void MainWindow::addNewBaudRateItem(CppSerialPort::BaudRate baudRate) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{toStdString(baudRate).c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::BAUD_RATE_ACTION_KEY, QVariant{static_cast<int>(baudRate)});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionBaudRateChecked);
    this->m_availableBaudRateActions.insert(tempAction);
    this->m_ui->menuBaudRate->addAction(tempAction);
    if (tempAction->text().toStdString() == toStdString(DEFAULT_BAUD_RATE)) {
        this->setBaudRate(tempAction);
    }
}

void MainWindow::addNewStopBitsItem(CppSerialPort::StopBits stopBits) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{toStdString(stopBits).c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::STOP_BITS_ACTION_KEY, QVariant{static_cast<int>(stopBits)});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionStopBitsChecked);
    this->m_availableStopBitsActions.insert(tempAction);
    this->m_ui->menuStopBits->addAction(tempAction);
    if (tempAction->text().toStdString() == toStdString(DEFAULT_STOP_BITS)) {
        this->setStopBits(tempAction);
    }
}

void MainWindow::addNewDataBitsItem(CppSerialPort::DataBits dataBits) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{toStdString(dataBits).c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::DATA_BITS_ACTION_KEY, QVariant{static_cast<int>(dataBits)});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionDataBitsChecked);
    this->m_availableDataBitsActions.insert(tempAction);
    this->m_ui->menuDataBits->addAction(tempAction);
    if (tempAction->text().toStdString() == toStdString(DEFAULT_DATA_BITS)) {
        this->setDataBits(tempAction);
    }
}

void MainWindow::addNewParityItem(CppSerialPort::Parity parity) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{toStdString(parity).c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::PARITY_ACTION_KEY, QVariant{static_cast<int>(parity)});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionParityChecked);
    this->m_availableParityActions.insert(tempAction);
    this->m_ui->menuParity->addAction(tempAction);
    if (tempAction->text().toStdString() == toStdString(DEFAULT_PARITY)) {
        this->setParity(tempAction);
    }
}

void MainWindow::addNewFlowControlItem(CppSerialPort::FlowControl flowControl) {
    using namespace ApplicationUtilities;
    QAction *tempAction{new QAction{toStdString(flowControl).c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setProperty(ApplicationStrings::FLOW_CONTROL_ACTION_KEY, QVariant{static_cast<int>(flowControl)});
    tempAction->setCheckable(true);
    connect(tempAction, &QAction::triggered, this, &MainWindow::onActionFlowControlChecked);
    this->m_availableFlowControlActions.insert(tempAction);
    this->m_ui->menuFlowControl->addAction(tempAction);
    if (tempAction->text().toStdString() == toStdString(DEFAULT_FLOW_CONTROL)) {
        this->setFlowControl(tempAction);
    }
}

template <typename T>
class TD;

void MainWindow::removeOldPortNameItem(const std::string &str) {
    using namespace ApplicationUtilities;
    using namespace ApplicationStrings;
    auto actionName = QString::fromStdString(str);
    auto foundPosition = findInQActionSet(&this->m_availablePortNamesActions, actionName);
    if (foundPosition != this->m_availablePortNamesActions.end()) {
        auto foundAction = *foundPosition;
        this->setStatusBarLabelText(QString{SERIAL_PORT_DISCONNECTED_STRING + foundAction->text()});
        if (foundAction->isChecked()) {
            this->m_ui->actionConnect->setChecked(false);
            this->m_ui->connectButton->setChecked(false);
            this->m_ui->actionConnect->setEnabled(false);
            this->m_ui->connectButton->setEnabled(false);
            this->m_ui->actionLoadScript->setEnabled(false);
            this->m_ui->sendBox->setEnabled(false);
            this->m_ui->sendButton->setEnabled(false);
        }
        foundAction->deleteLater();
        this->m_availablePortNamesActions.erase(foundPosition);
        return;
    }
}

std::unordered_set<QAction *>::iterator MainWindow::findInQActionSet(QActionSet *qActionSet, const QString &key) {
    QAction tempAction{nullptr};
    tempAction.setText(key);
    return qActionSet->find(&tempAction);
}

void MainWindow::removeOldBaudRateItem(CppSerialPort::BaudRate baudRate) {
    using namespace ApplicationUtilities;
    auto actionName = QString::fromStdString(toStdString(baudRate));
    auto foundPosition = findInQActionSet(&this->m_availableBaudRateActions, actionName);
    if (foundPosition != this->m_availableBaudRateActions.end()) {
        auto foundAction = *foundPosition;
        if (foundAction->text() == actionName) {
            foundAction->deleteLater();
            this->m_availableBaudRateActions.erase(foundPosition);
            return;
        }
    }
}

void MainWindow::removeOldStopBitsItem(CppSerialPort::StopBits stopBits) {
    using namespace ApplicationUtilities;
    auto actionName = QString::fromStdString(toStdString(stopBits));
    auto foundPosition = findInQActionSet(&this->m_availableStopBitsActions, actionName);
    if (foundPosition != this->m_availableStopBitsActions.end()) {
        auto foundAction = *foundPosition;
        if (foundAction->text() == actionName) {
            foundAction->deleteLater();
            this->m_availableStopBitsActions.erase(foundPosition);
            return;
        }
    }
}

void MainWindow::removeOldDataBitsItem(CppSerialPort::DataBits dataBits) {
    using namespace ApplicationUtilities;
    auto actionName = QString::fromStdString(toStdString(dataBits));
    auto foundPosition = findInQActionSet(&this->m_availableDataBitsActions, actionName);
    if (foundPosition != this->m_availableDataBitsActions.end()) {
        auto foundAction = *foundPosition;
        if (foundAction->text() == actionName) {
            foundAction->deleteLater();
            this->m_availableDataBitsActions.erase(foundPosition);
            return;
        }
    }
}

void MainWindow::removeOldParityItem(CppSerialPort::Parity parity) {
    using namespace ApplicationUtilities;
    auto actionName = QString::fromStdString(toStdString(parity));
    auto foundPosition = findInQActionSet(&this->m_availableParityActions, actionName);
    if (foundPosition != this->m_availableParityActions.end()) {
        auto foundAction = *foundPosition;
        if (foundAction->text() == actionName) {
            foundAction->deleteLater();
            this->m_availableParityActions.erase(foundPosition);
            return;
        }
    }
}

void MainWindow::removeOldFlowControlItem(CppSerialPort::FlowControl flowControl) {
    using namespace ApplicationUtilities;
    auto actionName = QString::fromStdString(toStdString(flowControl));
    auto foundPosition = findInQActionSet(&this->m_availableFlowControlActions, actionName);
    if (foundPosition != this->m_availableFlowControlActions.end()) {
        auto foundAction = *foundPosition;
        if (foundAction->text() == actionName) {
            foundAction->deleteLater();
            this->m_availableFlowControlActions.erase(foundPosition);
            return;
        }
    }
}

void MainWindow::launchSerialReceiveAsync()
{
    if ( (!this->m_byteStream) || (!this->m_byteStream->isOpen()) ) {
        return;
    }
    try {
		bool resetNeeded{ false };
        if (!this->m_serialReceiveAsyncHandle) {
			resetNeeded = true;
		} else if (this->m_serialReceiveAsyncHandle->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
			appendReceivedString(this->m_serialReceiveAsyncHandle->get());
			resetNeeded = true;
		}
		if (resetNeeded) {
            this->m_serialReceiveAsyncHandle.reset(new std::future<std::string>{std::async(std::launch::async,
                                                    &MainWindow::checkSerialReceive,
                                                    this)});
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

std::string MainWindow::checkSerialReceive()
{
    if (this->m_byteStream) {
        bool timeout{false};
        std::string returnString{this->m_byteStream->readLine(&timeout)};
        if (!returnString.empty()) {
            return returnString;
        }
    }
    return "";
}

void MainWindow::resetCommandHistory()
{
    this->m_currentHistoryIndex = 0;
    this->m_currentLinePushedIntoCommandHistory = false;
    clearEmptyStringsFromCommandHistory();
}

void MainWindow::clearEmptyStringsFromCommandHistory()
{
    while (true) {
        int i{0};
        for (auto &it : this->m_commandHistory) {
            if (it.isEmpty()) {
                this->m_commandHistory.erase(this->m_commandHistory.begin() + i);
                break;
            }
            i++;
        }
        break;
    }
}

void MainWindow::onSendButtonClicked()
{
    using namespace ApplicationStrings;
    if (this->m_byteStream) {
        appendTransmittedString(this->m_ui->sendBox->text());
    } else {
        if (this->m_availablePortNamesActions.size() != 0) {
            onActionConnectTriggered(false);
            onSendButtonClicked();
        } else {
            this->setStatusBarLabelText(NO_SERIAL_PORTS_CONNECTED_STRING);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    event->accept();
    /*
    using namespace ApplicationStrings;
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this, QUIT_PROMPT_WINDOW_TITLE_STRING, QUIT_PROMPT_STRING, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        event->accept();
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
    */
}

void MainWindow::onCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace ApplicationStrings;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    int i{0};
    for (auto &it : this->m_commandHistory) {
        if (it.isEmpty()) {
            i++;
            continue;
        }
        QAction *tempAction{new QAction{it, this}};
        tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{i++});
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, &QAction::triggered, this, &MainWindow::onCommandHistoryContextMenuActionTriggered);
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void MainWindow::onCommandHistoryContextMenuActionTriggered(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->property(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG).toUInt();
        this->m_ui->sendBox->setText(action->text());
    }
}

void MainWindow::setupAdditionalUiComponents()
{
    using namespace ApplicationStrings;
    using namespace ApplicationUtilities;


    for (auto &it : MainWindow::AVAILABLE_LINE_ENDINGS) {
        this->addNewLineEndingItem(it);
    }

    this->addNewDataBitsItem(CppSerialPort::DataBits::DataFive);
    this->addNewDataBitsItem(CppSerialPort::DataBits::DataSix);
    this->addNewDataBitsItem(CppSerialPort::DataBits::DataSeven);
    this->addNewDataBitsItem(CppSerialPort::DataBits::DataEight);

    this->addNewStopBitsItem(CppSerialPort::StopBits::StopOne);
#if defined(_WIN32)
    this->addNewStopBitsItem(CppSerialPort::StopBits::StopOneFive);
#endif //defined(_WIN32)
    this->addNewStopBitsItem(CppSerialPort::StopBits::StopTwo);

    this->addNewParityItem(CppSerialPort::Parity::ParityNone);
    this->addNewParityItem(CppSerialPort::Parity::ParityEven);
    this->addNewParityItem(CppSerialPort::Parity::ParityOdd);
    this->addNewParityItem(CppSerialPort::Parity::ParitySpace);
#if defined(_WIN32)
    this->addNewParityItem(CppSerialPort::Parity::ParityMark);
#endif //defined(_WIN32)


#if defined(_WIN32)
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud110);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud300);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud2400);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud4800);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud9600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud19200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud38400);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud57600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud115200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud128000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud256000);
#else
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud50);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud75);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud110);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud134);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud150);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud300);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1800);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud2400);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud4800);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud9600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud19200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud38400);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud57600);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud115200);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud230400);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud460800);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud500000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud576000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1000000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1152000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud1500000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud2000000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud2500000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud3000000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud3500000);
    this->addNewBaudRateItem(CppSerialPort::BaudRate::Baud4000000);
#endif //defined(_WIN32)

    this->addNewFlowControlItem(CppSerialPort::FlowControl::FlowOff);
    this->addNewFlowControlItem(CppSerialPort::FlowControl::FlowHardware);
    this->addNewFlowControlItem(CppSerialPort::FlowControl::FlowXonXoff);

    for (auto &it : CppSerialPort::SerialPort::availableSerialPorts()) {
        this->addNewPortNameItem(it);
    }

    if (!this->m_availablePortNamesActions.empty()) {
#if defined(_WIN32)
        auto foundPosition = findInQActionSet(&this->m_availablePortNamesActions, "COM1");
#else
        auto foundUSB = findInQActionSet(&this->m_availablePortNamesActions, "/dev/ttyUSB0");
        auto foundACM = findInQActionSet(&this->m_availablePortNamesActions, "/dev/ttyACM0");
        auto foundPosition = foundACM != this->m_availablePortNamesActions.end() ? foundACM : foundUSB;

#endif //defined(_WIN32)
        if (foundPosition == this->m_availablePortNamesActions.end()) {
            this->setPortName(*std::begin(this->m_availablePortNamesActions));
        } else {
            this->setPortName(*foundPosition);
        }
        this->m_ui->connectButton->setEnabled(true);
        this->m_ui->actionConnect->setEnabled(true);
    } else {
        this->m_ui->connectButton->setEnabled(false);
        this->m_ui->actionConnect->setEnabled(false);
    }
    this->m_ui->sendBox->setEnabled(false);
    this->m_ui->sendButton->setEnabled(false);
    this->m_ui->sendBox->setToolTip(SEND_BOX_DISABLED_TOOLTIP);
    this->m_ui->sendBox->setTabOrder(this->m_ui->sendBox, this->m_ui->sendButton);
    this->setStatusBarLabelText(CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING);
}

void MainWindow::setBaudRate(QAction *action) {
    for (auto &it : this->m_availableBaudRateActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    if (this->m_byteStream) {
        this->m_byteStream->setBaudRate(static_cast<CppSerialPort::BaudRate>(action->property(ApplicationStrings::BAUD_RATE_ACTION_KEY).toInt(nullptr)));
    }
}

void MainWindow::setFlowControl(QAction *action) {
    for (auto &it : this->m_availableFlowControlActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    if (this->m_byteStream) {
        this->m_byteStream->setFlowControl(static_cast<CppSerialPort::FlowControl>(action->property(ApplicationStrings::FLOW_CONTROL_ACTION_KEY).toInt(nullptr)));
    }
}

void MainWindow::setParity(QAction *action) {
    for (auto &it : this->m_availableParityActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    if (this->m_byteStream) {
        this->m_byteStream->setParity(static_cast<CppSerialPort::Parity>(action->property(ApplicationStrings::PARITY_ACTION_KEY).toInt(nullptr)));
    }
}

void MainWindow::setDataBits(QAction *action) {
    for (auto &it : this->m_availableDataBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    if (this->m_byteStream) {
        this->m_byteStream->setDataBits(static_cast<CppSerialPort::DataBits>(action->property(ApplicationStrings::DATA_BITS_ACTION_KEY).toInt(nullptr)));
    }
}

void MainWindow::setStopBits(QAction *action) {
    for (auto &it : this->m_availableStopBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    if (this->m_byteStream) {
        this->m_byteStream->setStopBits(static_cast<CppSerialPort::StopBits >(action->property(ApplicationStrings::STOP_BITS_ACTION_KEY).toInt(nullptr)));
    }
}

void MainWindow::setLineEnding(QAction *action) {
    for (auto &it : this->m_availableLineEndingActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
    this->setLineEnding(action->text().toStdString());
    if (this->m_byteStream) {
        this->m_byteStream->setLineEnding(this->m_lineEnding);
    }
}

void MainWindow::setPortName(QAction *action) {
    for (auto &it : this->m_availablePortNamesActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionBaudRateChecked(bool checked)  {
    Q_UNUSED(checked);
    this->setBaudRate(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionFlowControlChecked(bool checked) {
    Q_UNUSED(checked);
    this->setFlowControl(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionParityChecked(bool checked)  {
    Q_UNUSED(checked);
    this->setParity(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionDataBitsChecked(bool checked) {
    Q_UNUSED(checked);
    this->setDataBits(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionStopBitsChecked(bool checked) {
    Q_UNUSED(checked);
    this->setStopBits(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionLineEndingsChecked(bool checked) {
    Q_UNUSED(checked);
    this->setLineEnding(dynamic_cast<QAction *>(QObject::sender()));
}

void MainWindow::onActionPortNamesChecked(bool checked) {
    Q_UNUSED(checked);
    this->setPortName(dynamic_cast<QAction *>(QObject::sender()));
}

CppSerialPort::BaudRate MainWindow::getSelectedBaudRate() {
    for (auto &it : this->m_availableBaudRateActions) {
        if (it->isChecked()) {
            return static_cast<CppSerialPort::BaudRate>(it->property(ApplicationStrings::BAUD_RATE_ACTION_KEY).toInt(nullptr));
        }
    }
    throw std::runtime_error("MainWindow::getSelectedBaudRate(): No baud rates are currently selected");
}

CppSerialPort::StopBits MainWindow::getSelectedStopBits () {
    for (auto &it : this->m_availableStopBitsActions) {
        if (it->isChecked()) {
            return static_cast<CppSerialPort::StopBits>(it->property(ApplicationStrings::STOP_BITS_ACTION_KEY).toInt(nullptr));
        }
    }
    throw std::runtime_error("MainWindow::getSelectedStopBits(): No stop bits are currently selected");
}

CppSerialPort::FlowControl MainWindow::getSelectedFlowControl() {
    for (auto &it : this->m_availableFlowControlActions) {
        if (it->isChecked()) {
            return static_cast<CppSerialPort::FlowControl>(it->property(ApplicationStrings::FLOW_CONTROL_ACTION_KEY).toInt(nullptr));
        }
    }
    throw std::runtime_error("MainWindow::getSelectedFlowControl(): No flow control is currently selected");
}

CppSerialPort::DataBits MainWindow::getSelectedDataBits() {
    for (auto &it : this->m_availableDataBitsActions) {
        if (it->isChecked()) {
            return static_cast<CppSerialPort::DataBits>(it->property(ApplicationStrings::DATA_BITS_ACTION_KEY).toInt(nullptr));
        }
    }
    throw std::runtime_error("MainWindow::getSelectedDataBits(): No data bits are currently selected");

}

CppSerialPort::Parity MainWindow::getSelectedParity() {
    for (auto &it : this->m_availableParityActions) {
        if (it->isChecked()) {
            return static_cast<CppSerialPort::Parity>(it->property(ApplicationStrings::PARITY_ACTION_KEY).toInt(nullptr));
        }
    }
    throw std::runtime_error("MainWindow::getSelectedParity(): No parity is currently selected");

}

std::string MainWindow::getSelectedPortName() {
    for (auto &it : this->m_availablePortNamesActions) {
        if (it->isChecked()) {
            return it->property(ApplicationStrings::PORT_NAME_ACTION_KEY).toString().toStdString();
        }
    }
    throw std::runtime_error("MainWindow::getSelectedPortName(): No port name is currently selected");

}

void MainWindow::onActionConnectTriggered(bool checked)
{
    if (!checked) {
        this->onActionDisconnectTriggered(checked);
        return;
    }
    using namespace ApplicationStrings;
    this->pauseCommunication();

    std::string portName{this->getSelectedPortName()};
    CppSerialPort::BaudRate baudRate{this->getSelectedBaudRate()};
    CppSerialPort::Parity parity{this->getSelectedParity()};
    CppSerialPort::StopBits stopBits{this->getSelectedStopBits()};
    CppSerialPort::DataBits dataBits{this->getSelectedDataBits()};
    CppSerialPort::FlowControl flowControl{getSelectedFlowControl()};
    CppSerialPort::SerialPort *serialPort{this->m_byteStream.get()};
    if (serialPort) {
        if (baudRate == serialPort->baudRate() &&
                parity == serialPort->parity() &&
                stopBits == serialPort->stopBits() &&
                dataBits == serialPort->dataBits() &&
                flowControl == serialPort->flowControl() &&
                portName == serialPort->portName()) {
            if (!this->m_byteStream->isOpen()) {
                openSerialPort();
            }
            return;
        }
        try {
            closeSerialPort();
            this->m_byteStream.reset();
            this->m_byteStream = std::make_shared<CppSerialPort::SerialPort>(portName, baudRate, dataBits, stopBits, parity, flowControl);
            openSerialPort();
        } catch (std::exception &e) {
            (void)e;
            std::unique_ptr<QMessageBox> warningBox{new QMessageBox{}};
            warningBox->setText(QString{INVALID_SETTINGS_DETECTED_STRING} + e.what());
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
            warningBox->exec();
            if (this->m_byteStream) {
                closeSerialPort();
            }
        }
    } else {
        try {
            this->m_byteStream = std::make_shared<CppSerialPort::SerialPort>(portName, baudRate, dataBits, stopBits, parity);
            openSerialPort();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{new QMessageBox{}};
            warningBox->setText(QString{INVALID_SETTINGS_DETECTED_STRING} + e.what());
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
            warningBox->exec();
            if (this->m_byteStream) {
                closeSerialPort();
            }
        }
   }
}

void MainWindow::openSerialPort()
{
    using namespace ApplicationStrings;

    try {
        this->m_byteStream->openPort();
        this->m_ui->terminal->clear();
        this->m_ui->connectButton->setChecked(true);
        this->m_ui->sendButton->setEnabled(true);
        this->m_ui->actionDisconnect->setEnabled(true);
        this->m_ui->sendBox->setEnabled(true);
        this->m_ui->sendBox->setToolTip(SEND_BOX_ENABLED_TOOLTIP);
        this->m_ui->actionLoadScript->setEnabled(true);
        this->m_ui->sendBox->setFocus();
        this->setStatusBarLabelText(QString{SUCCESSFULLY_OPENED_SERIAL_PORT_STRING} + this->m_byteStream->portName().c_str());
        this->m_byteStream->setReadTimeout(MainWindow::SERIAL_READ_TIMEOUT);
        this->m_byteStream->setLineEnding(this->m_lineEnding);
        beginCommunication();
    } catch (std::exception &e) {
        std::unique_ptr<QMessageBox> warningBox{new QMessageBox{}};
        warningBox->setText(QString{COULD_NOT_OPEN_SERIAL_PORT_STRING} + e.what());
        warningBox->setWindowTitle(COULD_NOT_OPEN_SERIAL_PORT_WINDOW_TITLE_STRING);
        warningBox->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
        warningBox->exec();
        if (this->m_byteStream) {
            closeSerialPort();
        }
    }
}

void MainWindow::closeSerialPort()
{
    using namespace ApplicationStrings;
    this->m_byteStream->closePort();
    this->m_ui->connectButton->setChecked(false);
    this->m_ui->actionDisconnect->setEnabled(false);
    this->m_ui->sendBox->setEnabled(false);
    this->m_ui->sendBox->setToolTip(SEND_BOX_DISABLED_TOOLTIP);
    this->m_ui->actionLoadScript->setEnabled(false);
    this->setStatusBarLabelText(QString{SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING} + this->m_byteStream->portName().c_str());
    this->setWindowTitle(MAIN_WINDOW_TITLE);
}


void MainWindow::beginCommunication()
{
    using namespace ApplicationStrings;
    if (this->m_byteStream) {
        try {
            if (!this->m_byteStream->isOpen()) {
                openSerialPort();
                return;
            }
            this->setWindowTitle(this->windowTitle() + " - " + this->m_byteStream->portName().c_str());
            this->setStatusBarLabelText(QString{SUCCESSFULLY_OPENED_SERIAL_PORT_STRING} + this->m_byteStream->portName().c_str());
            this->m_checkSerialPortReceiveTimer->start();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{new QMessageBox{}};
            warningBox->setText(QString{INVALID_SETTINGS_DETECTED_STRING} + e.what());
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
            warningBox->exec();
        }
    } else {
        CppSerialPort::BaudRate baudRate{this->getSelectedBaudRate()};
        CppSerialPort::Parity parity{this->getSelectedParity()};
        CppSerialPort::StopBits stopBits{this->getSelectedStopBits()};
        CppSerialPort::DataBits dataBits{this->getSelectedDataBits()};
        CppSerialPort::FlowControl flowControl{getSelectedFlowControl()};
        std::string portName{this->getSelectedPortName()};
        try {
            this->m_byteStream = std::make_shared<CppSerialPort::SerialPort>(portName, baudRate, dataBits, stopBits, parity, flowControl);
            beginCommunication();
        } catch (std::exception &e) {
            (void)e;
        }
    }
}

void MainWindow::pauseCommunication()
{

}

void MainWindow::stopCommunication()
{
    using namespace ApplicationStrings;
    this->m_checkSerialPortReceiveTimer->stop();
    if (this->m_byteStream) {
        closeSerialPort();
    }
}

void MainWindow::onActionDisconnectTriggered(bool checked)
{
    (void)checked;
    using namespace ApplicationStrings;
    if (this->m_byteStream) {
        closeSerialPort();
        this->m_ui->connectButton->setChecked(false);
        LOG_DEBUG() << QString{"Waiting for async read of serial port %1 to finish..."}.arg(this->m_byteStream->portName().c_str());
        while (this->m_serialReceiveAsyncHandle->wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            std::this_thread::yield();
        }
        LOG_DEBUG() << "Completed, resetting serial port shared_ptr";
        this->m_byteStream.reset();
    }
}

void MainWindow::printRxResult(const std::string &str)
{
    using namespace ApplicationStrings;
    using namespace ApplicationUtilities;
    if (!str.empty()) {
        std::lock_guard<std::mutex> ioLock{this->m_printToTerminalMutex};
        this->m_ui->terminal->setTextColor(QColor(RED_COLOR_STRING));
        this->m_ui->terminal->append(QString{"%1%2"}.arg(TERMINAL_RECEIVE_BASE_STRING, stripLineEndings(str).c_str()));
    }
}

void MainWindow::printTxResult(const std::string &str)
{
    using namespace ApplicationStrings;
    using namespace ApplicationUtilities;
    std::lock_guard<std::mutex> ioLock{this->m_printToTerminalMutex};
    this->m_ui->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_ui->terminal->append(QString{"%1%2"}.arg(TERMINAL_TRANSMIT_BASE_STRING, str.c_str()));
}

void MainWindow::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->m_ui->sendBox->clearFocus();
            this->onReturnKeyPressed();
        } else if (qke->key() == Qt::Key_Up) {
            this->onUpArrowPressed();
        } else if (qke->key() == Qt::Key_Down) {
            this->onDownArrowPressed();
        } else if (qke->key() == Qt::Key_Escape) {
            this->onEscapeKeyPressed();
        } else if (qke->key() == Qt::Key_Alt) {
            this->onAltKeyPressed();
        } else if ((qke->key() == Qt::Key_A) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            this->onCtrlAPressed();
        } else if ((qke->key() == Qt::Key_E) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            this->onCtrlEPressed();
        } else if ((qke->key() == Qt::Key_U) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            this->onCtrlUPressed();
        } else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            this->onCtrlGPressed();
        } else if ((qke->key() == Qt::Key_C) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            this->onCtrlCPressed();
        }
        else {
            return QWidget::keyPressEvent(qke);
        }
    }
}

void MainWindow::onReturnKeyPressed()
{
    this->m_ui->sendButton->click();
}

void MainWindow::onUpArrowPressed()
{
    if ((this->m_currentHistoryIndex >= this->m_commandHistory.size()-1) || (this->m_commandHistory.empty())) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoCommandHistory) {
            this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_ui->sendBox->text());
            this->m_currentLinePushedIntoCommandHistory = true;
        }
    }
    this->m_currentHistoryIndex++;
    this->m_ui->sendBox->setText(this->m_commandHistory.at(this->m_currentHistoryIndex));
}

void MainWindow::onDownArrowPressed()
{
    if (this->m_currentHistoryIndex == 0) {
        return;
    }
    this->m_ui->sendBox->setText(this->m_commandHistory.at(--this->m_currentHistoryIndex));
}

void MainWindow::onEscapeKeyPressed()
{

}

void MainWindow::onAltKeyPressed()
{

}

void MainWindow::onCtrlAPressed()
{
     this->m_ui->sendBox->setCursorPosition(0);
}

void MainWindow::onCtrlEPressed()
{
    this->m_ui->sendBox->setCursorPosition(this->m_ui->sendBox->text().size());
}

void MainWindow::onCtrlUPressed()
{
    this->m_ui->sendBox->clear();
}

void MainWindow::onCtrlGPressed()
{
    this->m_ui->terminal->clear();
}

void MainWindow::onCtrlCPressed()
{
    using namespace ApplicationStrings;
}

void MainWindow::onConnectButtonClicked(bool checked)
{
    if ((this->m_ui->connectButton->isChecked()) || (checked)) {
        this->onActionConnectTriggered(checked);
    } else {
        this->onActionDisconnectTriggered(checked);
    }
}

void MainWindow::onDisconnectButtonClicked(bool checked)
{
    (void)checked;
    this->onActionDisconnectTriggered(checked);
}

void MainWindow::onApplicationAboutToClose()
{

}

bool MainWindow::eventFilter(QObject *sender, QEvent *event) {
    QKeyEvent *keyEvent{dynamic_cast<QKeyEvent *>(event)};
    if ( (keyEvent) && (keyEvent->type() == QKeyEvent::Type::KeyRelease) && (sender != this) ) {
        this->keyReleaseEvent(keyEvent);
        /*
        if ((keyEvent->key() == Qt::Key_Return) || (keyEvent->key() == Qt::Key_Enter)) {
            this->onSendButtonClicked();
        }
         */
    }

    return QObject::eventFilter(sender, event);
}

MainWindow::~MainWindow() {
    delete this->m_ui;
}
