#ifndef SERIALTERMINAL_MAINWINDOW_H
#define SERIALTERMINAL_MAINWINDOW_H

#include <QMainWindow>
#include <QString>
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

    std::shared_ptr<QSerialTerminalIcons> strPtr() const;
    void bindQSerialTerminalIcons(std::shared_ptr<QSerialTerminalIcons> qstiPtr);
    void bindQDesktopWidget(std::shared_ptr<QDesktopWidget> qDesktopWidget);
    void centerAndFitWindow();
    int xPlacement() const;
    int yPlacement() const;
    void begin();


private slots:
    void onCustomDialogClosed();
    void onSduiOkayButtonClicked();
    void onSduiCancelButtonClicked();
    void onSendButtonClicked();
    void onActionPortSettingsTriggered();
    void checkSerialReceive();
    void checkDisconnectedSerialPorts();
    void onStatusBarMessageChanged(QString newMessage);
    void onPortNameComboBoxIndexChanged(QString newIndex);
    void onEnterKeyPressed();

private:
    std::shared_ptr<Ui::MainWindow> m_uiPtr;
    std::shared_ptr<Ui::SettingsDialog> m_sduiPtr;
    std::shared_ptr<CustomDialog>  m_portSettingsDialog;
    std::unique_ptr<QTimer> m_checkPortDisconnectTimer;
    std::unique_ptr<QTimer> m_checkSerialPortReceiveTimer;
    std::unique_ptr<EventTimer> m_serialReceiveTimer;
    std::shared_ptr<SerialPort> m_serialPort;
    std::shared_ptr<QSerialTerminalIcons> m_qstiPtr;
    std::shared_ptr<QDesktopWidget> m_qDesktopWidget;
    std::vector<std::string> m_serialPortNames;
    std::string m_temporarySerialPortBuffer;
    int m_xPlacement;
    int m_yPlacement;

    void setupSettingsDialog();
    void beginCommunication();
    void pauseCommunication();
    void stopCommunication();
    void calculateXYPlacement();

    static const int s_SUCCESSFULLY_OPENED_SERIAL_PORT_MESSAGE_TIMEOUT;
    static const int s_SERIAL_TIMEOUT;
    static const int s_TASKBAR_HEIGHT;
    static const int s_CHECK_PORT_DISCONNECT_TIMEOUT;
    static const int s_CHECK_PORT_RECEIVE_TIMEOUT;
};

#endif //SERIALTERMINAL_MAINWINDOW_H
