#include <QApplication>
#include <QDesktopWidget>
#include <memory>
#include <QtCore/QDateTime>
#include <QLabel>
#include <QTimer>

#if defined(_WIN32)
#    include <Windows.h>
#endif //defined(_WIN32)

#include "ApplicationIcons.h"
#include "ApplicationStrings.h"
#include "MainWindow.h"
#include "GlobalDefinitions.h"
#include "ApplicationUtilities.h"
#include "ApplicationSettings.h"
#include "SingleInstanceGuard.h"


#if !defined(_MSC_VER)
#   include <csignal>
#   include <unistd.h>
#   include <getopt.h>

static struct option longOptions[]{
	{ "verbose",        no_argument,       nullptr, 'e' },
{ "help",           no_argument,       nullptr, 'h' },
{ "version",        no_argument,       nullptr, 'v' },
{ nullptr, 0, nullptr, 0 }
};
#endif //!defined(_MSC_VER)



#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

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
    //https://stackoverflow.com/a/28172162/4791654
    SingleInstanceGuard singleInstanceGuard{PROGRAM_LONG_NAME};
    if (!singleInstanceGuard.tryLockProcess()) {
        std::cerr << "Program already running, exiting" << std::endl;
        exit(1);
    }

    installSignalHandlers(interruptHandler);
    qInstallMessageHandler(globalLogHandler);
    QCoreApplication::setOrganizationName(AUTHOR_NAME);
    QCoreApplication::setOrganizationDomain(REMOTE_URL);
    QCoreApplication::setApplicationName(PROGRAM_LONG_NAME);

    ApplicationUtilities::checkOrCreateProgramSettingsDirectory();

#if defined(_MSC_VER)
    for (int i = 0; i < argc; i++) {
        auto it = argv[i];
        if (strlen(it) <= 0) {
            continue;
        }
        bool longOption{( (it[0] == '-') && (it[1] == '-') )};
        bool shortOption{(it[0] == '-')};
        if (longOption) {
            auto newIt = std::string{it + 2};
            if (newIt == "verbose") {
                ApplicationUtilities::verboseLogging = true;
                LOG_INFO() << "Setting LogLevel to verbose due to command line option";
            } else if (newIt == "version") {
                displayVersion();
                exit(EXIT_SUCCESS);
            } else if (newIt == "help") {
                displayHelp();
                exit(EXIT_SUCCESS);
            } else {
                LOG_WARN() << QString{"Invalid switch \"%1\" detected"}.arg(QString{it});
            }
        } else if (shortOption) {
            auto newIt = std::string{it + 1};
            if (newIt == "e") {
                ApplicationUtilities::verboseLogging = true;
                LOG_INFO() << "Setting LogLevel to verbose due to command line option";
            } else if (newIt == "v") {
                displayVersion();
                exit(EXIT_SUCCESS);
            } else if (newIt == "h") {
                displayHelp();
                exit(EXIT_SUCCESS);
            } else {
                LOG_WARN() << QString{"Invalid switch \"%1\" detected"}.arg(QString{it});
            }
        } else {
            LOG_WARN() << QString{"Invalid switch \"%1\" detected"}.arg(QString{it});
        }
    }
#else
    int optionIndex{0};
    int currentOption{0};
    opterr = 0;
    std::string shortOptions{ApplicationUtilities::buildShortOptions(longOptions, ARRAY_SIZE(longOptions))};
    while ( (currentOption = getopt_long(argc, argv, shortOptions.c_str(), longOptions, &optionIndex)) != -1) {
        switch (currentOption) {
            case 'h':
                displayHelp();
                exit(EXIT_SUCCESS);
            case 'v':
                displayVersion();
                exit(EXIT_SUCCESS);
            case 'e':
                ApplicationUtilities::verboseLogging = true;
                LOG_INFO() << "Setting LogLevel to verbose due to command line option";
                break;
            default:
                LOG_WARN() << QString{"Invalid switch \"%1\" detected"}.arg(QString{optarg});
                break;
        };
    }
#endif //defined(_MSC_VER)
    displayVersion();
    LOG_INFO() << QString{"Using log file %1"}.arg(ApplicationUtilities::getLogFilePath());
    LOG_INFO() << QString{"Current PID: %1"}.arg(ApplicationUtilities::getPID());

    using namespace ApplicationStrings;
    QApplication qApplication{argc, argv};
    ApplicationIcons::initializeInstance();
    std::shared_ptr<MainWindow> mainWindow{std::make_shared<MainWindow>()};
    mainWindow->setWindowIcon(applicationIcons->MAIN_WINDOW_ICON);
    mainWindow->setWindowTitle(MAIN_WINDOW_TITLE);
    mainWindow->setStyleSheet(MAIN_WINDOW_STYLESHEET);
    QRect screenGeometry{QApplication::desktop()->screenGeometry()};
    int x{(screenGeometry.width() - mainWindow->width()) / 2};
    int y{(screenGeometry.height() - mainWindow->height()) / 2};
    mainWindow->move(x, y);
    mainWindow->show();

    return qApplication.exec();
}


void interruptHandler(int signalNumber)
{
#if defined(_WIN32)
    Q_UNUSED(signalNumber);
    exitApplication("interruptHandler", signalNumber);
#else
    if ((signalNumber == SIGUSR1) || (signalNumber == SIGUSR2)) {
        LOG_INFO() << QString{"Changing log level to LogLevel to verbose due to external %1"}.arg(signalNumber == SIGUSR1 ? "SIGUSR1" : "SIGUSR2");
        return;
    }
    if (signalNumber == SIGCHLD) {
        return;
    }
    LOG_INFO() << QString{"Caught signal %1 (%2), exiting %3"}.arg(QS_NUMBER(signalNumber), strsignal(signalNumber), PROGRAM_NAME);
    exitApplication(strsignal(signalNumber), signalNumber);
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
    std::cout << QString{"Usage: %1 Option [=value]"}.arg(PROGRAM_NAME).toStdString() << std::endl;
    std::cout << "Options: " << std::endl;
    std::cout << "    -h, --help: Display this help text" << std::endl;
    std::cout << "    -v, --version: Display the version" << std::endl;
    std::cout << "    -e, --verbose: Enable verbose logging" << std::endl;
}

void logToFile(const std::string &str, const std::string &filePath)
{
    QFile qFile{filePath.c_str()};
    QString stringCopy{str.c_str()};
    if (qFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        if (qFile.write(toStdString(str).c_str(), static_cast<qint64>(str.length())) == -1) {
            throw std::runtime_error(QString{
                    R"(Failed to log data "%1" to file "%2" (file was opened, but not writable, permission problem?))"}.arg(str.c_str(), filePath.c_str()).toStdString());
        }
    } else {
        throw std::runtime_error(QString{R"(Failed to log data "%1" to file "%2" (could not open file))"}.arg(str.c_str(), filePath.c_str()).toStdString());
    }

}

void globalLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg{msg.toLocal8Bit()};
    QString logContext{""};
    auto *outputStream = &std::cout;
    switch (type) {
        case QtDebugMsg:
            if (!ApplicationUtilities::verboseLogging) {
                return;
            }
            logContext = "{  Debug }: ";
            outputStream = &std::cout;
            break;
        case QtInfoMsg:
            logContext = "{  Info  }: ";
            outputStream = &std::clog;
            break;
        case QtWarningMsg:
            logContext = "{  Warn  }: ";
            outputStream = &std::cout;
            break;
        case QtCriticalMsg:
            logContext = "{Critical}: ";
            outputStream = &std::cerr;
            break;
        case QtFatalMsg:
            logContext = "{  Fatal }: ";
            outputStream = &std::cerr;
    }
    QString logMessage{""};
    std::string coreLogMessage{QString{localMsg}.toStdString()};
    if (coreLogMessage.find('\"') == 0) {
        coreLogMessage = coreLogMessage.substr(1);
    }
    if (coreLogMessage.find_last_of('\"') == coreLogMessage.length() - 1) {
        coreLogMessage = coreLogMessage.substr(0, coreLogMessage.length() - 1);
    }
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
    logToFile(logMessage.toStdString(), ApplicationUtilities::getLogFilePath().toStdString());
    outputStream->flush();
    if (type == QtMsgType::QtFatalMsg) {
        abort();
    }
}

