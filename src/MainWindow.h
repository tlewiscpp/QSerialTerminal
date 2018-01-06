#ifndef QSERIALTERMINAL_MAINWINDOW_H
#define QSERIALTERMINAL_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QAction>

#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <list>
#include <future>
#include <memory>
#include <unordered_set>

#include "AboutApplicationWidget.h"
#include "QActionSetDefs.h"
#include <functional>

#include "SerialPort.h"

class QDesktopWidget;
class QCloseEvent;
class QSerialTerminalLineEdit;
class ApplicationIcons;
class QString;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    using QActionSet = std::unordered_set<QAction *>;

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent *event) override;

    void keyPressEvent(QKeyEvent *qke) override;
private slots:
    void launchSerialReceiveAsync();
    void checkDisconnectedSerialPorts();
    void onActionConnectTriggered(bool checked);
    void onActionDisconnectTriggered(bool checked);
    void onCommandHistoryContextMenuRequested(const QPoint &point);
    void onCommandHistoryContextMenuActionTriggered(bool checked);


    void onActionBaudRateChecked(bool checked);
    void onActionParityChecked(bool checked);
    void onActionDataBitsChecked(bool checked);
    void onActionStopBitsChecked(bool checked);
    void onActionPortNamesChecked(bool checked);
    void onActionLineEndingsChecked(bool checked);
    void onActionFlowControlChecked(bool checked);

    void onSendButtonClicked();
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
    bool eventFilter(QObject *sender, QEvent *event) override;

private:
    Ui::MainWindow *m_ui;
    std::shared_ptr<AboutApplicationWidget> m_aboutApplicationWidget;
    std::unique_ptr<QLabel> m_statusBarLabel;
    std::unique_ptr<QTimer> m_checkPortDisconnectTimer;
    std::unique_ptr<QTimer> m_checkSerialPortReceiveTimer;
    std::shared_ptr<CppSerialPort::SerialPort> m_byteStream;
    std::unordered_set<std::string> m_serialPortNames;
    std::mutex m_printToTerminalMutex;
    std::unique_ptr<std::future<std::string>> m_serialReceiveAsyncHandle;

    bool m_currentLinePushedIntoCommandHistory;
    std::vector<QString> m_commandHistory;
    unsigned int m_currentHistoryIndex;
    std::string m_lineEnding;

    QActionSet m_availableBaudRateActions;
    QActionSet m_availableParityActions;
    QActionSet m_availableStopBitsActions;
    QActionSet m_availableDataBitsActions;
    QActionSet m_availableFlowControlActions;
    QActionSet m_availablePortNamesActions;
    QActionSet m_availableLineEndingActions;

    void resetCommandHistory();
    void clearEmptyStringsFromCommandHistory();
    void openSerialPort();
    void closeSerialPort();
    void beginCommunication();
    void pauseCommunication();
    void stopCommunication();
    void setupAdditionalUiComponents();
    void appendReceivedString(const std::string &str);
    void appendTransmittedString(const QString &str);

    void printRxResult(const std::string &str);
    void printTxResult(const std::string &str);

    static const int CHECK_PORT_DISCONNECT_TIMEOUT;
    static const int CHECK_PORT_RECEIVE_TIMEOUT;
    static const int NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT;
    static const int SERIAL_READ_TIMEOUT;
    static const int STATUS_BAR_FONT_POINT_SIZE;
    static const char *CARRIAGE_RETURN_LINE_ENDING;
    static const char *NEW_LINE_LINE_ENDING;
    static const char *CARRIAGE_RETURN_NEW_LINE_LINE_ENDING;
    static const std::list<std::string> AVAILABLE_LINE_ENDINGS;
    static const std::string DEFAULT_LINE_ENDING;

    void addNewPortNameItem(const std::string &str);
    void addNewBaudRateItem(CppSerialPort::BaudRate baudRate);
    void addNewStopBitsItem(CppSerialPort::StopBits stopBits);
    void addNewDataBitsItem(CppSerialPort::DataBits dataBits);
    void addNewParityItem(CppSerialPort::Parity parity);
    void addNewFlowControlItem(CppSerialPort::FlowControl flowControl);
    void removeOldPortNameItem(const std::string &str);
    void removeOldBaudRateItem(CppSerialPort::BaudRate baudRate);
    void removeOldStopBitsItem(CppSerialPort::StopBits stopBits);
    void removeOldDataBitsItem(CppSerialPort::DataBits dataBits);
    void removeOldParityItem(CppSerialPort::Parity parity);
    void removeOldFlowControlItem(CppSerialPort::FlowControl flowControl);
    void setLineEnding(const std::string &lineEnding);
    void autoSetLineEnding();
    std::string checkSerialReceive();

    static const CppSerialPort::BaudRate DEFAULT_BAUD_RATE;
    static const CppSerialPort::Parity DEFAULT_PARITY;
    static const CppSerialPort::StopBits DEFAULT_STOP_BITS;
    static const CppSerialPort::DataBits DEFAULT_DATA_BITS;
    static const CppSerialPort::FlowControl DEFAULT_FLOW_CONTROL;

    void setStatusBarLabelText(const QString &str);

    void onAboutQSerialTerminalActionTriggered(bool checked);

    void onAboutApplicationWidgetWindowClosed();

    void addNewLineEndingItem(const std::string &lineEnding);

    CppSerialPort::BaudRate getSelectedBaudRate();
    CppSerialPort::StopBits getSelectedStopBits();
    CppSerialPort::FlowControl getSelectedFlowControl();
    CppSerialPort::DataBits getSelectedDataBits();
    std::string getSelectedPortName();
	CppSerialPort::Parity getSelectedParity();
    static std::unordered_set<QAction *>::iterator findInQActionSet(QActionSet *qActionSet, const QString &key);

    void setBaudRate(QAction *action);
    void setParity(QAction *action);
    void setStopBits(QAction *action);
    void setLineEnding(QAction *action);
    void setFlowControl(QAction *action);
    void setPortName(QAction *action);
    void setDataBits(QAction *action);

    std::string escapeLineEnding(const std::string &lineEnding);
};

#endif //QSERIALTERMINAL_MAINWINDOW_H
