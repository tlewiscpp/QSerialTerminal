#include <QApplication>
#include <QDesktopWidget>
#include <memory>

#include "qserialterminalicons.h"
#include "qserialterminalstrings.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    using namespace QSerialTerminalStrings;
    QApplication qApplication(argc, argv);
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    std::shared_ptr<QSerialTerminalIcons> programIcons{std::make_shared<QSerialTerminalIcons>()};
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(qDesktopWidget, qstiPtr)};
    mainWindow->setWindowIcon(programIcons->MAIN_WINDOW_ICON);
    mainWindow->setWindowTitle(MAIN_WINDOW_TITLE);
    mainWindow->setStyleSheet(MAIN_WINDOW_STYLESHEET);
    mainWindow->begin();
#if defined(__ANDROID__)
    mainWindow->showMaximized();
    system("su");
#endif
        mainWindow->showMaximized();

    return qApplication.exec();
}
