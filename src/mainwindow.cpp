#include "mainwindow.h"
#include "ui_mainwindow.h"

const int MainWindow::s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT{5000};
const int MainWindow::s_SERIAL_TIMEOUT{400};
const int MainWindow::s_TASKBAR_HEIGHT{15};
const int MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT{750};
const int MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT{500};
const int MainWindow::s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT{5000};
const int MainWindow::s_SCRIPT_INDENT{0};
const int MainWindow::s_SERIAL_READ_TIMEOUT{25};

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
    m_currentLinePushedIntoCommandHistory{false},
    m_currentHistoryIndex{0},
    m_cancelScript{false},
    m_xPlacement{0},
    m_yPlacement{0}
{
    using namespace QSerialTerminalStrings;
    this->m_uiPtr->setupUi(this);

    setupAdditionalUiComponents();

    connect(this->m_uiPtr->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    connect(this->m_uiPtr->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onApplicationAboutToClose()));

    connect(this->m_uiPtr->sendBox, SIGNAL(commandHistoryContextMenuRequested(const QPoint &)), this, SLOT(onCommandHistoryContextMenuRequested(const QPoint &)));
    connect(this->m_uiPtr->actionConnect, SIGNAL(triggered(bool)), this, SLOT(onActionConnectTriggered(bool)));
    connect(this->m_uiPtr->connectButton, SIGNAL(clicked(bool)), this, SLOT(onConnectButtonClicked(bool)));
    connect(this->m_uiPtr->actionDisconnect, SIGNAL(triggered(bool)), this, SLOT(onActionDisconnectTriggered(bool)));
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

    connect(this->m_uiPtr->actionLENone, SIGNAL(triggered(bool)), this, SLOT(onActionLENoneTriggered(bool)));
    connect(this->m_uiPtr->actionLECR, SIGNAL(triggered(bool)), this, SLOT(onActionLECRTriggered(bool)));
    connect(this->m_uiPtr->actionLELF, SIGNAL(triggered(bool)), this, SLOT(onActionLELFTriggered(bool)));
    connect(this->m_uiPtr->actionLECRLF, SIGNAL(triggered(bool)), this, SLOT(onActionLECRLFTriggered(bool)));

    connect(this->m_uiPtr->menuAbout, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuBaudRate, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuDataBits, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuFile, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuLineEndings, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuParity, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuPortNames, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));
    connect(this->m_uiPtr->menuStopBits, SIGNAL(aboutToShow()), this, SLOT(onContextMenuActive()));

    connect(this->m_uiPtr->menuAbout, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuBaudRate, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuDataBits, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuFile, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuLineEndings, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuParity, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuPortNames, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));
    connect(this->m_uiPtr->menuStopBits, SIGNAL(aboutToShow()), this, SLOT(onContextMenuInactive()));


    this->m_checkPortDisconnectTimer->setInterval(MainWindow::s_CHECK_PORT_DISCONNECT_TIMEOUT);
    this->m_checkSerialPortReceiveTimer->setInterval(MainWindow::s_CHECK_PORT_RECEIVE_TIMEOUT);
#if defined(_WIN32) || defined(__CYGWIN__)

#else
    connect(this->m_checkPortDisconnectTimer.get(), SIGNAL(timeout()), this, SLOT(checkDisconnectedSerialPorts()));
#endif
    connect(this->m_checkSerialPortReceiveTimer.get(), SIGNAL(timeout()), this, SLOT(checkSerialReceive()));
}

void MainWindow::onContextMenuActive(CustomMenu *customMenu)
{
    using namespace QSerialTerminalStrings;
    if (customMenu) {
        if (customMenu == this->m_uiPtr->menuPortNames) {
            if (customMenu->actions().empty()) {
                customMenu->close();
                std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
                warningBox->setText(NO_AVAILABLE_SERIAL_PORTS_STRING);
                warningBox->setWindowTitle(NO_AVAILABLE_SERIAL_PORTS_WINDOW_TITLE_STRING);
                warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
                warningBox->exec();
            }
        }
    }
}

void MainWindow::onContextMenuInactive(CustomMenu *customMenu)
{
    (void) customMenu;
}

void MainWindow::onActionLENoneTriggered(bool toggled)
{
    (void)toggled;
    doChangeLineEnding(LineEnding::LE_None);
}

void MainWindow::onActionLECRTriggered(bool toggled)
{
    (void)toggled;
    doChangeLineEnding(LineEnding::LE_CarriageReturn);
}

void MainWindow::onActionLELFTriggered(bool toggled)
{
    (void)toggled;
    doChangeLineEnding(LineEnding::LE_LineFeed);
}

void MainWindow::onActionLECRLFTriggered(bool toggled)
{
    (void)toggled;
    doChangeLineEnding(LineEnding::LE_CarriageReturnLineFeed);
}

void MainWindow::doChangeLineEnding(LineEnding newLineEnding)
{
    this->m_lineEnding = newLineEnding;
    if (this->m_lineEnding == LineEnding::LE_None) {
        this->m_uiPtr->actionLENone->setChecked(true);
        this->m_uiPtr->actionLECR->setChecked(false);
        this->m_uiPtr->actionLELF->setChecked(false);
        this->m_uiPtr->actionLECRLF->setChecked(false);
    } else if (this->m_lineEnding == LineEnding::LE_CarriageReturn) {
        this->m_uiPtr->actionLENone->setChecked(false);
        this->m_uiPtr->actionLECR->setChecked(true);
        this->m_uiPtr->actionLELF->setChecked(false);
        this->m_uiPtr->actionLECRLF->setChecked(false);
    } else if (this->m_lineEnding == LineEnding::LE_LineFeed) {
        this->m_uiPtr->actionLENone->setChecked(false);
        this->m_uiPtr->actionLECR->setChecked(false);
        this->m_uiPtr->actionLELF->setChecked(true);
        this->m_uiPtr->actionLECRLF->setChecked(false);
    } else if (this->m_lineEnding == LineEnding::LE_CarriageReturnLineFeed) {
        this->m_uiPtr->actionLENone->setChecked(false);
        this->m_uiPtr->actionLECR->setChecked(false);
        this->m_uiPtr->actionLELF->setChecked(false);
        this->m_uiPtr->actionLECRLF->setChecked(true);
    }
}

std::string MainWindow::getLineEnding(LineEnding lineEnding)
{
    if (lineEnding == LineEnding::LE_None) {
        return "";
    } else if (lineEnding == LineEnding::LE_CarriageReturn) {
        return "\r";
    } else if (lineEnding == LineEnding::LE_LineFeed) {
        return "\n";
    } else if (lineEnding == LineEnding::LE_CarriageReturnLineFeed) {
        return "\r\n";
    } else {
        return "";
    }
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
        if (!this->m_serialPort->isOpen()) {
            this->m_serialPort->openPort();
        }
        if (str.toStdString() != "") {
            this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_uiPtr->sendBox->text());
            resetCommandHistory();
        }
        this->m_serialPort->writeString(str.toStdString() + this->getLineEnding(this->m_lineEnding));
        printTxResult(str.toStdString());
        this->m_uiPtr->sendBox->clear();
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
    using namespace GeneralUtilities;
}

void MainWindow::checkSerialReceive()
{
    using namespace GeneralUtilities;
    if (this->m_serialPort) {
        if (!this->m_serialPort->isOpen()) {
            this->m_serialPort->openPort();
        }
        std::string returnString{stripNonAsciiCharacters(this->m_serialPort->readString())};
        if (returnString != "") {
            appendReceivedString(returnString);
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

void MainWindow::resetCommandHistory()
{
    this->m_currentHistoryIndex = 0;
    this->m_currentLinePushedIntoCommandHistory = false;
    clearEmptyStringsFromCommandHistory();
}

void MainWindow::clearEmptyStringsFromCommandHistory()
{
    using namespace GeneralUtilities;
    while (true) {
        int i{0};
        for (auto &it : this->m_commandHistory) {
            if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
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
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if (this->m_uiPtr->sendButton->text() == toQString(SEND_STRING)) {
        if (this->m_serialPort) {
            appendTransmittedString(toQString(stripLineEndings(this->m_uiPtr->sendBox->text().toStdString())));
        } else {
            if (this->m_availablePortNamesActions.size() != 0) {
                onActionConnectTriggered(false);
                onSendButtonClicked();
            } else {
                this->m_uiPtr->statusBar->showMessage(NO_SERIAL_PORTS_CONNECTED_STRING);
            }
        }
    } else {
        this->m_cancelScript = true;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    this->m_cancelScript = true;
    event->accept();
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

void MainWindow::onCommandHistoryContextMenuRequested(const QPoint &point)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    QMenu commandHistoryContextMenu(COMMAND_HISTORY_CONTEXT_MENU_STRING, this);
    int i{0};
    for (auto &it : this->m_commandHistory) {
        if ((isWhitespace(it.toStdString())) || (it.toStdString() == "")) {
            i++;
            continue;
        }
        CustomAction *tempAction{new CustomAction{it, i++, this}};
        commandHistoryContextMenu.addAction(tempAction);
        connect(tempAction, SIGNAL(triggered(CustomAction *,bool)), this, SLOT(onCommandHistoryContextMenuActionTriggered(CustomAction *, bool)));
    }
    commandHistoryContextMenu.exec(mapToGlobal(point));
}

void MainWindow::onCommandHistoryContextMenuActionTriggered(CustomAction *action, bool checked)
{
    (void)checked;
    if (action) {
        this->m_currentHistoryIndex = action->index();
        this->m_uiPtr->sendBox->setText(action->text());
    }
}

void MainWindow::setupAdditionalUiComponents()
{
    using namespace QSerialTerminalStrings;
    for (auto &it : SerialPort::availableDataBits()) {
        CustomAction *tempAction{new CustomAction{toQString(it), 0, this}};
        tempAction->setCheckable(true);
        if (tempAction->text().toStdString() == SerialPort::DEFAULT_DATA_BITS_STRING) {
            tempAction->setChecked(true);
        }
        connect(tempAction, SIGNAL(triggered(CustomAction*, bool)), this, SLOT(onActionDataBitsChecked(CustomAction*, bool)));
        this->m_availableDataBitsActions.push_back(tempAction);
        this->m_uiPtr->menuDataBits->addAction(tempAction);
    }

    for (auto &it : SerialPort::availableStopBits()) {
        CustomAction*tempAction{new CustomAction{toQString(it), 0, this}};
        tempAction->setCheckable(true);
        if (tempAction->text().toStdString() == SerialPort::DEFAULT_STOP_BITS_STRING) {
            tempAction->setChecked(true);
        }
        connect(tempAction, SIGNAL(triggered(CustomAction*, bool)), this, SLOT(onActionStopBitsChecked(CustomAction*, bool)));
        this->m_availableStopBitsActions.push_back(tempAction);
        this->m_uiPtr->menuStopBits->addAction(tempAction);
    }

    for (auto &it : SerialPort::availableParity()) {
        CustomAction*tempAction{new CustomAction{toQString(it), 0, this}};
        tempAction->setCheckable(true);
        if (tempAction->text().toStdString() == SerialPort::DEFAULT_PARITY_STRING) {
            tempAction->setChecked(true);
        }
        connect(tempAction, SIGNAL(triggered(CustomAction*, bool)), this, SLOT(onActionParityChecked(CustomAction*, bool)));
        this->m_availableParityActions.push_back(tempAction);
        this->m_uiPtr->menuParity->addAction(tempAction);
    }

    for (auto &it : SerialPort::availableBaudRates()) {
        CustomAction*tempAction{new CustomAction{toQString(it), 0, this}};
        tempAction->setCheckable(true);
        if (tempAction->text().toStdString() == SerialPort::DEFAULT_BAUD_RATE_STRING) {
            tempAction->setChecked(true);
        }
        connect(tempAction, SIGNAL(triggered(CustomAction*, bool)), this, SLOT(onActionBaudRateChecked(CustomAction*, bool)));
        this->m_availableBaudRateActions.push_back(tempAction);
        this->m_uiPtr->menuBaudRate->addAction(tempAction);
    }

    for (auto &it : SerialPort::availableSerialPorts()) {
        CustomAction *tempAction{new CustomAction{toQString(it), 0, this}};
        tempAction->setCheckable(true);
        connect(tempAction, SIGNAL(triggered(CustomAction*, bool)), this, SLOT(onActionPortNamesChecked(CustomAction*, bool)));
        this->m_availablePortNamesActions.push_back(tempAction);
        this->m_uiPtr->menuPortNames->addAction(tempAction);
    }

    this->m_uiPtr->sendBox->setEnabled(false);
    this->m_uiPtr->sendBox->setToolTip(SEND_BOX_DISABLED_TOOLTIP);
    this->m_uiPtr->actionLoadScript->setEnabled(false);
    this->m_uiPtr->actionLoadScript->setToolTip(ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP);

    this->m_uiPtr->actionDisconnect->setEnabled(false);
    this->m_uiPtr->sendBox->setTabOrder(this->m_uiPtr->sendBox, this->m_uiPtr->sendButton);

    this->m_uiPtr->statusBar->showMessage(CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING);
}

void MainWindow::onActionBaudRateChecked(CustomAction *action, bool checked)
{
    if ((action->isChecked()) || (checked)) {
        return;
    }
    for (auto &it : this->m_availableBaudRateActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionParityChecked(CustomAction *action, bool checked)
{
    if ((action->isChecked()) || (checked)) {
        return;
    }
    for (auto &it : this->m_availableParityActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionDataBitsChecked(CustomAction *action, bool checked)
{
    if ((action->isChecked()) || (checked)) {
        return;
    }
    for (auto &it : this->m_availableDataBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionStopBitsChecked(CustomAction *action, bool checked)
{
    if ((action->isChecked()) || (checked)) {
        return;
    }
    for (auto &it : this->m_availableStopBitsActions) {
        if (it == action) {
            action->setChecked(true);
        } else {
            it->setChecked(false);
        }
    }
}

void MainWindow::onActionPortNamesChecked(CustomAction *action, bool checked)
{
    if ((action->isChecked()) || (checked)) {
        return;
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
    (void)checked;
    using namespace QSerialTerminalStrings;
    this->pauseCommunication();

    BaudRate baudRate{SerialPort::parseBaudRateFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::BAUD_RATE))};
    Parity parity{SerialPort::parseParityFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::PARITY))};
    StopBits stopBits{SerialPort::parseStopBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::STOP_BITS))};
    DataBits dataBits{SerialPort::parseDataBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::DATA_BITS))};
    std::string portName{this->getSerialPortItemFromActions(SerialPortItemType::PORT_NAME)};
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
    this->m_uiPtr->actionDisconnect->setEnabled(true);
    this->m_uiPtr->sendBox->setEnabled(true);
    this->m_uiPtr->sendBox->setToolTip(SEND_BOX_ENABLED_TOOLTIP);
    this->m_uiPtr->actionLoadScript->setEnabled(true);
    this->m_uiPtr->actionLoadScript->setToolTip(ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP);
    this->m_uiPtr->sendBox->setFocus();
    this->m_uiPtr->statusBar->showMessage(SUCCESSFULLY_OPENED_SERIAL_PORT_STRING + toQString(this->m_serialPort->portName()));
    this->m_serialPort->setTimeout(MainWindow::s_SERIAL_READ_TIMEOUT);
    beginCommunication();
}

void MainWindow::closeSerialPort()
{
    using namespace QSerialTerminalStrings;
    this->m_serialPort->closePort();
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
    using namespace GeneralUtilities;
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
        BaudRate baudRate{SerialPort::parseBaudRateFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::BAUD_RATE))};
        Parity parity{SerialPort::parseParityFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::PARITY))};
        StopBits stopBits{SerialPort::parseStopBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::STOP_BITS))};
        DataBits dataBits{SerialPort::parseDataBitsFromRaw(this->getSerialPortItemFromActions(SerialPortItemType::DATA_BITS))};
        std::string portName{this->getSerialPortItemFromActions(SerialPortItemType::PORT_NAME)};
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
        this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(TERMINAL_RECEIVE_BASE_STRING) + toQString(stripLineEndings(stripNonAsciiCharacters(str))));
    }
}

void MainWindow::printTxResult(const std::string &str)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(BLUE_COLOR_STRING));
    this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(TERMINAL_TRANSMIT_BASE_STRING) + toQString(str));
}

void MainWindow::printDelayResult(DelayType delayType, int howLong)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    this->m_uiPtr->terminal->setTextColor(QColor(GREEN_COLOR_STRING));
    QString stringToAppend{toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(TERMINAL_DELAY_BASE_STRING) + toQString(howLong)};
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
    this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + stringToAppend);
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
                this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + BEGINNING_INFINITE_LOOP_STRING);
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString(INFINITE_LOOP_COUNT_TAIL_STRING));
        }
    } else {
        if (loopType == LoopType::START) {
            if (currentLoop == 0) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(BEGINNING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
            }
            this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(BEGIN_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
        } else if (loopType == LoopType::END) {
            this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(END_LOOP_BASE_STRING) + toQString(currentLoop + 1) + toQString("/") + toQString(loopCount) + toQString(")"));
            if ((currentLoop+1) == loopCount) {
                this->m_uiPtr->terminal->append(toQString(tWhitespace(MainWindow::s_SCRIPT_INDENT)) + toQString(ENDING_LOOPS_BASE_STRING) + toQString(loopCount) + toQString(LOOPS_TAIL_STRING));
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
        std::unique_ptr<TScriptExecutor> scriptExecutor{std::make_unique<TScriptExecutor>(file.fileName().toStdString())};
        if (scriptExecutor->hasCommands()) {
            std::unique_ptr<QMessageBox> warningBox{std::make_unique<QMessageBox>()};
            warningBox->setText(EMPTY_SCRIPT_STRING + file.fileName());
            warningBox->setWindowTitle(EMPTY_SCRIPT_WINDOW_TITLE_STRING);
            warningBox->setWindowIcon(this->m_qstiPtr->MAIN_WINDOW_ICON);
            warningBox->exec();
        } else {
            this->m_uiPtr->terminal->append("");
            this->m_uiPtr->terminal->append(EXECUTING_SCRIPT_STRING + file.fileName());
            this->m_uiPtr->sendBox->setEnabled(false);
            this->m_uiPtr->sendButton->setText(CANCEL_SCRIPT_STRING);
            scriptExecutor->execute(this,
                                    this->m_serialPort,
                                    this->m_packagedRxResultTask,
                                    this->m_packagedTxResultTask,
                                    this->m_packagedDelayResultTask,
                                    this->m_packagedFlushResultTask,
                                    this->m_packagedLoopResultTask);
           this->m_uiPtr->terminal->setTextColor(QColor());
            if (this->m_cancelScript) {
                this->m_uiPtr->terminal->append(CANCELED_EXECUTING_SCRIPT_STRING + file.fileName());
                this->m_cancelScript = false;
            } else {
                this->m_uiPtr->terminal->append(FINISHED_EXECUTING_SCRIPT_STRING + file.fileName());
            }
            this->m_serialPort->flushRXTX();
            printFlushResult(FlushType::RX_TX);
            this->m_uiPtr->sendBox->setEnabled(false);
            this->m_uiPtr->sendBox->setFocus();
            this->m_uiPtr->sendButton->setText(SEND_STRING);
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

void MainWindow::processEvents()
{
    this->application()->processEvents();
}

bool MainWindow::cancelScript() const
{
    return this->m_cancelScript;
}

void MainWindow::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            this->m_uiPtr->sendBox->clearFocus();
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


void MainWindow::onReturnKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onReturnKeyPressed();
}

void MainWindow::onUpArrowPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onUpArrowPressed();
}

void MainWindow::onDownArrowPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onDownArrowPressed();
}

void MainWindow::onEscapeKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onEscapeKeyPressed();
}

void MainWindow::onAltKeyPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onAltKeyPressed();
}

void MainWindow::onCtrlAPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    this->onCtrlAPressed();
}

void MainWindow::onCtrlEPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    this->onCtrlEPressed();
}

void MainWindow::onCtrlUPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    this->onCtrlUPressed();
}

void MainWindow::onCtrlGPressed(SerialTerminalLineEdit *stle)
{

    (void)stle;
    this->onCtrlGPressed();
}

void MainWindow::onCtrlCPressed(SerialTerminalLineEdit *stle)
{
    (void)stle;
    this->onCtrlCPressed();
}

void MainWindow::onReturnKeyPressed()
{
    using namespace GeneralUtilities;
    this->m_uiPtr->sendButton->click();
}

void MainWindow::onUpArrowPressed()
{
    if (this->m_currentHistoryIndex >= this->m_commandHistory.size()-1) {
        return;
    }
    if (this->m_currentHistoryIndex == 0) {
        if (!this->m_currentLinePushedIntoCommandHistory) {
            this->m_commandHistory.insert(this->m_commandHistory.begin(), this->m_uiPtr->sendBox->text());
            this->m_currentLinePushedIntoCommandHistory = true;
        }
    }
    this->m_currentHistoryIndex++;
    this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(this->m_currentHistoryIndex));
}

void MainWindow::onDownArrowPressed()
{
    if (this->m_currentHistoryIndex == 0) {
        return;
    }
    this->m_uiPtr->sendBox->setText(this->m_commandHistory.at(--this->m_currentHistoryIndex));
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

void MainWindow::onCtrlCPressed()
{
    using namespace QSerialTerminalStrings;
    if (this->m_uiPtr->sendButton->text() == toQString(CANCEL_SCRIPT_STRING)) {
        this->m_cancelScript = true;
    }
}

void MainWindow::onConnectButtonClicked(bool checked)
{
    if ((this->m_uiPtr->connectButton->isChecked()) || (checked)) {
        this->onActionDisconnectTriggered(checked);
    } else {
        this->onActionConnectTriggered(checked);
    }
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
