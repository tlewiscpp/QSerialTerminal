/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "src/QSerialTerminalLineEdit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionQuit;
    QAction *actionAboutQt;
    QAction *actionLoadScript;
    QAction *actionLENone;
    QAction *actionLECR;
    QAction *actionLELF;
    QAction *actionLECRLF;
    QAction *actionAboutQSerialTerminal;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QFrame *frame;
    QGridLayout *mainFrame;
    QSerialTerminalLineEdit *sendBox;
    QPushButton *sendButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *scrollAreaGridLayout;
    QTextBrowser *terminal;
    QPushButton *connectButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuAbout;
    QMenu *menuLineEndings;
    QMenu *menuBaudRate;
    QMenu *menuParity;
    QMenu *menuStopBits;
    QMenu *menuDataBits;
    QMenu *menuPortNames;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(877, 561);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(215, 100));
        QFont font;
        font.setPointSize(14);
        MainWindow->setFont(font);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        actionConnect->setCheckable(true);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QFont font1;
        font1.setPointSize(11);
        actionQuit->setFont(font1);
        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName(QStringLiteral("actionAboutQt"));
        actionAboutQt->setFont(font);
        actionLoadScript = new QAction(MainWindow);
        actionLoadScript->setObjectName(QStringLiteral("actionLoadScript"));
        actionLENone = new QAction(MainWindow);
        actionLENone->setObjectName(QStringLiteral("actionLENone"));
        actionLENone->setCheckable(true);
        actionLENone->setChecked(true);
        actionLECR = new QAction(MainWindow);
        actionLECR->setObjectName(QStringLiteral("actionLECR"));
        actionLECR->setCheckable(true);
        actionLELF = new QAction(MainWindow);
        actionLELF->setObjectName(QStringLiteral("actionLELF"));
        actionLELF->setCheckable(true);
        actionLECRLF = new QAction(MainWindow);
        actionLECRLF->setObjectName(QStringLiteral("actionLECRLF"));
        actionLECRLF->setCheckable(true);
        actionAboutQSerialTerminal = new QAction(MainWindow);
        actionAboutQSerialTerminal->setObjectName(QStringLiteral("actionAboutQSerialTerminal"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(5);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(7, 7, 7, 7);
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        mainFrame = new QGridLayout(frame);
        mainFrame->setSpacing(6);
        mainFrame->setContentsMargins(11, 11, 11, 11);
        mainFrame->setObjectName(QStringLiteral("mainFrame"));
        mainFrame->setContentsMargins(7, 7, 7, 5);
        sendBox = new QSerialTerminalLineEdit(frame);
        sendBox->setObjectName(QStringLiteral("sendBox"));
        sendBox->setMinimumSize(QSize(0, 25));
        QFont font2;
        font2.setPointSize(10);
        sendBox->setFont(font2);
        sendBox->setAutoFillBackground(false);
        sendBox->setStyleSheet(QStringLiteral(""));

        mainFrame->addWidget(sendBox, 0, 0, 1, 1);

        sendButton = new QPushButton(frame);
        sendButton->setObjectName(QStringLiteral("sendButton"));

        mainFrame->addWidget(sendButton, 0, 1, 1, 1);

        scrollArea = new QScrollArea(frame);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollArea->setAlignment(Qt::AlignCenter);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 845, 432));
        scrollAreaGridLayout = new QGridLayout(scrollAreaWidgetContents);
        scrollAreaGridLayout->setSpacing(0);
        scrollAreaGridLayout->setContentsMargins(11, 11, 11, 11);
        scrollAreaGridLayout->setObjectName(QStringLiteral("scrollAreaGridLayout"));
        scrollAreaGridLayout->setContentsMargins(0, 0, 0, 0);
        terminal = new QTextBrowser(scrollAreaWidgetContents);
        terminal->setObjectName(QStringLiteral("terminal"));
        terminal->setMinimumSize(QSize(0, 0));
        QFont font3;
        font3.setFamily(QStringLiteral("DejaVu Sans Mono"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        terminal->setFont(font3);
        terminal->setAutoFillBackground(true);
        terminal->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));

        scrollAreaGridLayout->addWidget(terminal, 0, 0, 1, 1);

        connectButton = new QPushButton(scrollAreaWidgetContents);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setMinimumSize(QSize(0, 30));
        connectButton->setMaximumSize(QSize(16777215, 16777215));
        connectButton->setAutoFillBackground(false);
        connectButton->setCheckable(true);
        connectButton->setAutoDefault(true);

        scrollAreaGridLayout->addWidget(connectButton, 1, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        mainFrame->addWidget(scrollArea, 1, 0, 1, 2);


        gridLayout->addWidget(frame, 0, 0, 1, 2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 877, 30));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile->setFont(font);
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuLineEndings = new QMenu(menuBar);
        menuLineEndings->setObjectName(QStringLiteral("menuLineEndings"));
        menuLineEndings->setFont(font);
        menuBaudRate = new QMenu(menuBar);
        menuBaudRate->setObjectName(QStringLiteral("menuBaudRate"));
        menuBaudRate->setFont(font);
        menuParity = new QMenu(menuBar);
        menuParity->setObjectName(QStringLiteral("menuParity"));
        menuParity->setFont(font);
        menuStopBits = new QMenu(menuBar);
        menuStopBits->setObjectName(QStringLiteral("menuStopBits"));
        menuStopBits->setFont(font);
        menuDataBits = new QMenu(menuBar);
        menuDataBits->setObjectName(QStringLiteral("menuDataBits"));
        menuDataBits->setFont(font);
        menuPortNames = new QMenu(menuBar);
        menuPortNames->setObjectName(QStringLiteral("menuPortNames"));
        menuPortNames->setFont(font);
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuPortNames->menuAction());
        menuBar->addAction(menuBaudRate->menuAction());
        menuBar->addAction(menuParity->menuAction());
        menuBar->addAction(menuStopBits->menuAction());
        menuBar->addAction(menuDataBits->menuAction());
        menuBar->addAction(menuLineEndings->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionQuit);
        menuAbout->addAction(actionAboutQt);
        menuAbout->addAction(actionAboutQSerialTerminal);

        retranslateUi(MainWindow);

        connectButton->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionConnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionDisconnect->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionAboutQt->setText(QApplication::translate("MainWindow", "About Qt", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionAboutQt->setShortcut(QApplication::translate("MainWindow", "Alt+Shift+A", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLoadScript->setText(QApplication::translate("MainWindow", "Load Script", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLoadScript->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLENone->setText(QApplication::translate("MainWindow", "None", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLENone->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLECR->setText(QApplication::translate("MainWindow", "Carriage Return (\\r)", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLECR->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+R", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLELF->setText(QApplication::translate("MainWindow", "Line Feed (\\n)", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLELF->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+F", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionLECRLF->setText(QApplication::translate("MainWindow", "CR & LF (\\r\\n) ", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionLECRLF->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+A", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionAboutQSerialTerminal->setText(QApplication::translate("MainWindow", "About QSerialTerminal", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionAboutQSerialTerminal->setShortcut(QApplication::translate("MainWindow", "Shift+A", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
#ifndef QT_NO_TOOLTIP
        sendButton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Select a serial port from the &quot;port&quot; menu above, then connect by clicking the &quot;connect&quot; button at the bottom, then enter some text to send, then click this button to send the text to the serial device</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        sendButton->setText(QApplication::translate("MainWindow", "Send", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        connectButton->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>Select a serial port from the &quot;port&quot; menu above, then click this button to begin communicating with a serial device</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        connectButton->setText(QApplication::translate("MainWindow", "Connect", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        connectButton->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+C", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        menuLineEndings->setTitle(QApplication::translate("MainWindow", "Line Endings", Q_NULLPTR));
        menuBaudRate->setTitle(QApplication::translate("MainWindow", "Baud Rate", Q_NULLPTR));
        menuParity->setTitle(QApplication::translate("MainWindow", "Parity", Q_NULLPTR));
        menuStopBits->setTitle(QApplication::translate("MainWindow", "Stop Bits", Q_NULLPTR));
        menuDataBits->setTitle(QApplication::translate("MainWindow", "Data Bits", Q_NULLPTR));
        menuPortNames->setTitle(QApplication::translate("MainWindow", "Port", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
