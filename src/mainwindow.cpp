#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MainWindow::s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT{5000};
const int MainWindow::s_SERIAL_TIMEOUT{400};
const int MainWindow::s_TASKBAR_HEIGHT{15};
const int MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT{750};
const int MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT{50};
const int MainWindow::s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT{5000};

MainWindow::MainWindow(std::shared_ptr<QDesktopWidget> qDesktopWidget,
                       std::shared_ptr<QSerialTerminalIcons> qstiPtr,
                       QWidget *parent) :
    QMainWindow{parent},
    m_uiPtr{std::make_shared<Ui::MainWindow>()},
    m_checkPortDisconnectTimer{std::make_unique<QTimer>()},
    m_checkSerialPortReceiveTimer{std::make_unique<QTimer>()},
    m_qstiPtr{qstiPtr},
    m_qDesktopWidget{qDesktopWidget},
    m_serialPortNames{SerialPort::availableSerialPorts()},
    m_packagedRxResultTask{MainWindow::staticPrintRxResult},
    m_packagedTxResultTask{MainWindow::staticPrintTxResult},
    m_packagedDelayResultTask{MainWindow::staticPrintDelayResult},
    m_packagedFlushResultTask{MainWindow::staticPrintFlushResult},
    m_packagedLoopResultTask{MainWindow::staticPrintLoopResult},
    m_currentHistoryIndex{0},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace QSerialTerminalStrings;
    this->m_uiPtr->setupUi(this);

    setupAdditionalUiComponents();

    connect(this->m_uiPtr->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(this->m_uiPtr->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onApplicationAboutToClose()));

    connect(this->m_uiPtr->actionConnect, SIGNAL(triggered(bool)), this, SLOT(onActionConnectTriggered(bool)));
    connect(this->m_uiPtr->connectButton, SIGNAL(clicked(bool)), this, SLOT(onConnectButtonClicked(bool)));
    connect(this->m_uiPtr->actionDisconnect, SIGNAL(triggered(bool)), this, SLOT(onActionDisconnectTriggered(bool)));
    connect(this->m_uiPtr->disconnectButton, SIGNAL(clicked(bool)), this, SLOT(onDisconnectButtonClicked(bool)));
    connect(this->m_uiPtr->actionLoadScript, SIGNAL(triggered(bool)), this, SLOT(onActionLoadScriptTriggered(bool)));

    connect(this->m_uiPtr->sendButton, SIGNAL(clicked(bool)), this, SLOT(onSendButtonClicked()));
    connect(this->m_uiPtr->sendBox, SIGNAL(returnPressed(SerialTerminalLineEdit*)), this, SLOT(onReturnKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(upArrowPressed(SerialTerminalLineEdit*)), this, SLOT(onUpArrowPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(downArrowPressed(SerialTerminalLineEdit*)), this, SLOT(onDownArrowPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(escapePressed(SerialTerminalLineEdit*)), this, SLOT(onEscapeKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(altPressed(SerialTerminalLineEdit*)), this, SLOT(onAltKeyPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlAPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlAPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlEPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlEPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlUPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlUPressed(SerialTerminalLineEdit*)));
    connect(this->m_uiPtr->sendBox, SIGNAL(ctrlGPressed(SerialTerminalLineEdit*)), this, SLOT(onCtrlGPressed(SerialTerminalLineEdit*)));

    this->m_checkPortDisconnectTimer->setInterval(MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT);
    this->m_checkSerialPortReceiveTimer->setInterval(MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT);
    connect(this->m_checkPortDisconnectTimer.get(), SIGNAL(timeout()), this, SLOT(checkDisconnectedSerialPorts()));
    connect(this->m_checkSerialPortReceiveTimer.get(), SIGNAL(timeout()), this, SLOT(checkSerialReceive()));
}

void MainWindow::appendReceivedString(const std::string &str)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if ((str != "") && (!isWhitespace(str)) && startsWith(str, '{')) {
        printRxResult(str);
    }
}

void MainWindow::appendTransmittedString(const QString &str)
{
    using namespace QSerialTerminalStrings;
    if (this->m_serialPort) {
        if (this->m_serialPort->isOpen()) {
            this->m_commandHistory.emplace_back(this->m_uiPtr->sendBox->text());
            this->m_currentHistoryIndex++;
            this->m_serialPort->writeString(str.toStdString());
            printTxResult(str.toStdString());
            this->m_uiPtr->sendBox->clear();
        }
    }
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
    using namespace QSerialTerminalUtilities;
    this->m_uiPtr->sendBox->setEnabled(this->m_uiPtr->portNameComboBox->count() != 0);
    this->m_uiPtr->sendBox->setEnabled(this->m_serialPort && (this->m_serialPort->isOpen()));
    std::vector<std::string> currentSerialPorts{SerialPort::availableSerialPorts()};

    for (auto &it : currentSerialPorts) {
        if (this->m_uiPtr->portNameComboBox->findData(toQString(it)) == -1) {
            this->m_uiPtr->portNameComboBox->addItem(toQString(it), toQString(it));
        }
    }

    for (int comboBoxIndex = 0; comboBoxIndex < this->m_uiPtr->portNameComboBox->count(); comboBoxIndex++) {
        bool deletionNeeded{true};
        std::string serialPortToDelete{""};
        for (auto &it : currentSerialPorts) {
            if (toQString(it) == this->m_uiPtr->portNameComboBox->itemText(comboBoxIndex)) {
                deletionNeeded = false;
            }
        }
        if (deletionNeeded) {
            if (this->m_serialPort) {
                if (this->m_serialPort->portName() == this->m_uiPtr->portNameComboBox->itemText(comboBoxIndex).toStdString()) {
                    if (this->m_serialPort->isOpen()) {
                        closeSerialPort();
                    }
                }
            }
            this->m_uiPtr->portNameComboBox->removeItem(comboBoxIndex);

        }
    }
}

void MainWindow::checkSerialReceive()
{
    if (this->m_serialPort) {
        if (this->m_serialPort->isOpen()) {
            appendReceivedString(this->m_serialPort->readString());
        }
    }
}

void MainWindow::centerAndFitWindow()
{
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
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if (this->m_serialPort) {
        if ((this->m_uiPtr->sendBox->text().toStdString() != "") && (!isWhitespace(this->m_uiPtr->sendBox->text().toStdString()))) {
            appendTransmittedString(toQString(stripLineEndings(this->m_uiPtr->sendBox->text().toStdString())));
        }
    } else {
        if (this->m_uiPtr->portNameComboBox->count() != 0) {
            onActionConnectTriggered(false);
            onSendButtonClicked();
        } else {
            this->m_uiPtr->statusBar->showMessage(NO_SERIAL_PORTS_CONNECTED_STRING);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    /*
    using namespace QSerialTerminalStrings;
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

void MainWindow::setupAdditionalUiComponents()
{
    using namespace QSerialTerminalStrings;
    using namespace QSerialTerminalUtilities;
    for (auto &it : SerialPort::availableDataBits()) {
        this->m_uiPtr->dataBitsComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableStopBits()) {
        this->m_uiPtr->stopBitsComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableParity()) {
        this->m_uiPtr->parityComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableBaudRates()) {
        this->m_uiPtr->baudRateComboBox->addItem(toQString(it), toQString(it));
    }

    for (auto &it : SerialPort::availableSerialPorts()) {
        this->m_uiPtr->portNameComboBox->addItem(toQString(it), toQString(it));
    }
    this->m_uiPtr->portNameComboBox->setCurrentIndex(0);
    this->m_uiPtr->dataBitsComboBox->setCurrentIndex(this->m_uiPtr->dataBitsComboBox->findData(toQString(SerialPort::DEFAULT_DATA_BITS_STRING)));
    this->m_uiPtr->stopBitsComboBox->setCurrentIndex(this->m_uiPtr->stopBitsComboBox->findData(toQString(SerialPort::DEFAULT_STOP_BITS_STRING)));
    this->m_uiPtr->parityComboBox->setCurrentIndex(this->m_uiPtr->parityComboBox->findData(toQString(SerialPort::DEFAULT_PARITY_STRING)));
    this->m_uiPtr->baudRateComboBox->setCurrentIndex(this->m_uiPtr->baudRateComboBox->findData(toQString(SerialPort::DEFAULT_BAUD_RATE_STRING)));

    this->m_uiPtr->sendBox->setEnabled(false);
    this->m_uiPtr->sendBox->setToolTip(SEND_BOX_DISABLED_TOOLTIP);
    this->m_uiPtr->actionLoadScript->setEnabled(false);
    this->m_uiPtr->actionLoadScript->setToolTip(ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP);
    this->m_uiPtr->disconnectButton->setEnabled(false);
    this->m_uiPtr->actionDisconnect->setEnabled(false);

    this->m_uiPtr->statusBar->showMessage(CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING);
}

void MainWindow::onActionConnectTriggered(bool checked)
{
    (void)checked;
    using namespace QSerialTerminalStrings;
    using namespace QSerialTerminalUtilities;
    this->pauseCommunication();
    BaudRate baudRate{SerialPort::parseBaudRateFromRaw(this->m_uiPtr->baudRateComboBox->currentText().toStdString())};
    Parity parity{SerialPort::parseParityFromRaw(this->m_uiPtr->parityComboBox->currentText().toStdString())};
    StopBits stopBits{SerialPort::parseStopBitsFromRaw(this->m_uiPtr->stopBitsComboBox->currentText().toStdString())};
    DataBits dataBits{SerialPort::parseDataBitsFromRaw(this->m_uiPtr->dataBitsComboBox->currentText().toStdString())};
    std::string portName{this->m_uiPtr->portNameComboBox->currentText().toStdString()};
    if (this->m_serialPort) {
        if (baudRate == this->m_serialPort->baudRate() &&
            parity == this->m_serialPort->parity() &&
            stopBits == this->m_serialPort->stopBits() &&
            dataBits == this->m_serialPort->dataBits() &&
            portName == this->m_serialPort->portName()) {
            if (!this->m_serialPort->isOpen()) {
                try {
                     openSerialPort();
                } catch (std::exception &e) {
                    (void)e;
                }
            }
            return;
        } else {
            try {
                closeSerialPort();
                this->m_serialPort.reset();
                this->m_serialPort = std::make_shared<SerialPort>(portName, baudRate, dataBits, stopBits, parity);
                openSerialPort();
            } catch (std::exception &e) {
                (void)e;
                std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
                warningBox->setText(INVALID_SETTINGS_DETECTED_STRING + toQString(e.what()));
                warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
                warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
                warningBox->exec();
                if (this->m_serialPort) {
                    closeSerialPort();
                }
            }
        }
    } else {
        try {
            this->m_serialPort = std::make_shared<SerialPort>(portName, baudRate, dataBits, stopBits, parity);
            openSerialPort();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(INVALID_SETTINGS_DETECTED_STRING + toQString(e.what()));
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
            if (this->m_serialPort) {
                closeSerialPort();
            }
        }
    }
}

void MainWindow::openSerialPort()
{
    using namespace QSerialTerminalStrings;
    this->m_serialPort->openPort();
    this->m_uiPtr->disconnectButton->setEnabled(true);
    this->m_uiPtr->actionDisconnect->setEnabled(true);
    this->m_uiPtr->sendBox->setEnabled(true);
    this->m_uiPtr->sendBox->setToolTip(SEND_BOX_ENABLED_TOOLTIP);
    this->m_uiPtr->actionLoadScript->setEnabled(true);
    this->m_uiPtr->actionLoadScript->setToolTip(ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP);
    this->m_uiPtr->sendBox->setFocus();
    this->m_uiPtr->statusBar->showMessage(SUCCESSFULLY_OPENED_SERIAL_PORT_STRING + toQString(this->m_serialPort->portName()));
    beginCommunication();
}

void MainWindow::closeSerialPort()
{
    using namespace QSerialTerminalStrings;
    this->m_serialPort->closePort();
    this->m_uiPtr->disconnectButton->setEnabled(false);
    this->m_uiPtr->actionDisconnect->setEnabled(false);
    this->m_uiPtr->sendBox->setEnabled(false);
    this->m_uiPtr->sendBox->setToolTip(SEND_BOX_DISABLED_TOOLTIP);
    this->m_uiPtr->actionLoadScript->setEnabled(false);
    this->m_uiPtr->actionLoadScript->setToolTip(ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP);
    this->m_uiPtr->statusBar->showMessage(SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING + toQString(this->m_serialPort->portName()));
    this->setWindowTitle(MAIN_WINDOW_TITLE);
}

void MainWindow::begin()
{
    this->show();
    this->centerAndFitWindow();
    this->m_checkPortDisconnectTimer->start();
    this->m_checkSerialPortReceiveTimer->start();
}

void MainWindow::beginCommunication()
{
    using namespace QSerialTerminalUtilities;
    using namespace QSerialTerminalStrings;
    if (this->m_serialPort) {
        try {
            if (!this->m_serialPort->isOpen()) {
                openSerialPort();
                return;
            }
            this->setWindowTitle(this->windowTitle() + " - " + toQString(this->m_serialPort->portName()));
            this->m_uiPtr->statusBar->showMessage(toQString(SUCCESSFULLY_OPENED_SERIAL_PORT_STRING) + toQString(this->m_serialPort->portName()));
            this->m_checkSerialPortReceiveTimer->start();
        } catch (std::exception &e) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(INVALID_SETTINGS_DETECTED_STRING + toQString(e.what()));
            warningBox->setWindowTitle(INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        }
    } else {
        BaudRate baudRate{SerialPort::parseBaudRateFromRaw(this->m_uiPtr->baudRateComboBox->currentText().toStdString())};
        Parity parity{SerialPort::parseParityFromRaw(this->m_uiPtr->parityComboBox->currentText().toStdString())};
        StopBits stopBits{SerialPort::parseStopBitsFromRaw(this->m_uiPtr->stopBitsComboBox->currentText().toStdString())};
        DataBits dataBits{SerialPort::parseDataBitsFromRaw(this->m_uiPtr->dataBitsComboBox->currentText().toStdString())};
        std::string portName{this->m_uiPtr->portNameComboBox->currentText().toStdString()};
        try {
            this->m_serialPort = std::make_shared<SerialPort>(portName, baudRate, dataBits, stopBits, parity);
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
    using namespace QSerialTerminalStrings;
    this->m_checkSerialPortReceiveTimer->stop();
    if (this->m_serialPort) {
        closeSerialPort();
    }
}

void MainWindow::onActionDisconnectTriggered(bool checked)
{
    (void)checked;
    using namespace QSerialTerminalStrings;
    if (this->m_serialPort) {
        closeSerialPort();
        this->m_serialPort.reset();
    }
}

void MainWindow::printRxResult(const std::string &str)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if ((str != "") && (!isWhitespace(str)) && startsWith(str, '{')) {
        this->m_uiPtr->terminal->setTextColor(QColor(RED_COLOR_STRING));
        this->m_uiPtr->terminal->append(toQString(TERMINAL_RECEIVE_BASE_STRING) + toQString(stripLineEndings(stripNonAsciiCharacters(str))));
    }
}

void MainWindow::printTxResult(const std::string &str)
{
    using namespace QSerialTerminalStrings;
    this->m_uiPtr->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_uiPtr->terminal->append(toQString(TERMINAL_TRANSMIT_BASE_STRING) + toQString(str));
}

void MainWindow::printDelayResult(DelayType delayType, int howLong)
{
    using namespace QSerialTerminalStrings;
    this->m_uiPtr->terminal->setTextColor(QColor(GREEN_COLOR_STRING));
    QString stringToAppend{toQString(TERMINAL_DELAY_BASE_STRING) + toQString(howLong)};
    if (delayType == DelayType::SECONDS) {
        stringToAppend.append(SECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MILLISECONDS) {
        stringToAppend.append(MILLISECONDS_SUFFIX_STRING);
    } else if (delayType == DelayType::MICROSECONDS) {
        stringToAppend.append(MICROSECONDS_SUFFIX_STRING);
    }
    this->m_uiPtr->terminal->append(stringToAppend);
}

void MainWindow::printFlushResult(FlushType flushType)
{
    using namespace GeneralUtilities;
    using namespace QSerialTerminalStrings;
    QString stringToAppend{""};

    if (flushType == FlushType::RX) {
        stringToAppend.append(TERMINAL_FLUSH_RX_BASE_STRING);
    } else if (flushType == FlushType::TX) {
        stringToAppend.append(TERMINAL_FLUSH_TX_BASE_STRING);
    } else if (flushType == FlushType::RX_TX) {
        stringToAppend.append(TERMINAL_FLUSH_RX_TX_BASE_STRING);
    }
    this->m_uiPtr->terminal->setTextColor(QColor(GRAY_COLOR_STRING));
    this->m_uiPtr->terminal->append(stringToAppend);
    this->m_uiPtr->terminal->setTextColor(QColor());
}

QApplication *MainWindow::application()
{
    return qApp;
}


void MainWindow::printLoopResult(LoopType loopType, int currentLoop, int loopCount)
{
    using namespace GeneralUtilities;
    using namespace QSerialTerminalStrings;
    QString stringToAppend{""};
    this->m_uiPtr->terminal->setTextColor(QColor(ORANGE_COLOR_STRING));
    if (loopCount == -1) {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(BEGINNING_INFINITE_LOOP_STRING);
            }
            this->m_uiPtr->terminal->append(toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        }
    } else {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(BEGINNING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
            this->m_uiPtr->terminal->append(toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
            if ((currentLoop+1) == loopCount) {
                this->m_uiPtr->terminal->append(toQString(ENDING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
        }
        this->m_uiPtr->terminal->setTextColor(QColor());
    }
}

void MainWindow::staticPrintRxResult(MainWindow *mainWindow, const std::string &str)
{
    if (mainWindow) {
        mainWindow->printRxResult(str);
    }
}

void MainWindow::staticPrintTxResult(MainWindow *mainWindow, const std::string &str)
{
    if (mainWindow) {
        mainWindow->printTxResult(str);
    }
}

void MainWindow::staticPrintDelayResult(MainWindow *mainWindow,  DelayType delayType, int howLong)
{
    if (mainWindow) {
        mainWindow->printDelayResult(delayType, howLong);
    }
}

void MainWindow::staticPrintFlushResult(MainWindow *mainWindow, FlushType flushType)
{
    if (mainWindow) {
        mainWindow->printFlushResult(flushType);
    }
}

void MainWindow::staticPrintLoopResult(MainWindow *mainWindow, LoopType loopType, int currentLoop, int loopCount)
{
    if (mainWindow) {
        mainWindow->printLoopResult(loopType, currentLoop, loopCount);
    }
}


void MainWindow::onActionLoadScriptTriggered(bool checked)
{
    (void)checked;
    using namespace QSerialTerminalStrings;
    this->m_checkSerialPortReceiveTimer->stop();
    this->m_checkPortDisconnectTimer->stop();
    QFile homeCheck{OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY};
    QString defaultScriptFileDirectory{""};
    if (homeCheck.exists()) {
        defaultScriptFileDirectory = OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;
    }
    QFile file{QFileDialog::getOpenFileName(this, OPEN_SCRIPT_FILE_CAPTION, defaultScriptFileDirectory)};
    if (file.fileName() == "") {
        return;
    }
    if (file.exists()) {
        std::unique_ptr<ScriptExecutor> scriptExecutor{std::make_unique<ScriptExecutor>(file.fileName().toStdString())};
        if (scriptExecutor->scriptReader()->commands()->empty()) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(EMPTY_SCRIPT_STRING + file.fileName());
            warningBox->setWindowTitle(EMPTY_SCRIPT_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        } else {
            this->m_uiPtr->terminal->append("");
            this->m_uiPtr->terminal->append(EXECUTING_SCRIPT_STRING + file.fileName());
            scriptExecutor->execute(this,
                                    this->m_serialPort,
                                    this->m_packagedRxResultTask,
                                    this->m_packagedTxResultTask,
                                    this->m_packagedDelayResultTask,
                                    this->m_packagedFlushResultTask,
                                    this->m_packagedLoopResultTask);
            this->m_uiPtr->terminal->append(FINISHED_EXECUTING_SCRIPT_STRING + file.fileName());
        }
    } else {
        std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
        warningBox->setText(FILE_DOES_NOT_EXIST_STRING + file.fileName());
        warningBox->setWindowTitle(FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING);
        warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
        warningBox->exec();
    }
    this->m_checkSerialPortReceiveTimer->start();
    this->m_checkPortDisconnectTimer->start();
}

void MainWindow::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->parentWidget()->setFocus();
            onReturnKeyPressed();
        } else if (qke->key() == Qt::Key_Up) {
            onUpArrowPressed();
        } else if (qke->key() == Qt::Key_Down) {
            onDownArrowPressed();
        } else if (qke->key() == Qt::Key_Escape) {
            onEscapeKeyPressed();
        } else if (qke->key() == Qt::Key_Alt) {
            onAltKeyPressed();
        } else if ((qke->key() == Qt::Key_A) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlAPressed();
        } else if ((qke->key() == Qt::Key_E) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlEPressed();
        } else if ((qke->key() == Qt::Key_U) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlUPressed();
        } else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            onCtrlGPressed();
        } else {
            return QWidget::keyPressEvent(qke);
        }
    }
}


void MainWindow::onReturnKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    onReturnKeyPressed();
}

void MainWindow::onUpArrowPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onUpArrowPressed();
}

void MainWindow::onDownArrowPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onDownArrowPressed();
}

void MainWindow::onEscapeKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;

    onEscapeKeyPressed();
}

void MainWindow::onAltKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;

    onAltKeyPressed();
}

void MainWindow::onCtrlAPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlAPressed();
}

void MainWindow::onCtrlEPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlEPressed();
}

void MainWindow::onCtrlUPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlUPressed();
}

void MainWindow::onCtrlGPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    onCtrlGPressed();
}

void MainWindow::onReturnKeyPressed()
{
    this->m_uiPtr->sendButton->click();
    this->m_uiPtr->sendBox->setFocus();
}

void MainWindow::onUpArrowPressed()
{
    if (this->m_currentHistoryIndex == this->m_commandHistory.size()) {
        this->m_commandHistory.emplace_back( this->m_uiPtr->sendBox->text());
    }
    if (this->m_currentHistoryIndex == 0) {
        this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(this->m_currentHistoryIndex));
    } else {
        this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(--this->m_currentHistoryIndex));
    }
}

void MainWindow::onDownArrowPressed()
{
    if (this->m_currentHistoryIndex != this->m_commandHistory.size()) {
        if ((this->m_currentHistoryIndex + 1) == (this->m_commandHistory.size())) {
            this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(this->m_currentHistoryIndex));
        } else {
            this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(++this->m_currentHistoryIndex));
        }
    }
}

void MainWindow::onEscapeKeyPressed()
{

}

void MainWindow::onAltKeyPressed()
{

}

void MainWindow::onCtrlAPressed()
{
     this->m_uiPtr->sendBox->setCursorPosition(0);
}

void MainWindow::onCtrlEPressed()
{
    this->m_uiPtr->sendBox->setCursorPosition(this->m_uiPtr->sendBox->text().size());
}

void MainWindow::onCtrlUPressed()
{
    this->m_uiPtr->sendBox->clear();
}

void MainWindow::onCtrlGPressed()
{
    this->m_uiPtr->terminal->clear();
}

void MainWindow::onConnectButtonClicked(bool checked)
{
    (void)checked;
    this->onActionConnectTriggered(checked);
}

void MainWindow::onDisconnectButtonClicked(bool checked)
{
    (void)checked;
    this->onActionDisconnectTriggered(checked);
}

void MainWindow::onApplicationAboutToClose()
{
    if (this->m_serialPort && this->m_serialPort->isOpen()) {
        this->m_serialPort->closePort();
    }
}

MainWindow::~MainWindow()
{

}
