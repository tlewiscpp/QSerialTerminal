#ifndef QSERIALTERMINAL_APPLICATONUTILITIES_H
#define QSERIALTERMINAL_APPLICATONUTILITIES_H

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <cstdlib>

#include <ctime>
#include <vector>
#include <tuple>
#include <utility>
#include <regex>
#include <cstdio>
#include <random>
#include <QCryptographicHash>
#include <QString>

#if !defined(_MSC_VER)
#    include  <getopt.h>
#endif //!defined(_MSC_VER)

#include "SerialPort.h"

class QFile;
class QByteArray;

namespace ApplicationUtilities
{
    std::string stripLineEndings(std::string str);
    QString getProgramSettingsDirectory();
    QString getInstallDirectory();
    void checkOrCreateProgramLogDirectory();
    QString getLogFilePath();
    QString getLogFileName();
    QString getOSVersion();
    QString getBuildArchitecture();
    QString getCurrentArchitecture();
    std::string nWhitespace(size_t howMuch);

    extern bool verboseLogging;
    void exitApplication(const std::string &reason, int exitCode);

QString getPID();

    class Random
    {
    public:
        Random() = default;

        explicit Random(std::mt19937::result_type seed);
        int drawNumber(int min, int max);

    private:
        std::mt19937 m_randomEngine{std::random_device{}()};
    };

    int randomBetween(int lowLimit, int highLimit, bool lowInclusive = false, bool highInclusive = false);

    void logString(const std::string &str);
    std::string getPadding(size_t howMuch, char padChar);
    std::string getPadding(size_t howMuch, const char *padString);
    std::string getPadding(size_t howMuch, const std::string &padString);

    int roundIntuitively(double numberToRound);
    bool endsWith(const std::string &stringToCheck, const std::string &matchString);
    bool endsWith(const std::string &stringToCheck, char matchChar);

    template <typename T>
    std::string toStdString(const T &rhs);

    /*snprintf style*/
    template<typename ... Args>
    std::string PStringFormat(const char *format, Args& ... args)
    {
#if defined(__ANDROID__)
        ssize_t size{snprintf(nullptr, 0, format, args ...) + 1};
#else
        ssize_t size{std::snprintf(nullptr, 0, format, args ...) + 1};
#endif
        std::unique_ptr<char[]> stringBuffer{new char[size]};
        snprintf(stringBuffer.get(), size, format, args ...);
        return std::string{stringBuffer.get(), stringBuffer.get() + size - 1};
    }

    int stringToInt(const std::string &str);
    int stringToInt(const char *str);

    /*Base case to break recursion*/
    std::string TStringFormat(const char *formatting);

    /*C# style String.Format()*/
    template <typename First, typename ... Args>
    std::string TStringFormat(const char *formatting, First& first, Args& ... args)
    {
        /* Match exactly one opening brace, one or more numeric digit,
        * then exactly one closing brace, identifying a token */
        static const std::regex targetRegex{"\\{[0-9]+\\}"};
        std::smatch match;
        
        /* Copy the formatting string to a std::string, to
        * make for easier processing, which will eventually
        * be used (the .c_str() method) to pass the remainder
        * of the formatting recursively */
        std::string returnString{formatting};

        /* Copy the formatting string to another std::string, which
        * will get modified in the regex matching loop, to remove the
        * current match from the string and find the next match */
        std::string copyString{formatting};

        /* std::tuple to hold the current smallest valued brace token,
        * wrapped in a std::vector because there can be multiple brace
        * tokens with the same value. For example, in the following format string:
        * "There were {0} books found matching the title {1}, {0}/{2}",
        * this pass will save the locations of the first and second {0} */
        using TokenInformation = std::tuple<int, size_t, size_t>;
        std::vector<TokenInformation> smallestValueInformation{std::make_tuple(-1, 0, 0)};

        /*Iterate through string, finding position and lengths of all matches {x}*/
        while(std::regex_search(copyString, match, targetRegex)) {
            /*Get the absolute position of the match in the original return string*/
            size_t foundPosition{match.position() + (returnString.length() - copyString.length())};
            int regexMatchNumericValue{0};
            try {
                /*Convert the integer value between the opening and closing braces to an int to compare */
                regexMatchNumericValue = stringToInt(returnString.substr(foundPosition + 1, (foundPosition + match.str().length())));
                
                /*Do not allow negative numbers, although this should never get picked up the regex anyway*/
                if (regexMatchNumericValue < 0) {
                    throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
                }
                /* If the numeric value in the curly brace token is smaller than
                * the current smallest (or if the smallest value has not yet been set,
                * ie it is the first match), set the corresponding smallestX variables
                * and wrap them up into a TokenInformation and add it to the std::vector */
                int smallestValue{std::get<0>(smallestValueInformation.at(0))};
                if ((smallestValue == -1) || (regexMatchNumericValue < smallestValue)) {
                    smallestValueInformation.clear();
                    smallestValueInformation.push_back(std::make_tuple(regexMatchNumericValue,
                                                                    foundPosition,
                                                                    match.str().length()));
                } else if (regexMatchNumericValue == smallestValue) {
                    smallestValueInformation.push_back(std::make_tuple(regexMatchNumericValue,
                                                                    foundPosition,
                                                                    match.str().length()));
                }
            } catch (std::exception e) {
                //TODO: Throw instead of just output exception 
                std::cout << e.what() << std::endl;
            }
            copyString = match.suffix();
        }
        int smallestValue{std::get<0>(smallestValueInformation.at(0))};
        if (smallestValue == -1) {
            throw std::runtime_error(TStringFormat("ERROR: In TStringFormat() - Formatted string is invalid (formatting = {0})", formatting));
        }
        /* Set the returnString to be up to the brace token, then the string
        * representation of current argument in line (first), then the remainder
        * of the format string, effectively removing the token and replacing it
        * with the requested item in the final string, then pass it off recursively */
        
        std::string firstString{toStdString(first)};
        int index{0};
        for (const auto &it : smallestValueInformation) {
            size_t smallestValueLength{std::get<2>(it)};


            /* Since the original string will be modified, the adjusted position must be 
            calculated for any repeated brace tokens, kept track of by index.
            The length of string representation of first mutiplied by which the iterationn count
            is added, and the length of the brace token multiplied by the iteration count is
            subtracted, resulting in the correct starting position of the current brace token */
            size_t lengthOfTokenBracesRemoved{index * smallestValueLength};
            size_t lengthOfStringAdded{index * firstString.length()};
            size_t smallestValueAdjustedPosition{std::get<1>(it) + lengthOfStringAdded - lengthOfTokenBracesRemoved};
            returnString = returnString.substr(0, smallestValueAdjustedPosition)
                        + firstString
                        + returnString.substr(smallestValueAdjustedPosition + smallestValueLength);
            index++;
        }
        return TStringFormat(returnString.c_str(), args...);
    }

    template<typename ... Args>
    QString QStringFormat(const char *format, Args ... args)
    {
        ssize_t size{snprintf(nullptr, 0, format, args ...) + 1};
        std::unique_ptr<char[]> stringBuffer{new char[size]};
        snprintf(stringBuffer.get(), size, format, args ...);
        return QString::fromStdString(std::string{stringBuffer.get(), stringBuffer.get() + size - 1});
    }

    std::string boolToString(bool value);
    bool stringToBool(const std::string &value);
    QString boolToQString(bool value);
    bool qStringToBool(const QString &value);
    QByteArray getFileChecksum(const QString &fileName, QCryptographicHash::Algorithm hashAlgorithm);
    QByteArray getFileChecksum(QIODevice *inputDevice, QCryptographicHash::Algorithm hashAlgorithm);

   QString getFileDirectoryPath(const QFile &file);
   QString getFileName(const QFile &file);
   bool looksLikeWindowsFilePath(const QString &path);
    std::string stripFromString(const std::string &stringToStrip, const std::string &whatToStrip);
    std::string stripFromString(const std::string &stringToStrip, char whatToStrip);
    std::string stripAllFromString(const std::string &stringToStrip, const std::string &whatToStrip);
    std::string stripAllFromString(const std::string &stringToStrip, char whatToStrip);

#if !defined(_MSC_VER)
std::string buildShortOptions(option *longOptions, size_t numberOfLongOptions);
#endif //!defined(_MSC_VER)

std::vector<std::string> split(const std::string &inputString, char delimiter);

std::string dataBitsToString(CppSerialPort::DataBits dataBits);
std::string stopBitsToString(CppSerialPort::StopBits stopBits);
std::string parityToString(CppSerialPort::Parity parity);
std::string flowControlToString(CppSerialPort::FlowControl flowControl);
std::string baudRateToString(CppSerialPort::BaudRate baudRate);


template <typename T> inline std::string toStdString(const T &t) {
    return dynamic_cast<std::ostringstream &>(std::ostringstream{} << t).str();
}

template<> inline std::string toStdString(const CppSerialPort::BaudRate &baudRate) { return baudRateToString(baudRate); }
template<> inline std::string toStdString(const CppSerialPort::StopBits &stopBits) { return stopBitsToString(stopBits); }
template<> inline std::string toStdString(const CppSerialPort::Parity &parity) { return parityToString(parity); }
template<> inline std::string toStdString(const CppSerialPort::FlowControl &flowControl) { return flowControlToString(flowControl); }
template<> inline std::string toStdString(const CppSerialPort::DataBits &dataBits) { return dataBitsToString(dataBits); }


}

#endif //QSERIALTERMINAL_APPLICATONUTILITIES_H
