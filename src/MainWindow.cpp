#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SerialPort.h"
#include "ApplicationUtilities.h"
#include "EventTimer.h"
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
#include <QtCore/QtCore>

using namespace CppSerialPort;

const int MainWindow::s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT{5000};
const int MainWindow::s_SERIAL_TIMEOUT{0};
const int MainWindow::s_TASKBAR_HEIGHT{15};
const int MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT{750};
const int MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT{1};
const int MainWindow::s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT{5000};
const int MainWindow::s_SCRIPT_INDENT{0};
const int MainWindow::s_SERIAL_READ_TIMEOUT{500};
const CppSerialPort::BaudRate MainWindow::s_DEFAULT_BAUD_RATE{CppSerialPort::BaudRate::BAUD9600};
const CppSerialPort::Parity MainWindow::s_DEFAULT_PARITY{CppSerialPort::Parity::NONE};
const CppSerialPort::StopBits MainWindow::s_DEFAULT_STOP_BITS{CppSerialPort::StopBits::ONE};
const CppSerialPort::DataBits MainWindow::s_DEFAULT_DATA_BITS{CppSerialPort::DataBits::EIGHT};
const int MainWindow::s_STATUS_BAR_FONT_POINT_SIZE{12};
const std::string MainWindow::s_CARRIAGE_RETURN_LINE_ENDING{R"(\r)"};
const std::string MainWindow::s_NEW_LINE_LINE_ENDING{R"(\n)"};
const std::string MainWindow::s_CARRIAGE_RETURN_NEW_LINE_LINE_ENDING{R"(\r\n)"};
const std::list<std::string> MainWindow::s_AVAILABLE_LINE_ENDINGS{s_CARRIAGE_RETURN_LINE_ENDING,
                                                                  s_NEW_LINE_LINE_ENDING,
                                                                  s_CARRIAGE_RETURN_NEW_LINE_LINE_ENDING};
const std::string MainWindow::s_DEFAULT_LINE_ENDING{MainWindow::s_CARRIAGE_RETURN_NEW_LINE_LINE_ENDING};

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
    tempFont.setPointSize(MainWindow::s_STATUS_BAR_FONT_POINT_SIZE);
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

    /* initialize all strings and stuff for the BoardResizeWindow */
    this->m_aboutApplicationWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_aboutApplicationWidget->setWindowTitle(MainWindow::tr(MAIN_WINDOW_TITLE));
    this->m_aboutApplicationWidget->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
    this->connect(this->m_ui->actionAboutQSerialTerminal, &QAction::triggered, this, &MainWindow::onAboutQSerialTerminalActionTriggered);
    this->connect(this->m_aboutApplicationWidget.get(), &AboutApplicationWidget::aboutToClose, this, &MainWindow::onAboutApplicationWidgetWindowClosed);

    this->m_checkPortDisconnectTimer->setInterval(MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT);
    this->m_checkSerialPortReceiveTimer->setInterval(MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT);

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

void MainWindow::setLineEnding(const std::string &lineEnding)
{
    using namespace ApplicationStrings;
    if (lineEnding.find(MainWindow::s_CARRIAGE_RETURN_LINE_ENDING) != std::string::npos) {
        if (lineEnding.find(MainWindow::s_NEW_LINE_LINE_ENDING) != std::string::npos) {
            this->m_lineEnding = "\r\n";
        } else {
            this->m_lineEnding = "\r";
        }
    } else if (lineEnding.find(MainWindow::s_NEW_LINE_LINE_ENDING) != std::string::npos)  {
        this->m_lineEnding = "\n";
    } else {
        throw std::runtime_error("setLineEnding (invalid line ending \"" + lineEnding + "\"");
    }
}

void MainWindow::autoSetLineEnding()
{
    for (auto &it : this->m_availableLineEndingActions) {
        if (it->isChecked()) {
            if (this->m_byteStream) {
                this->setLineEnding(it->text().toStdString());
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
    auto names = CppSerialPort::SerialPort::availableSerialPorts();
    auto maybeUpdate = std::vector<std::string>{};
    auto currents = std::vector<std::string>{};

    for (auto &it : names) {
        maybeUpdate.push_back(it);
    }
    for (auto &it : this->m_availablePortNamesActions) {
        currents.push_back(it->text().toStdString());
    }
    for (auto &it : maybeUpdate) {
        if (std::find(currents.begin(), currents.end(), it) == currents.end()) {
            this->addNewSerialPortInfoItem(SerialPortItemType::PORT_NAME, it);
        }
    }
    for (auto &it : currents) {
        if (std::find(maybeUpdate.begin(), maybeUpdate.end(), it) == maybeUpdate.end()) {
            this->removeOldSerialPortInfoItem(SerialPortItemType::PORT_NAME, it);
        }
    }
}

void MainWindow::addNewSerialPortInfoItem(SerialPortItemType serialPortItemType, const std::string &actionName)
{
    QAction *tempAction{new QAction{actionName.c_str(), this}};
    tempAction->setProperty(ApplicationStrings::ACTION_INDEX_PROPERTY_TAG, QVariant{0});
    tempAction->setCheckable(true);

    if (serialPortItemType == SerialPortItemType::PORT_NAME) {
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionPortNamesChecked);
        this->m_availablePortNamesActions.push_back(tempAction);
        this->m_ui->menuPortNames->addAction(tempAction);
    } else if (serialPortItemType == SerialPortItemType::BAUD_RATE) {
        if (tempAction->text().toStdString() == CppSerialPort::SerialPort::baudRateToString(s_DEFAULT_BAUD_RATE)) {
            tempAction->setChecked(true);
        }
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionBaudRateChecked);
        this->m_availableBaudRateActions.push_back(tempAction);
        this->m_ui->menuBaudRate->addAction(tempAction);
    } else if (serialPortItemType == SerialPortItemType::PARITY) {
        if (tempAction->text().toStdString() == CppSerialPort::SerialPort::parityToString(s_DEFAULT_PARITY)) {
            tempAction->setChecked(true);
        }
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionParityChecked);
        this->m_availableParityActions.push_back(tempAction);
        this->m_ui->menuParity->addAction(tempAction);
    } else if (serialPortItemType == SerialPortItemType::DATA_BITS) {
        if (tempAction->text().toStdString() == CppSerialPort::SerialPort::dataBitsToString(s_DEFAULT_DATA_BITS)) {
            tempAction->setChecked(true);
        }
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionDataBitsChecked);
        this->m_availableDataBitsActions.push_back(tempAction);
        this->m_ui->menuDataBits->addAction(tempAction);
    } else if (serialPortItemType == SerialPortItemType::STOP_BITS) {
        if (tempAction->text().toStdString() == CppSerialPort::SerialPort::stopBitsToString(s_DEFAULT_STOP_BITS)) {
            tempAction->setChecked(true);
        }
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionStopBitsChecked);
        this->m_availableStopBitsActions.push_back(tempAction);
        this->m_ui->menuStopBits->addAction(tempAction);
    } else if (serialPortItemType == SerialPortItemType::LINE_ENDING) {
        if (tempAction->text().toStdString() == MainWindow::s_DEFAULT_LINE_ENDING) {
            tempAction->setChecked(true);
        }
        connect(tempAction, &QAction::triggered, this, &MainWindow::onActionLineEndingsChecked);
        this->m_availableLineEndingActions.push_back(tempAction);
        this->m_ui->menuLineEndings->addAction(tempAction);
    } else {
        delete tempAction;
    }
}

void MainWindow::removeOldSerialPortInfoItem(SerialPortItemType serialPortItemType, const std::string &actionName)
{
    using namespace ApplicationStrings;
    int i{0};
    if (serialPortItemType == SerialPortItemType::PORT_NAME) {
        for (auto &it : this->m_availablePortNamesActions) {
            if (it->text().toStdString() == actionName) {
                this->setStatusBarLabelText(QString{SERIAL_PORT_DISCONNECTED_STRING + it->text()});
                if (it->isChecked()) {
                    this->m_ui->actionConnect->setChecked(false);
                    this->m_ui->connectButton->setChecked(false);
                    this->m_ui->actionConnect->setEnabled(false);
                    this->m_ui->connectButton->setEnabled(false);
                    this->m_ui->actionLoadScript->setEnabled(false);
                    this->m_ui->sendBox->setEnabled(false);
                    this->m_ui->sendButton->setEnabled(false);
                }
                it->deleteLater();
                this->m_availablePortNamesActions.erase(this->m_availablePortNamesActions.begin() + i);
                return;
            }
            i++;
        }
    } else if (serialPortItemType == SerialPortItemType::BAUD_RATE) {
        for (auto &it : this->m_availableBaudRateActions) {
            if (it->text().toStdString() == actionName) {
                it->deleteLater();
                this->m_availableBaudRateActions.erase(this->m_availableBaudRateActions.begin() + i);
                return;
            }
            i++;
        }
    } else if (serialPortItemType == SerialPortItemType::PARITY) {
        for (auto &it : this->m_availableParityActions) {
            if (it->text().toStdString() == actionName) {
                it->deleteLater();
                this->m_availableParityActions.erase(this->m_availableParityActions.begin() + i);
                return;
            }
            i++;
        }
    } else if (serialPortItemType == SerialPortItemType::DATA_BITS) {
        for (auto &it : this->m_availableDataBitsActions) {
            if (it->text().toStdString() == actionName) {
                it->deleteLater();
                this->m_availableDataBitsActions.erase(this->m_availableDataBitsActions.begin() + i);
                return;
            }
            i++;
        }
    } else if (serialPortItemType == SerialPortItemType::STOP_BITS) {
        for (auto &it : this->m_availableStopBitsActions) {
            if (it->text().toStdString() == actionName) {
                it->deleteLater();
                this->m_availableStopBitsActions.erase(this->m_availableStopBitsActions.begin() + i);
                return;
            }
            i++;
        }
    }
}

void MainWindow::launchSerialReceiveAsync()
{
    if (!this->m_byteStream) {
        return;
    }
    if (!this->m_byteStream->isOpen()) {
        return;
    }
    try {
        if (!this->m_serialReceiveAsyncHandle) {
           this->m_serialReceiveAsyncHandle = std::unique_ptr< std::future<std::string> >{new std::future<std::string>{std::async(std::launch::async,
                                                                                        &MainWindow::checkSerialReceive,
                                                                                        this)}};
        }
        if (this->m_serialReceiveAsyncHandle->wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            appendReceivedString(this->m_serialReceiveAsyncHandle->get());
            this->m_serialReceiveAsyncHandle = std::unique_ptr<std::future<std::string>>{new std::future<std::string>{std::async(std::launch::async,
                                                                                          &MainWindow::checkSerialReceive,
                                                                                          this)}};
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

std::string MainWindow::checkSerialReceive()
{
    if (this->m_byteStream) {
        this->autoSetLineEnding();
        this->m_byteStream->setLineEnding(this->m_lineEnding);
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


    for (auto &it : MainWindow::s_AVAILABLE_LINE_ENDINGS) {
        this->addNewSerialPortInfoItem(SerialPortItemType::LINE_ENDING, it);
    }

    this->addNewSerialPortInfoItem(SerialPortItemType::DATA_BITS, CppSerialPort::SerialPort::dataBitsToString(CppSerialPort::DataBits::FIVE));
    this->addNewSerialPortInfoItem(SerialPortItemType::DATA_BITS, CppSerialPort::SerialPort::dataBitsToString(CppSerialPort::DataBits::SIX));
    this->addNewSerialPortInfoItem(SerialPortItemType::DATA_BITS, CppSerialPort::SerialPort::dataBitsToString(CppSerialPort::DataBits::SEVEN));
    this->addNewSerialPortInfoItem(SerialPortItemType::DATA_BITS, CppSerialPort::SerialPort::dataBitsToString(CppSerialPort::DataBits::EIGHT));

    this->addNewSerialPortInfoItem(SerialPortItemType::STOP_BITS, CppSerialPort::SerialPort::stopBitsToString(CppSerialPort::StopBits::ONE));
    this->addNewSerialPortInfoItem(SerialPortItemType::STOP_BITS, CppSerialPort::SerialPort::stopBitsToString(CppSerialPort::StopBits::TWO));

    this->addNewSerialPortInfoItem(SerialPortItemType::PARITY, CppSerialPort::SerialPort::parityToString(CppSerialPort::Parity::NONE));
    this->addNewSerialPortInfoItem(SerialPortItemType::PARITY, CppSerialPort::SerialPort::parityToString(CppSerialPort::Parity::EVEN));
    this->addNewSerialPortInfoItem(SerialPortItemType::PARITY, CppSerialPort::SerialPort::parityToString(CppSerialPort::Parity::ODD));

    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD300));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD2400));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD4800));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD9600));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD19200));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD38400));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD57600));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD115200));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD230400));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD460800));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD1000000));
    this->addNewSerialPortInfoItem(SerialPortItemType::BAUD_RATE, CppSerialPort::SerialPort::baudRateToString(CppSerialPort::BaudRate::BAUD2000000));

    for (auto &it : CppSerialPort::SerialPort::availableSerialPorts()) {
        this->addNewSerialPortInfoItem(SerialPortItemType::PORT_NAME, it);
    }

    if (!this->m_availablePortNamesActions.empty()) {
        (*std::begin(this->m_availablePortNamesActions))->setChecked(true);
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

void MainWindow::onActionBaudRateChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    for (auto &it : this->m_availableBaudRateActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionParityChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    for (auto &it : this->m_availableParityActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionDataBitsChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    for (auto &it : this->m_availableDataBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionStopBitsChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    for (auto &it : this->m_availableStopBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionLineEndingsChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    for (auto &it : this->m_availableLineEndingActions) {
        if (it == action) {
            action->setChecked(true);
            if (this->m_byteStream) {
                this->setLineEnding(action->text().toStdString());
                this->m_byteStream->setLineEnding(this->m_lineEnding);
            }
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionPortNamesChecked(bool checked)
{
    QAction *action{dynamic_cast<QAction *>(QObject::sender())};
    (void)checked;
    if (!action->isChecked()) {
        this->m_ui->connectButton->setEnabled(false);
    } else {
        this->m_ui->connectButton->setEnabled(true);
    }
    for (auto &it : this->m_availablePortNamesActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

std::string MainWindow::getSerialPortItemFromActions(SerialPortItemType serialPortItemType)
{
    if (serialPortItemType == SerialPortItemType::PORT_NAME) {
        for (auto &it : this->m_availablePortNamesActions) {
            if (it->isChecked()) {
                return it->text().toStdString();
            }
        }
    } else if (serialPortItemType == SerialPortItemType::BAUD_RATE) {
        for (auto &it : this->m_availableBaudRateActions) {
            if (it->isChecked()) {
                return it->text().toStdString();
            }
        }
    } else if (serialPortItemType == SerialPortItemType::PARITY) {
        for (auto &it : this->m_availableParityActions) {
            if (it->isChecked()) {
                return it->text().toStdString();
            }
        }
    } else if (serialPortItemType == SerialPortItemType::DATA_BITS) {
        for (auto &it : this->m_availableDataBitsActions) {
            if (it->isChecked()) {
                return it->text().toStdString();
            }
        }
    } else if (serialPortItemType == SerialPortItemType::STOP_BITS) {
        for (auto &it : this->m_availableStopBitsActions) {
            if (it->isChecked()) {
                return it->text().toStdString();
            }
        }
    }
    return "";
}

void MainWindow::onActionConnectTriggered(bool checked)
{
    if (!checked) {
        this->onActionDisconnectTriggered(checked);
        return;
    }
    using namespace ApplicationStrings;
    this->pauseCommunication();

    CppSerialPort::BaudRate baudRate{CppSerialPort::SerialPort::parseBaudRateFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::BAUD_RATE))};
    CppSerialPort::Parity parity{CppSerialPort::SerialPort::parseParityFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::PARITY))};
    CppSerialPort::StopBits stopBits{CppSerialPort::SerialPort::parseStopBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::STOP_BITS))};
    CppSerialPort::DataBits dataBits{CppSerialPort::SerialPort::parseDataBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::DATA_BITS))};
    std::string portName{this->getSerialPortItemFromActions(SerialPortItemType::PORT_NAME)};
    if (CppSerialPort::SerialPort *serialPort{dynamic_cast<CppSerialPort::SerialPort *>(this->m_byteStream.get())}) {
        if (baudRate == serialPort->baudRate() &&
                parity == serialPort->parity() &&
                stopBits == serialPort->stopBits() &&
                dataBits == serialPort->dataBits() &&
                portName == serialPort->portName()) {
            if (!this->m_byteStream->isOpen()) {
                openSerialPort();
            }
            return;
        }
        try {
            closeSerialPort();
            this->m_byteStream.reset();
            this->m_byteStream = std::make_shared<CppSerialPort::SerialPort>(portName, baudRate, dataBits, stopBits, parity);
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
        this->m_byteStream->setReadTimeout(MainWindow::s_SERIAL_READ_TIMEOUT);
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
        CppSerialPort::BaudRate baudRate{CppSerialPort::SerialPort::parseBaudRateFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::BAUD_RATE))};
        CppSerialPort::Parity parity{CppSerialPort::SerialPort::parseParityFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::PARITY))};
        CppSerialPort::StopBits stopBits{CppSerialPort::SerialPort::parseStopBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::STOP_BITS))};
        CppSerialPort::DataBits dataBits{CppSerialPort::SerialPort::parseDataBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::DATA_BITS))};
        std::string portName{this->getSerialPortItemFromActions(SerialPortItemType::PORT_NAME)};
        try {
            this->m_byteStream = std::make_shared<CppSerialPort::SerialPort>(portName, baudRate, dataBits, stopBits, parity);
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
        this->m_ui->terminal->append(QString{"%1%2%3"}.arg(nWhitespace((size_t) MainWindow::s_SCRIPT_INDENT).c_str(), TERMINAL_RECEIVE_BASE_STRING, stripLineEndings(str).c_str()));
    }
}

void MainWindow::printTxResult(const std::string &str)
{
    using namespace ApplicationStrings;
    using namespace ApplicationUtilities;
    std::lock_guard<std::mutex> ioLock{this->m_printToTerminalMutex};
    this->m_ui->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_ui->terminal->append(QString{"%1%2%3"}.arg(nWhitespace(MainWindow::s_SCRIPT_INDENT).c_str(), TERMINAL_TRANSMIT_BASE_STRING, str.c_str()));
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
    if (this->m_byteStream && this->m_byteStream->isOpen()) {
        this->m_byteStream->closePort();
    }
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
