#ifndef QSERIALTERMINAL_MAINWINDOW_H
#define QSERIALTERMINAL_MAINWINDOW_H

#include <QMainWindow>

#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include <functional>
#include <list>
#include <future>
#include <memory>
#include <QLabel>
#include <QTimer>

#include "IByteStream.h"
#include "SerialPort.h"
#include "AboutApplicationWidget.h"

class QDesktopWidget;
class QAction;
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
    Q_OBJECT
    enum class SerialPortItemType {
        PORT_NAME,
        BAUD_RATE,
        PARITY,
        DATA_BITS,
        STOP_BITS,
        LINE_ENDING
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override { delete this->m_ui; }
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
    std::shared_ptr<CppSerialPort::IByteStream> m_byteStream;
    std::vector<std::string> m_serialPortNames;
    std::mutex m_printToTerminalMutex;
    std::unique_ptr<std::future<std::string>> m_serialReceiveAsyncHandle;

    bool m_currentLinePushedIntoCommandHistory;
    std::vector<QString> m_commandHistory;
    unsigned int m_currentHistoryIndex;
    std::string m_lineEnding;

    std::vector<QAction *> m_availableBaudRateActions;
    std::vector<QAction *> m_availableParityActions;
    std::vector<QAction *> m_availableStopBitsActions;
    std::vector<QAction *> m_availableDataBitsActions;
    std::vector<QAction *> m_availablePortNamesActions;
    std::vector<QAction *> m_availableLineEndingActions;

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

    static const int s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT;
    static const int s_SERIAL_TIMEOUT;
    static const int s_TASKBAR_HEIGHT;
    static const int s_CHECK_PORT_DISCONNECT_TIMEOUT;
    static const int s_CHECK_PORT_RECEIVE_TIMEOUT;
    static const int s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT;
    static const int s_SCRIPT_INDENT;
    static const int s_SERIAL_READ_TIMEOUT;
    static const int s_STATUS_BAR_FONT_POINT_SIZE;
    static const std::string s_CARRIAGE_RETURN_LINE_ENDING;
    static const std::string s_NEW_LINE_LINE_ENDING;
    static const std::string s_CARRIAGE_RETURN_NEW_LINE_LINE_ENDING;
    static const std::list<std::string> s_AVAILABLE_LINE_ENDINGS;
    static const std::string s_DEFAULT_LINE_ENDING;

    std::string getSerialPortItemFromActions(SerialPortItemType serialPortItemType);
    void addNewSerialPortInfoItem(SerialPortItemType serialPortItemType, const std::string &actionName);
    void removeOldSerialPortInfoItem(SerialPortItemType serialPortItemType, const std::string &actionName);
    void setLineEnding(const std::string &lineEnding);
    void autoSetLineEnding();
    std::string checkSerialReceive();

    static const CppSerialPort::BaudRate s_DEFAULT_BAUD_RATE;
    static const CppSerialPort::Parity s_DEFAULT_PARITY;
    static const CppSerialPort::StopBits s_DEFAULT_STOP_BITS;
    static const CppSerialPort::DataBits s_DEFAULT_DATA_BITS;

    void setStatusBarLabelText(const QString &str);

    void onAboutQSerialTerminalActionTriggered(bool checked);

    void onAboutApplicationWidgetWindowClosed();
};

#endif //QSERIALTERMINAL_MAINWINDOW_H
