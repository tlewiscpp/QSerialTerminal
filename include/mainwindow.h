#ifndef SERIALTERMINAL_MAINWINDOW_H
#define SERIALTERMINAL_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QColor>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QTimer>
#include <vector>
#include <string>
#include <chrono>
#include <memory>
#include <serialport.h>
#include <systemcommand.h>
#include <generalutilities.h>
#include <eventtimer.h>

#include "customdialog.h"
#include "eventtimer.h"
#include "serialterminallineedit.h"
#include "qserialterminalstrings.h"
#include "qserialterminalicons.h"
#include "qserialterminalutilities.h"

namespace Ui
{
    class MainWindow;
    class SettingsDialog;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

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


private slots:
    void checkSerialReceive();
    void checkDisconnectedSerialPorts();
    void onStatusBarMessageChanged(QString newMessage);
    void onActionConnectTriggered();
    void onActionDisconnectTriggered();

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
    void onReturnKeyPressed();
    void onUpArrowPressed();
    void onDownArrowPressed();
    void onEscapeKeyPressed();
    void onAltKeyPressed();
    void onCtrlAPressed();
    void onCtrlEPressed();
    void onCtrlUPressed();
    void onCtrlGPressed();

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
    std::vector<QString> m_commandHistory;
    unsigned int m_currentHistoryIndex;
    int m_xPlacement;
    int m_yPlacement;

    void beginCommunication();
    void pauseCommunication();
    void stopCommunication();
    void calculateXYPlacement();
    void setupSettingsDialog();
    void appendReceivedString(const std::string &str);
    void appendTransmittedString(const QString &str);

    static const int s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT;
    static const int s_SERIAL_TIMEOUT;
    static const int s_TASKBAR_HEIGHT;
    static const int s_CHECK_PORT_DISCONNECT_TIMEOUT;
    static const int s_CHECK_PORT_RECEIVE_TIMEOUT;
    static const int s_NO_SERIAL_PORTS_CONNECTED_MESSAGE_TIMEOUT;
};

#endif //SERIALTERMINAL_MAINWINDOW_H
