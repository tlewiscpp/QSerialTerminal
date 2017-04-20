#include <QApplication>
#include <QDesktopWidget>
#include <memory>
#include <serialport.h>
#include <tstream.h>
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
    mainWindow->begin();
#if defined(__ANDROID__)
    mainWindow->showMaximized();
#endif
        mainWindow->showMaximized();

    return qApplication.exec();
}
