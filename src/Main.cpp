#include <QApplication>
#include <QDesktopWidget>
#include <memory>
#include <QtCore/QDateTime>

#include "ApplicationIcons.h"
#include "ApplicationStrings.h"
#include "MainWindow.h"
#include "GlobalDefinitions.h"
#include "ApplicationUtilities.h"
#include "ApplicationSettings.h"

#if defined(_MSC_VER)
#   include <Windows.h>
#else
#   include <csignal>
#   include <unistd.h>
#endif

const std::list<const char *> HELP_SWITCHES{"-h", "--h", "-help", "--help"};
const std::list<const char *> VERSION_SWITCHES{"v", "-v", "--v", "-version", "--version"};

void displayHelp();
void displayVersion();
void interruptHandler(int signalNumber);
void installSignalHandlers(void (*signalHandler)(int));
void globalLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
void exitApplication(const std::string &why);

using namespace ApplicationStrings;
using namespace GlobalSettings;
using namespace ApplicationUtilities;

int main(int argc, char *argv[])
{
    installSignalHandlers(interruptHandler);
    qInstallMessageHandler(globalLogHandler);
    QCoreApplication::setOrganizationName(AUTHOR_NAME);
    QCoreApplication::setOrganizationDomain(REMOTE_URL);
    QCoreApplication::setApplicationName(PROGRAM_LONG_NAME);

    ApplicationUtilities::checkOrCreateProgramSettingsDirectory();

    std::cout << std::endl;
    for (auto iter = argv + 1; iter != (argv + argc); iter++) {
        if (isSwitch(*iter, HELP_SWITCHES)) {
            displayHelp();
            return 0;
        } else if (isSwitch(*iter, VERSION_SWITCHES)) {
            displayVersion();
            return 0;
        }
    }
    LOG_INFO() << QString{"Using log file %1"}.arg(ApplicationUtilities::getLogFilePath());

    displayVersion();



    using namespace ApplicationStrings;
    QApplication qApplication(argc, argv);
    std::shared_ptr<QDesktopWidget> qDesktopWidget{std::make_shared<QDesktopWidget>()};
    std::shared_ptr<ApplicationIcons> programIcons{std::make_shared<ApplicationIcons>()};
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>(programIcons)};
    mainWindow->setWindowIcon(programIcons->MAIN_WINDOW_ICON);
    mainWindow->setWindowTitle(MAIN_WINDOW_TITLE);
    mainWindow->setStyleSheet(MAIN_WINDOW_STYLESHEET);
    QRect screenGeometry{QApplication::desktop()->screenGeometry()};
    int x{(screenGeometry.width() - mainWindow->width()) / 2};
    int y{(screenGeometry.height() - mainWindow->height()) / 2};
    mainWindow->move(x, y);
    mainWindow->show();

    return qApplication.exec();
}



void exitApplication(const std::string &why)
{
    LOG_INFO() << "Exiting application for reason: \"" << QString::fromStdString(why) << "\"";
    interruptHandler(SIGQUIT);
}

void interruptHandler(int signalNumber)
{
#if defined(_WIN32)
    Q_UNUSED(signalNumber);
#else
    if ((signalNumber == SIGUSR1) || (signalNumber == SIGUSR2) || (signalNumber == SIGCHLD)) {
        return;
    }
    LOG_INFO() << QString{"Caught signal %1 (%2), exiting %3"}.arg(QS_NUMBER(signalNumber), strsignal(signalNumber), PROGRAM_NAME);
    exit (signalNumber);
#endif
}

void installSignalHandlers(void (*signalHandler)(int))
{
#if defined(_WIN32)
    Q_UNUSED(signalHandler);
#else

    static struct sigaction signalInterruptHandler;
    signalInterruptHandler.sa_handler = signalHandler;
    sigemptyset(&signalInterruptHandler.sa_mask);
    signalInterruptHandler.sa_flags = 0;
    sigaction(SIGHUP, &signalInterruptHandler, NULL);
    sigaction(SIGINT, &signalInterruptHandler, NULL);
    sigaction(SIGQUIT, &signalInterruptHandler, NULL);
    sigaction(SIGILL, &signalInterruptHandler, NULL);
    sigaction(SIGABRT, &signalInterruptHandler, NULL);
    sigaction(SIGFPE, &signalInterruptHandler, NULL);
    sigaction(SIGPIPE, &signalInterruptHandler, NULL);
    sigaction(SIGALRM, &signalInterruptHandler, NULL);
    sigaction(SIGTERM, &signalInterruptHandler, NULL);
    sigaction(SIGUSR1, &signalInterruptHandler, NULL);
    sigaction(SIGUSR2, &signalInterruptHandler, NULL);
    sigaction(SIGCHLD, &signalInterruptHandler, NULL);
    sigaction(SIGCONT, &signalInterruptHandler, NULL);
    sigaction(SIGTSTP, &signalInterruptHandler, NULL);
    sigaction(SIGTTIN, &signalInterruptHandler, NULL);
    sigaction(SIGTTOU, &signalInterruptHandler, NULL);
#endif
}

void displayVersion()
{
    using namespace ApplicationUtilities;
    LOG_INFO() << QString{"%1, v%2.%3.%4"}.arg(PROGRAM_NAME, QS_NUMBER(SOFTWARE_MAJOR_VERSION), QS_NUMBER(SOFTWARE_MINOR_VERSION), QS_NUMBER(SOFTWARE_PATCH_VERSION));
    LOG_INFO() << QString{"Written by %1"}.arg(AUTHOR_NAME);
    LOG_INFO() << QString{"Built with %1 v%2.%3.%4, %5"}.arg(COMPILER_NAME, QS_NUMBER(COMPILER_MAJOR_VERSION), QS_NUMBER(COMPILER_MINOR_VERSION), QS_NUMBER(COMPILER_PATCH_VERSION), __DATE__);
    LOG_INFO() << QString{"Build architecture: %1"}.arg(getBuildArchitecture());
    LOG_INFO() << QString{"Current detected architecture: %1"}.arg(getCurrentArchitecture());
    LOG_INFO() << QString{"Detected OS version: %1"}.arg(getOSVersion());
}


void displayHelp()
{
    LOG_INFO() << QString{"Usage: %1"}.arg(PROGRAM_NAME);
    LOG_INFO() << "Options: ";
    LOG_INFO() << "    -h, --h, -help, --help: Display this help text";
    LOG_INFO() << "    -v, --v, -version, --version: Display the version";
}

template <typename StringType, typename FileStringType>
void logToFile(const StringType &str, const FileStringType &filePath)
{
    QFile qFile{filePath};
    QString stringCopy{toQString(str)};
    if (qFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        if (qFile.write(toStdString(str).c_str(), toStdString(str).length()) == -1) {
            throw std::runtime_error(QString{"Failed to log data \"%1\" to file \"%2\" (file was opened, but not writable, permission problem?)"}.arg(toQString(str), toQString(filePath)).toStdString());
        }
    } else {
        throw std::runtime_error(QString{"Failed to log data \"%1\" to file \"%2\" (could not open file)"}.arg(toQString(str), toQString(filePath)).toStdString());
    }

}

void globalLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg{msg.toLocal8Bit()};
    QString logContext{""};
    auto *outputStream = &std::cout;
    switch (type) {
        case QtDebugMsg:
            logContext = "{Debug}: ";
            outputStream = &std::cout;
            break;
        case QtInfoMsg:
            logContext = "{Info}: ";
            outputStream = &std::clog;
            break;
        case QtWarningMsg:
            logContext = "{Warning}: ";
            outputStream = &std::cout;
            break;
        case QtCriticalMsg:
            logContext = "{Critical}: ";
            outputStream = &std::cerr;
            break;
        case QtFatalMsg:
            logContext = "{Fatal}: ";
            outputStream = &std::cerr;
            abort();
    }
    QString logMessage{""};
    std::string coreLogMessage{QString{localMsg}.toStdString()};
    if (coreLogMessage.find("\"") == 0) {
        coreLogMessage = coreLogMessage.substr(1);
    }
    if (coreLogMessage.find_last_of("\"") == coreLogMessage.length() - 1) {
        coreLogMessage = coreLogMessage.substr(0, coreLogMessage.length() - 1);
    }
    //coreLogMessage.erase(std::remove_if(coreLogMessage.begin(), coreLogMessage.end(),[](char c) { return c == '\"'; }), coreLogMessage.end());
    if ((type == QtCriticalMsg) || (type == QtFatalMsg)) {
        logMessage = QString{"[%1] - %2 %3 (%4:%5, %6)"}.arg(QDateTime::currentDateTime().time().toString(), logContext, coreLogMessage.c_str(), context.file, QS_NUMBER(context.line), context.function);
    } else {
        logMessage = QString{"[%1] - %2 %3"}.arg(QDateTime::currentDateTime().time().toString(), logContext, coreLogMessage.c_str());
    }
    bool addLineEnding{true};
    static const QList<const char *> LINE_ENDINGS{"\r\n", "\r", "\n", "\n\r"};
    for (const auto &it : LINE_ENDINGS) {
        if (logMessage.endsWith(it)) {
            addLineEnding = false;
        }
    }
    if (addLineEnding) {
        logMessage.append("\n");
    }
    if (outputStream) {
        *outputStream << logMessage.toStdString();
    }
    logToFile(logMessage.toStdString(), ApplicationUtilities::getLogFilePath());
    outputStream->flush();
}

