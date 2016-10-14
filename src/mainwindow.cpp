#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_settings-dialog.h"

const int MainWindow::s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT{500};
const int MainWindow::s_SERIAL_TIMEOUT{400};
const int MainWindow::s_TASKBAR_HEIGHT{15};
const int MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT{750};
const int MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT{50};

MainWindow::MainWindow(std::shared_ptr<QDesktopWidget> qDesktopWidget,
                       std::shared_ptr<QSerialTerminalIcons> qstiPtr,
                       QWidget *parent) :
    QMainWindow{parent},
    m_uiPtr{std::make_shared<Ui::MainWindow>()},
    m_sduiPtr{std::make_shared<Ui::SettingsDialog>()},
    m_portSettingsDialog{std::make_shared<CustomDialog>()},
    m_checkPortDisconnectTimer{std::make_unique<QTimer>()},
    m_checkSerialPortReceiveTimer{std::make_unique<QTimer>()},
    m_serialReceiveTimer{std::make_unique<EventTimer>()},
    m_qstiPtr{qstiPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_serialPortNames{SerialPort::availableSerialPorts()},
    m_temporarySerialPortBuffer{""},
    m_xPlacement{0},
    m_yPlacement{0}
{
    this->m_uiPtr->setupUi(this);
    this->m_sduiPtr->setupUi(this->m_portSettingsDialog.get());

    setupSettingsDialog();

    connect(this->m_portSettingsDialog.get(), SIGNAL(aboutToClose()), this, SLOT(onCustomDialogClosed()));
    connect(this->m_sduiPtr->okayButton, SIGNAL(clicked(bool)), this, SLOT(onSduiOkayButtonClicked()));
    connect(this->m_sduiPtr->cancelButton, SIGNAL(clicked(bool)), this, SLOT(onSduiCancelButtonClicked()));
    connect(this->m_uiPtr->actionPortSettings, SIGNAL(triggered(bool)), this, SLOT(onActionPortSettingsTriggered()));
    connect(this->m_uiPtr->sendButton, SIGNAL(clicked(bool)), this, SLOT(onSendButtonClicked()));
    connect(this->m_uiPtr->statusBar, SIGNAL(messageChanged(QString)), this, SLOT(onStatusBarMessageChanged(QString)));
    connect(this->m_sduiPtr->portNameComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onPortNameComboBoxIndexChanged(QString)));

    this->m_checkPortDisconnectTimer->setInterval(MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT);
    this->m_checkSerialPortReceiveTimer->setInterval(MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT);
    connect(this->m_checkPortDisconnectTimer.get(), SIGNAL(timeout()), this, SLOT(checkDisconnectedSerialPorts()));
    connect(this->m_checkSerialPortReceiveTimer.get(), SIGNAL(timeout()), this, SLOT(checkSerialReceive()));
    this->m_sduiPtr->okayButton->setEnabled(this->m_sduiPtr->portNameComboBox->count() != 0);
    connect(this->m_uiPtr->sendBox, SIGNAL(enterPressed()), this, SLOT(onEnterKeyPressed()));
}

void MainWindow::onEnterKeyPressed()
{
    this->m_uiPtr->sendButton->click();
}

void MainWindow::onStatusBarMessageChanged(QString newMessage)
{
    using namespace QSerialTerminalStrings;
    if (newMessage.toStdString().find(SUCCESSFULLY_OPENED_SERIAL_PORT_STRING) != std::string::npos) {
        this->m_uiPtr->statusBar->showMessage(this->m_serialPort->portName().c_str());
    }
}

void MainWindow::onPortNameComboBoxIndexChanged(QString newIndex)
{
    //this->m_sduiPtr->okayButton->setEnabled(newIndex == "");
}

void MainWindow::bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget)
{
    this->m_qDesktopWidget.reset();
    this->m_qDesktopWidget = qDesktopWidget;
}

void MainWindow::bindQSerialTerminalIcons(std::shared_ptr<QSerialTerminalIcons> qstiPtr)
{
    this->m_qstiPtr.reset();
    this->m_qstiPtr = qstiPtr;
}

void MainWindow::checkDisconnectedSerialPorts()
{
    /*
    using namespace QSerialTerminalUtilities;
    this->m_sduiPtr->okayButton->setEnabled(this->m_sduiPtr->portNameComboBox->count() != 0);
    std::vector<std::string> currentSerialPorts{SerialPort::availableSerialPorts()};

    for (auto &it : currentSerialPorts) {
        if (this->m_sduiPtr->portNameComboBox->findData(toQString(it)) == -1) {
            this->m_sduiPtr->portNameComboBox->addItem(toQString(it), toQString(it));
        }
    }

    for (int comboBoxIndex = 0; comboBoxIndex < this->m_sduiPtr->portNameComboBox->count(); comboBoxIndex++) {
        bool deletionNeeded{true};
        for (auto &it : currentSerialPorts) {
            if (toQString(it) == this->m_sduiPtr->portNameComboBox->itemText(comboBoxIndex)) {
                deletionNeeded = false;
            }
        }
        if (deletionNeeded) {
            this->m_sduiPtr->portNameComboBox->removeItem(comboBoxIndex);
        }
    }
    */
}

void MainWindow::checkSerialReceive()
{
    if (!this->m_serialReceiveTimer->isPaused()) {
        this->m_serialReceiveTimer->update();
    }
    if (this->m_serialPort) {
        if (!this->m_serialPort->isOpen()) {
            this->m_serialPort->openPort();
        }
        this->m_serialPort->flushRXTX();
        std::string temp{this->m_serialPort->readString()};
        if (temp != "") {
            this->m_serialReceiveTimer->start();
            this->m_temporarySerialPortBuffer += temp;
        }
        if (this->m_serialReceiveTimer->totalTimeMilliseconds() > MainWindow::s_SERIAL_TIMEOUT) {
            this->m_uiPtr->terminal->append(this->m_temporarySerialPortBuffer.c_str());
            this->m_temporarySerialPortBuffer.clear();
        }
    }
}

void MainWindow::centerAndFitWindow()
{
    this->setFixedSize(this->minimumSize());
    calculateXYPlacement();
    this->move(this->m_xPlacement, this->m_yPlacement);
}

int MainWindow::xPlacement() const
{
    return this->m_xPlacement;
}

int MainWindow::yPlacement() const
{
    return this->m_yPlacement;
}

void MainWindow::calculateXYPlacement()
{
    std::unique_ptr<QRect> avail{std::make_unique<QRect>(this->m_qDesktopWidget->availableGeometry())};
    this->m_xPlacement = (avail->width()/2)-(this->width()/2);
    this->m_yPlacement = (avail->height()/2)-(this->height()/2) - MainWindow::s_TASKBAR_HEIGHT;
}

void MainWindow::onSendButtonClicked()
{
    if (this->m_serialPort) {
        this->m_serialPort->writeString(this->m_uiPtr->sendBox->text().toStdString());
    } else {
        onActionPortSettingsTriggered();
    }
}

void MainWindow::onActionPortSettingsTriggered()
{
    this->setEnabled(false);
    this->m_portSettingsDialog->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    using namespace QSerialTerminalStrings;
    QMessageBox::StandardButton userReply;
    userReply = QMessageBox::question(this, QUIT_PROMPT_WINDOW_TITLE_STRING, QUIT_PROMPT_STRING, QMessageBox::Yes|QMessageBox::No);
    if (userReply == QMessageBox::Yes) {
        this->m_portSettingsDialog->close();
        event->accept();
        QMainWindow::closeEvent(event);
    } else {
        event->ignore();
    }
}

void MainWindow::onCustomDialogClosed()
{
    this->setEnabled(true);
}

void MainWindow::setupSettingsDialog()
{
    using namespace QSerialTerminalStrings;
    using namespace QSerialTerminalUtilities;
    this->m_portSettingsDialog->setWindowTitle(SETTINGS_DIALOG_WINDOW_TITLE);

    for (auto &it : SerialPort::availableDataBits()) {
        this->m_sduiPtr->dataBitsComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableStopBits()) {
        this->m_sduiPtr->stopBitsComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableParity()) {
        this->m_sduiPtr->parityComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableBaudRates()) {
        this->m_sduiPtr->baudRateComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableSerialPorts()) {
        this->m_sduiPtr->portNameComboBox->addItem(toQString(it), toQString(it));
    }
    this->m_sduiPtr->portNameComboBox->setCurrentIndex(0);
    this->m_sduiPtr->dataBitsComboBox->setCurrentIndex(this->m_sduiPtr->dataBitsComboBox->findData(toQString(SerialPort::DEFAULT_DATA_BITS_STRING)));
    this->m_sduiPtr->stopBitsComboBox->setCurrentIndex(this->m_sduiPtr->stopBitsComboBox->findData(toQString(SerialPort::DEFAULT_STOP_BITS_STRING)));
    this->m_sduiPtr->parityComboBox->setCurrentIndex(this->m_sduiPtr->parityComboBox->findData(toQString(SerialPort::DEFAULT_PARITY_STRING)));
    this->m_sduiPtr->baudRateComboBox->setCurrentIndex(this->m_sduiPtr->baudRateComboBox->findData(toQString(SerialPort::DEFAULT_BAUD_RATE_STRING)));
    /*
    this->m_sduiPtr->portNameComboBox->setEditable(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setReadOnly(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    this->m_sduiPtr->portNameComboBox->setItemData(Qt::AlignCenter, Qt::TextAlignmentRole);

    this->m_sduiPtr->portNameComboBox->setEditable(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setReadOnly(true);
    this->m_sduiPtr->portNameComboBox->setItemData(Qt::AlignCenter, Qt::TextAlignmentRole);

    this->m_sduiPtr->portNameComboBox->setEditable(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setReadOnly(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    this->m_sduiPtr->portNameComboBox->setItemData(Qt::AlignCenter, Qt::TextAlignmentRole);

    this->m_sduiPtr->portNameComboBox->setEditable(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setReadOnly(true);
    this->m_sduiPtr->portNameComboBox->lineEdit()->setAlignment(Qt::AlignCenter);
    this->m_sduiPtr->portNameComboBox->setItemData(Qt::AlignCenter, Qt::TextAlignmentRole);
    */
}

void MainWindow::onSduiOkayButtonClicked()
{
    using namespace QSerialTerminalStrings;
    using namespace QSerialTerminalUtilities;
    this->pauseCommunication();
    BaudRate baudRate{SerialPort::parseBaudRateFromRaw(this->m_sduiPtr->baudRateComboBox->currentText().toStdString())};
    Parity parity{SerialPort::parseParityFromRaw(this->m_sduiPtr->parityComboBox->currentText().toStdString())};
    StopBits stopBits{SerialPort::parseStopBitsFromRaw(this->m_sduiPtr->stopBitsComboBox->currentText().toStdString())};
    DataBits dataBits{SerialPort::parseDataBitsFromRaw(this->m_sduiPtr->dataBitsComboBox->currentText().toStdString())};
    std::string portName{this->m_sduiPtr->portNameComboBox->currentText().toStdString()};
    if (this->m_serialPort) {
        this->m_serialPort->closePort();
        if (baudRate == this->m_serialPort->baudRate() &&
            parity == this->m_serialPort->parity() &&
            stopBits == this->m_serialPort->stopBits() &&
            dataBits == this->m_serialPort->dataBits() &&
            portName == this->m_serialPort->portName()) {

            this->setEnabled(true);
            this->m_portSettingsDialog->hide();
            beginCommunication();
        } else {
            this->m_serialPort->openPort();
        }
    } else {
        try {
            this->m_serialPort = std::make_shared<SerialPort>(portName, baudRate, dataBits, stopBits, parity);
            this->m_serialPort->openPort();
            this->m_serialPort->writeString("{ioreport}");
            this->setEnabled(true);
            this->m_portSettingsDialog->hide();
            beginCommunication();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(INVALID_SETTINGS_DETECTED_STRING + toQString(e.what()));
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        }
    }
}

void MainWindow::begin()
{
    this->show();
    this->m_portSettingsDialog->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
    this->centerAndFitWindow();
    this->m_checkPortDisconnectTimer->start();
    this->onActionPortSettingsTriggered();

}

void MainWindow::beginCommunication()
{
    using namespace QSerialTerminalUtilities;
    using namespace QSerialTerminalStrings;
    this->setEnabled(true);
    this->m_portSettingsDialog->hide();
    if (this->m_serialPort) {
        try {
            if (!this->m_serialPort->isOpen()) {
                this->m_serialPort->openPort();
            }
            this->setWindowTitle(this->windowTitle() + " - " + toQString(this->m_serialPort->portName()));
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_OPENED_SERIAL_PORT_STRING) + toQString(this->m_serialPort->portName()), MainWindow::s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT);
            this->m_serialReceiveTimer->start();
            this->m_checkSerialPortReceiveTimer->start();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(INVALID_SETTINGS_DETECTED_STRING + toQString(e.what()));
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        }
    }
}

void MainWindow::pauseCommunication()
{

}

void MainWindow::stopCommunication()
{
    this->m_checkSerialPortReceiveTimer->stop();

    if (this->m_serialPort) {
        this->m_serialPort->closePort();
    }
}

void MainWindow::onSduiCancelButtonClicked()
{
    beginCommunication();
}


MainWindow::~MainWindow()
{

}
