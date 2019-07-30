#include "ApplicationUtilities.hpp"
#include "GlobalDefinitions.hpp"
#include "ApplicationSettings.hpp"
#include "ApplicationStrings.hpp"
#include <QDateTime>
#include <QtCore/QCoreApplication>

#if defined(_WIN32)
#    include <third-party/win32/getopt/getopt.h>
#else
#    include <getopt.h>
#endif //defined(_WIN32)

namespace ApplicationUtilities  {

static QString programSettingsDirectory{""};
static QString logFileName{""};

bool verboseLogging{false};
static QString PID{""};
static QString processUUID{""};


std::string buildShortOptions(option *longOptions, size_t numberOfLongOptions)
{
    std::string returnString{""};
    for (size_t i = 0; i < numberOfLongOptions; i++) {
        option *currentOption{longOptions + i};
        if (currentOption->val == 0) {
            continue;
        }
        returnString += static_cast<char>(currentOption->val);
        if (currentOption->has_arg == required_argument) {
            returnString += ':';
        }
    }
    return returnString;
}


void exitApplication(const std::string &why, int exitCode)
{
    LOG_INFO() << QString{R"(Exiting application for reason: "%1" (exit code: %2))"}.arg(why.c_str(), QS_NUMBER(exitCode));
    _Exit(exitCode);
}

QString getPID() {
    if (PID.isEmpty()) {
        PID = QS_NUMBER(QCoreApplication::applicationPid());
    }
    return PID;
}

    std::string stripLineEndings(std::string str)
    {
        if (str.length() == 0) {
            return str;
        }
        if (endsWith(str, '\r')) {
            str.pop_back();
        }
        if (str.length() == 0) {
            return str;
        }
        if (endsWith(str, '\n')) {
            str.pop_back();
            return str;
        }
        return str;
    }

    std::string nWhitespace(size_t howMuch)
    {
        return std::string(howMuch, ' ');
    }


    QString getProgramSettingsDirectory()
    {
        if (!programSettingsDirectory.isEmpty()) {
            return programSettingsDirectory;
        }
        QString homeDirectory{QDir::toNativeSeparators(QDir::homePath())};
        QDir baseDirectory{homeDirectory};
        if (!baseDirectory.exists()) {
            throw std::runtime_error("You don't exist, go away");
        }
        QString additionalSettingsPath{""};
        #if defined(_WIN32)
            if (QSysInfo::windowsVersion() > QSysInfo::WinVersion::WV_VISTA) {
                additionalSettingsPath += (QString{"\\AppData\\Local\\"} + GlobalSettings::PROGRAM_LONG_NAME + "\\");
            } else {
                additionalSettingsPath += (QString{"\\"} + GlobalSettings::PROGRAM_LONG_NAME + "\\");
            }
        #else
            additionalSettingsPath += (QString{"/.local/share/"} + GlobalSettings::PROGRAM_LONG_NAME + "/");
        #endif
        programSettingsDirectory = homeDirectory + additionalSettingsPath;
        return programSettingsDirectory;

    }


    QString getOSVersion()
    {
    #if defined(_WIN32)
        if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_32s) {
            return "Windows 3.1";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_95) {
            return "Windows 95";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_98) {
            return "Windows 98";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_Me) {
            return "Windows Me";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_NT) {
            return "Windows NT";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_2000) {
            return "Windows 2000";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_XP) {
            return "Windows XP";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_2003) {
            return "Windows 2003";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_VISTA) {
            return "Windows Vista";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS7) {
            return "Windows 7";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS8) {
            return "Windows 8";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS8_1) {
            return "Windows 8.1";
        } else if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_WINDOWS10) {
            return "Windows 10";
        } else {
            return "I dunno, maybe DOS or something";
        }
    #elif defined(__linux__)
        return "Linux, or something";
    #elif defined(__MAC_OSX__)
        if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_None) {
            return "Non Darwin-based MacOS";
        }else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_9) {
            return "MacOS 9";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_0) {
            return "MacOS 10.0 (Cheetah)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_1) {
            return "MacOS 10.1 (Puma)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_2) {
            return "MacOS 10.2 (Jaguar)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_3) {
            return "MacOS 10.3 (Panther)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_4) {
            return "MacOS 10.4 (Tiger)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_5) {
            return "MacOS 10.5 (Leopard)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_6) {
            return "MacOS 10.6 (Snow Leopard)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_7) {
            return "MacOS 10.7 (Lion)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_8) {
            return "MacOS 10.8 (Mountain Lion)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_9) {
            return "MacOS 10.9 (Mavericks)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_10) {
            return "MacOS 10.10 (Yosemite)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_11) {
            return "MacOS 10.11 (El Capitan)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_10_12) {
            return "MacOS 10.12 (Sierra)";
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_4_3) {
            return "iOS 4.3"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_5_0) {
            return "iOS 5.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_5_1) {
            return "iOS 5.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_6_0) {
            return "iOS 6.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_6_1) {
            return "iOS 6.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_7_0) {
            return "iOS 7.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_7_1) {
            return "iOS 7.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_0) {
            return "iOS 8.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_1) {
            return "iOS 8.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_2) {
            return "iOS 8.2"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_3) {
            return "iOS 8.3"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_8_4) {
            return "iOS 8.4"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
            return "iOS 9.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
            return "iOS 9.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
            return "iOS 9.2"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_9_0) {
            return "iOS 9.3"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_10_0) {
            return "iOS 10.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_0) {
            return "tvOS 9.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_1) {
            return "tvOS 9.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_TVOS_9_2) {
            return "tvOS 9.2"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_IOS_10_0) {
            return "tvOS 10.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_0) {
            return "watchOS 2.0"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_1) {
            return "watchOS 2.1"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_2_2) {
            return "watchOS 2.2"
        } else if (QSysInfo::macVersion() == QSysInfo::MacintoshVersion::MV_WATCH_3_0) {
            return "watchOS 3.0"
        } else {
            return "Unknown MacOS version";
        }
    #else
        return "Unknown OS";
    #endif
    }

    QString getBuildArchitecture()
    {
        return QSysInfo::buildCpuArchitecture();
    }

    QString getCurrentArchitecture()
    {
        return QSysInfo::currentCpuArchitecture();
    }

	bool clearDirectoryOfFiles(const QString &directoryPath) {
		QDir directory{ directoryPath };
		if (!directory.exists()) {
			return false;
		}
		directory.setNameFilters({ "*.*" });
		directory.setFilter(QDir::Files);
		for (auto &it : directory.entryList()) {
			directory.remove(it);
		}
		return true;
	}


	void checkOrCreateProgramLogDirectory() {
		std::vector<QString> toLogInfo{};
		QString settings{ QDir::tempPath() + '/' + QCoreApplication::applicationName() };
		QDir settingsDirectory{ settings };
		if (settingsDirectory.exists()) {
			clearDirectoryOfFiles(settings);
			toLogInfo.push_back(QString{ "Detected log directory at %1" }.arg(settings));
			toLogInfo.emplace_back("Cleared stale log entries");
		} else {
			if (settingsDirectory.mkpath(".")) {
				toLogInfo.push_back(QString{ "Log directory not found, created new directory at %1" }.arg(settings));
			} else {
				throw std::runtime_error(QString{ "Log directory not found, and one could not be created at %1" }.arg(settings).toStdString());
			}
		}
		for (auto &it : toLogInfo) {
			LOG_INFO() << it;
		}
	}
	QString getLogFilePath() {
		if (logFileName.isEmpty()) {
			QString log{ getLogFileName() };
		}
		return QString{ "%1/%2/%3" }.arg(QDir::tempPath(), QCoreApplication::applicationName(), logFileName);
	}

	QString getLogFileName() {
		if (logFileName.isEmpty()) {
			QString currentDateTime{ QDateTime::currentDateTime().toString() };
			QString newDateTime{ "" };
			for (const auto &it : currentDateTime) {
				if ((it == ' ') || (it == ':')) {
					newDateTime += '-';
				} else {
					newDateTime += it.toLower();
				}
			}
			//logFileName = QString{"%1-%2.log"}.arg(newDateTime, QS_NUMBER(randomBetween(0, 60000)));
			logFileName = QString{ "%1.log" }.arg(newDateTime);
		}
		return logFileName;
		}


    static std::unique_ptr<Random> randomDevice = std::unique_ptr<Random>{new Random()};

    Random::Random(std::mt19937::result_type seed) :
        m_randomEngine{seed}
    {

    }

    int Random::drawNumber(int min, int max)
    {
        return std::uniform_int_distribution<int>{min, max}(this->m_randomEngine);
    }

    int randomBetween(int lowLimit, int highLimit, bool lowInclusive, bool highInclusive)
    {
        int low{lowInclusive ? lowLimit : lowLimit + 1};
        int high{highInclusive ? highLimit : highLimit - 1};
        return randomDevice->drawNumber(low, high);
    }

    int roundIntuitively(double numberToRound)
    {
        double tempContainer{numberToRound - static_cast<int>(numberToRound)};
        if (tempContainer >= 0.5) {
            return (static_cast<int>(numberToRound) + 1);
        } else {
            return static_cast<int>(numberToRound);
        }
    }

    bool endsWith(const std::string &stringToCheck, const std::string &matchString)
    {
        if (matchString.size() > stringToCheck.size()) {
            return false;
        }
        return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
    }

    bool endsWith(const std::string &stringToCheck, char matchChar)
    {
        return endsWith(stringToCheck, std::string{1, matchChar});
    }

    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip)
    {
        std::string returnString{stringToStrip};
        if (returnString.find(whatToStrip) == std::string::npos) {
            return returnString;
        }
        size_t foundPosition{stringToStrip.find(whatToStrip)};
        if (foundPosition == 0) {
            returnString = returnString.substr(whatToStrip.length());
        } else if (foundPosition == (returnString.length() - whatToStrip.length())) {
            returnString = returnString.substr(0, foundPosition);
        } else {
            returnString = returnString.substr(0, foundPosition) + returnString.substr(foundPosition+whatToStrip.length());
        }
        return returnString;
    }

    std::string stripFromString(const std::string &stringToStrip, char whatToStrip)
    {
        return stripFromString(stringToStrip, std::string(1, whatToStrip));
    }


    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip)
       {
           std::string returnString = stringToStrip;
           if (returnString.find(whatToStrip) == std::string::npos) {
               return returnString;
           }
           while (returnString.find(whatToStrip) != std::string::npos) {
               returnString = stripFromString(returnString, whatToStrip);
           }
           return returnString;
       }

       std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip)
       {
           return stripAllFromString(stringToStrip, std::string(1, whatToStrip));
       }


   std::string boolToString(bool value)
   {
       return (value ? "true" : "false");
   }

   bool stringToBool(const std::string &value)
   {
       std::string copyString{value};
       std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
       return copyString == "true";
   }

   QString boolToQString(bool value)
   {
       return QString::fromStdString(boolToString(value));
   }

   bool qStringToBool(const QString &value)
   {
       return stringToBool(value.toStdString());
   }

   QByteArray getFileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm)
   {
       QFile inputFile{fileName};
       if (!inputFile.exists()) {
           throw std::runtime_error(QString{"In QmsUtilities::getFileChecksum(const QString &, QCryptographicHash::Algorithm): input file %1 does not exist"}.arg(fileName).toStdString());
       }
       if (!inputFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
           throw std::runtime_error(QString{"In QmsUtilities::getFileChecksum(const QString &, QCryptographicHash::Algorithm): could not open file %1"}.arg(fileName).toStdString());
       }
       QCryptographicHash hash{hashAlgorithm};
       hash.addData(&inputFile);
       return hash.result();
   }

   QByteArray getFileChecksum(QIODevice *inputDevice, QCryptographicHash::Algorithm hashAlgorithm)
   {
       if (!inputDevice) {
           throw std::runtime_error("In QmsUtilities::getFileChecksum(QIODevice *, QCryptographicHash::Algorithm): input QIODevice is a nullptr");
       }
       if (!inputDevice->isOpen()) {
           if (!inputDevice->open(QIODevice::OpenModeFlag::ReadOnly)) {
               throw std::runtime_error("In QmsUtilities::getFileChecksum(QIODevice *, QCryptographicHash::Algorithm): could not open QIODevice");
           }
       }
       QCryptographicHash hash{hashAlgorithm};
       hash.addData(inputDevice);
       return hash.result();
   }

   QString getFileDirectoryPath(const QFile &file)
   {
       if (!file.exists()) {
           return QString{""};
       }
       QChar pathSeparator{'/'};
       if (looksLikeWindowsFilePath(file.fileName())) {
           pathSeparator = '\\';
       }
       QString tempString{file.fileName()};
       auto lastSeparator = tempString.lastIndexOf(pathSeparator);
       if (lastSeparator == -1) {
           return QString{""};
       }
       return tempString.mid(0, lastSeparator + 1);
   }

   QString getFileName(const QFile &file)
   {
       if (!file.exists()) {
           return QString{""};
       }
       QChar pathSeparator{'/'};
       if (looksLikeWindowsFilePath(file.fileName())) {
           pathSeparator = '\\';
       }
       QString tempString{file.fileName()};
       auto lastSeparator = tempString.lastIndexOf(pathSeparator);
       if (lastSeparator == -1) {
           return QString{""};
       }
       return tempString.mid(lastSeparator + 1);
   }

   bool looksLikeWindowsFilePath(const QString &path)
   {
       std::string copyString{path.toStdString()};
       auto windowsPathCount = std::count_if(copyString.begin(), copyString.end(), [] (char c) { return c == '\\'; });
       auto nixPathCount = std::count_if(copyString.begin(), copyString.end(), [](char c) { return c == '/'; });
       return windowsPathCount > nixPathCount;
   }

std::vector<std::string> split(const std::string &inputString, char delimiter)
{
    std::istringstream stringStream{inputString};
    std::string item{""};
    std::vector<std::string> returnVector{};
    while(std::getline(stringStream, item, delimiter)) {
        returnVector.push_back(item);
    }
    return returnVector;
}

std::string dataBitsToString(CppSerialPort::DataBits dataBits) {
    switch (dataBits) {
        case CppSerialPort::DataBits::DataFive:  return "5";
        case CppSerialPort::DataBits::DataSix:   return "6";
        case CppSerialPort::DataBits::DataSeven: return "7";
        case CppSerialPort::DataBits::DataEight: return "8";
    }
    Q_UNREACHABLE();
}

std::string stopBitsToString(CppSerialPort::StopBits stopBits) {
#if defined(_WIN32)
    switch (stopBits) {
        case CppSerialPort::StopBits::StopOne:     return "1";
        case CppSerialPort::StopBits::StopOneFive: return "1.5";
        case CppSerialPort::StopBits::StopTwo:     return "2";
    }
#else
    switch (stopBits) {
        case CppSerialPort::StopBits::StopOne:     return "1";
        case CppSerialPort::StopBits::StopTwo:     return "2";
    }
#endif
    Q_UNREACHABLE();
}

std::string parityToString(CppSerialPort::Parity parity) {
#if defined(_WIN32)
    switch (parity) {
        case CppSerialPort::Parity::ParityEven:  return "Even";
        case CppSerialPort::Parity::ParityOdd:   return "Odd";
        case CppSerialPort::Parity::ParityNone:  return "None";
        case CppSerialPort::Parity::ParitySpace: return "Space";
        case CppSerialPort::Parity::ParityMark:  return "Mark";
    }
#else
    switch (parity) {
        case CppSerialPort::Parity::ParityEven:  return "Even";
        case CppSerialPort::Parity::ParityOdd:   return "Odd";
        case CppSerialPort::Parity::ParityNone:  return "None";
        case CppSerialPort::Parity::ParitySpace: return "Space";
    }
#endif //defined(_WIN32)
    Q_UNREACHABLE();
}

std::string flowControlToString(CppSerialPort::FlowControl flowControl) {
    switch (flowControl) {
        case CppSerialPort::FlowControl::FlowOff:      return "Off";
        case CppSerialPort::FlowControl::FlowHardware: return "Hardware";
        case CppSerialPort::FlowControl::FlowXonXoff:  return "XonXoff";
    }
    Q_UNREACHABLE();

}

std::string baudRateToString(CppSerialPort::BaudRate baudRate) {
#if defined(_WIN32)
    switch (baudRate) {
        case CppSerialPort::BaudRate::Baud110:     return "110";
        case CppSerialPort::BaudRate::Baud300:     return "300";
        case CppSerialPort::BaudRate::Baud600:     return "600";
        case CppSerialPort::BaudRate::Baud1200:    return "1200";
        case CppSerialPort::BaudRate::Baud2400:    return "2400";
        case CppSerialPort::BaudRate::Baud4800:    return "4800";
        case CppSerialPort::BaudRate::Baud9600:    return "9600";
        case CppSerialPort::BaudRate::Baud19200:   return "19200";
        case CppSerialPort::BaudRate::Baud38400:   return "38400";
        case CppSerialPort::BaudRate::Baud57600:   return "57600";
        case CppSerialPort::BaudRate::Baud115200:  return "115200";
        case CppSerialPort::BaudRate::Baud128000:  return "128000";
        case CppSerialPort::BaudRate::Baud256000:  return "256000";
        case CppSerialPort::BaudRate::Baud230400:  return "230400";
        case CppSerialPort::BaudRate::Baud460800:  return "460800";
        case CppSerialPort::BaudRate::Baud500000:  return "500000";
        case CppSerialPort::BaudRate::Baud576000:  return "576000";
        case CppSerialPort::BaudRate::Baud921600:  return "921600";
        case CppSerialPort::BaudRate::Baud1000000: return "1000000";
        case CppSerialPort::BaudRate::Baud1152000: return "1152000";
        case CppSerialPort::BaudRate::Baud1500000: return "1500000";
        case CppSerialPort::BaudRate::Baud2000000: return "2000000";
        case CppSerialPort::BaudRate::Baud2500000: return "2500000";
        case CppSerialPort::BaudRate::Baud3000000: return "3000000";
        case CppSerialPort::BaudRate::Baud3500000: return "3500000";
        case CppSerialPort::BaudRate::Baud4000000: return "4000000";
    }
#else
    switch (baudRate) {
        case CppSerialPort::BaudRate::Baud50:      return "50";
        case CppSerialPort::BaudRate::Baud75:      return "75";
        case CppSerialPort::BaudRate::Baud110:     return "110";
        case CppSerialPort::BaudRate::Baud134:     return "134";
        case CppSerialPort::BaudRate::Baud150:     return "150";
        case CppSerialPort::BaudRate::Baud200:     return "200";
        case CppSerialPort::BaudRate::Baud300:     return "300";
        case CppSerialPort::BaudRate::Baud600:     return "600";
        case CppSerialPort::BaudRate::Baud1200:    return "1200";
        case CppSerialPort::BaudRate::Baud1800:    return "1800";
        case CppSerialPort::BaudRate::Baud2400:    return "2400";
        case CppSerialPort::BaudRate::Baud4800:    return "4800";
        case CppSerialPort::BaudRate::Baud9600:    return "9600";
        case CppSerialPort::BaudRate::Baud19200:   return "19200";
        case CppSerialPort::BaudRate::Baud38400:   return "38400";
        case CppSerialPort::BaudRate::Baud57600:   return "57600";
        case CppSerialPort::BaudRate::Baud115200:  return "115200";
        case CppSerialPort::BaudRate::Baud230400:  return "230400";
        case CppSerialPort::BaudRate::Baud460800:  return "460800";
        case CppSerialPort::BaudRate::Baud500000:  return "500000";
        case CppSerialPort::BaudRate::Baud576000:  return "576000";
        case CppSerialPort::BaudRate::Baud921600:  return "921600";
        case CppSerialPort::BaudRate::Baud1000000: return "1000000";
        case CppSerialPort::BaudRate::Baud1152000: return "1152000";
        case CppSerialPort::BaudRate::Baud1500000: return "1500000";
        case CppSerialPort::BaudRate::Baud2000000: return "2000000";
        case CppSerialPort::BaudRate::Baud2500000: return "2500000";
        case CppSerialPort::BaudRate::Baud3000000: return "3000000";
        case CppSerialPort::BaudRate::Baud3500000: return "3500000";
        case CppSerialPort::BaudRate::Baud4000000: return "4000000";
    }
#endif //defined(_WIN32)
    Q_UNREACHABLE();


}

}
