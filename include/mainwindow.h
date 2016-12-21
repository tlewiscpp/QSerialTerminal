#ifndef QSERIALTERMINAL_MAINWINDOW_H
#define QSERIALTERMINAL_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <vector>
#include <list>
#include <string>
#include <chrono>
#include <functional>
#include <memory>
#include <serialport.h>
#include <systemcommand.h>
#include <generalutilities.h>
#include <eventtimer.h>
#include <tscriptreader.h>
#include <tscriptexecutor.h>
#include <tstream.h>

#include "customaction.h"
#include "custommenu.h"
#include "serialterminallineedit.h"
#include "qserialterminalstrings.h"
#include "qserialterminalicons.h"

namespace Ui
{
    class MainWindow;
    class SettingsDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class LineEnding {
        LE_None,
        LE_CarriageReturn,
        LE_LineFeed,
        LE_CarriageReturnLineFeed
    };

    enum class SerialPortItemType {
        PORT_NAME,
        BAUD_RATE,
        PARITY,
        DATA_BITS,
        STOP_BITS
    };

public:
    explicit MainWindow(std::shared_ptr<QDesktopWidget> qDesktopWidget,
                        std::shared_ptr<QSerialTerminalIcons> qstiPtr,
                        QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(std::to_string(convert)); }
    QString toQString(const std::string &str) { return QString::fromStdString(str); }
    QString toQString(const char *str) { return QString::fromStdString(static_cast<std::string>(str)); }
    QString toQString(const QString &qstr) { return qstr; }

    std::shared_ptr<QSerialTerminalIcons> strPtr() const;
    void bindQSerialTerminalIcons(std::shared_ptr<QSerialTerminalIcons> qstiPtr);
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;
    void begin();
    void keyPressEvent(QKeyEvent *qke);
    QApplication *application();
    bool cancelScript() const;
    void processEvents();
private slots:
    void checkSerialReceive();
    void checkDisconnectedSerialPorts();
    void onActionConnectTriggered(bool checked);
    void onActionDisconnectTriggered(bool checked);
    void onActionLoadScriptTriggered(bool checked);
    void onCommandHistoryContextMenuRequested(const QPoint &point);
    void onCommandHistoryContextMenuActionTriggered(CustomAction *action, bool checked);

    void onContextMenuActive(CustomMenu *);
    void onContextMenuInactive(CustomMenu *);

    void onActionBaudRateChecked(CustomAction *action, bool checked);
    void onActionParityChecked(CustomAction *action, bool checked);
    void onActionDataBitsChecked(CustomAction *action, bool checked);
    void onActionStopBitsChecked(CustomAction *action, bool checked);
    void onActionPortNamesChecked(CustomAction *action, bool checked);


    void onActionLENoneTriggered(bool checked);
    void onActionLECRTriggered(bool checked);
    void onActionLELFTriggered(bool checked);
    void onActionLECRLFTriggered(bool checked);

    void onSendButtonClicked();
    void onReturnKeyPressed(SerialTerminalLineEdit *stle);
    void onUpArrowPressed(SerialTerminalLineEdit *stle);
    void onDownArrowPressed(SerialTerminalLineEdit *stle);
    void onEscapeKeyPressed(SerialTerminalLineEdit *stle);
    void onAltKeyPressed(SerialTerminalLineEdit *stle);
    void onCtrlAPressed(SerialTerminalLineEdit *stle);
    void onCtrlEPressed(SerialTerminalLineEdit *stle);
    void onCtrlUPressed(SerialTerminalLineEdit *stle);
    void onCtrlGPressed(SerialTerminalLineEdit *stle);
    void onCtrlCPressed(SerialTerminalLineEdit *stle);
    void onReturnKeyPressed();
    void onUpArrowPressed();
    void onDownArrowPressed();
    void onEscapeKeyPressed();
    void onAltKeyPressed();
    void onCtrlAPressed();
    void onCtrlEPressed();
    void onCtrlUPressed();
    void onCtrlGPressed();
    void onCtrlCPressed();

    void onApplicationAboutToClose();
    void onConnectButtonClicked(bool checked);
    void onDisconnectButtonClicked(bool checked);

private:
    std::shared_ptr<Ui::MainWindow> m_uiPtr;
    std::unique_ptr<QTimer> m_checkPortDisconnectTimer;
    std::unique_ptr<QTimer> m_checkSerialPortReceiveTimer;
    std::shared_ptr<SerialPort> m_serialPort;
    std::shared_ptr<QSerialTerminalIcons> m_qstiPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::vector<std::string> m_serialPortNames;
    std::function<void(MainWindow*, const std::string &)> m_packagedRxResultTask;
    std::function<void(MainWindow*, const std::string &)> m_packagedTxResultTask;
    std::function<void(MainWindow*, DelayType, int)> m_packagedDelayResultTask;
    std::function<void(MainWindow*, FlushType)> m_packagedFlushResultTask;
    std::function<void(MainWindow*, LoopType, int, int)> m_packagedLoopResultTask;
    bool m_currentLinePushedIntoCommandHistory;
    std::vector<QString> m_commandHistory;
    unsigned int m_currentHistoryIndex;
    bool m_cancelScript;
    LineEnding m_lineEnding;
    int m_xPlacement;
    int m_yPlacement;
    std::vector<CustomAction *> m_availableBaudRateActions;
    std::vector<CustomAction *> m_availableParityActions;
    std::vector<CustomAction *> m_availableStopBitsActions;
    std::vector<CustomAction *> m_availableDataBitsActions;
    std::vector<CustomAction *> m_availablePortNamesActions;

    void resetCommandHistory();
    void clearEmptyStringsFromCommandHistory();
    void openSerialPort();
    void closeSerialPort();
    void beginCommunication();
    void pauseCommunication();
    void stopCommunication();
    void calculateXYPlacement();
    void setupAdditionalUiComponents();
    std::string getLineEnding(LineEnding lineEnding);
    void appendReceivedString(const std::string &str);
    void appendTransmittedString(const QString &str);
    void doChangeLineEnding(LineEnding newLineEnding);

    static void staticPrintRxResult(MainWindow *mainWindow, const std::string &str);
    static void staticPrintTxResult(MainWindow *mainWindow, const std::string &str);
    static void staticPrintDelayResult(MainWindow *mainWindow, DelayType delayType, int howLong);
    static void staticPrintFlushResult(MainWindow *mainWindow, FlushType flushType);
    static void staticPrintLoopResult(MainWindow *mainWindow, LoopType loopType, int currentLoop, int loopCount);

    void printRxResult(const std::string &str);
    void printTxResult(const std::string &str);
    void printDelayResult(DelayType delayType, int howLong);
    void printFlushResult(FlushType flushType);
    void printLoopResult(LoopType loopType, int currentLoop, int loopCount);

    static const int s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT;
    static const int s_SERIAL_TIMEOUT;
    static const int s_TASKBAR_HEIGHT;
    static const int s_CHECK_PORT_DISCONNECT_TIMEOUT;
    static const int s_CHECK_PORT_RECEIVE_TIMEOUT;
    static const int s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT;
    static const int s_SCRIPT_INDENT;
    static const int s_SERIAL_READ_TIMEOUT;
    std::string getSerialPortItemFromActions(SerialPortItemType serialPortItemType);
};

#endif //QSERIALTERMINAL_MAINWINDOW_H
