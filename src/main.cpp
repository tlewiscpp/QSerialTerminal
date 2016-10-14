#include <QApplication>
#include <QDesktopWidget>
#include <memory>
#include <serialport.h>
#include <generalutilities.h>
#include "qserialterminalicons.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    using namespace QSerialTerminalStrings;
    QApplication qApplication(argc, argv);
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    std::shared_ptr<QSerialTerminalIcons> qstiPtr{std::make_shared<QSerialTerminalIcons>()};
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(qDesktopWidget, qstiPtr)};
    mainWindow->setWindowIcon(qstiPtr->MAIN_WINDOW_ICON);
    mainWindow->setWindowTitle(MAIN_WINDOW_TITLE);
    mainWindow->setStyleSheet(MAIN_WINDOW_STYLESHEET);
    std::shared_ptr<SerialPort> serialPort{std::make_shared<SerialPort>("/dev/ttyACM0",
                                                                        BaudRate::BAUD115200,
                                                                        DataBits::EIGHT,
                                                                        StopBits::ONE,
                                                                        Parity::NONE)};

    using namespace GeneralUtilities;
    try {
        serialPort->openPort();
    } catch(std::exception &e) {
        std::cout << "Error opening serial port" << std::endl;
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        serialPort->writeString("{ardtype}");
        delayMilliseconds(50);
        std::cout << serialPort->readString() << std::endl;
        //delayMilliseconds(500);
    }
    serialPort->closePort();

    //mainWindow->centerAndFitWindow();
    //mainWindow->show();
    mainWindow->begin();

    return qApplication.exec();
}
