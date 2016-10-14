#ifndef QSERIALTERMINAL_QSERIALTERMINALUTILITIES_H
#define QSERIALTERMINAL_QSERIALTERMINALUTILITIES_H

#include <iostream>
#include <ctime>
#include <QString>
#include <sstream>
#include <string>

namespace QSerialTerminalUtilities
{
    void logString(const std::string &str);

    template<typename T>
    std::string toString(const T &convert)
    {
        std::string returnString{""};
        std::stringstream transfer;
        transfer << convert;
        transfer >> returnString;
        return returnString;
    }
    std::string toString(const std::string &str);
    std::string toString(const char *str);

    template <typename T>
    QString toQString(const T &convert) { return QString::fromStdString(toString(convert)); }
    QString toQString(const std::string &convert);
    QString toQString(const char *convert);
    QString toQString(const QString &convert);

    int roundIntuitively(double numberToRound);
    int randomBetween(int lowLimit, int highLimit);

    const long long int constexpr NANOSECONDS_PER_MICROSECOND{1000};
    const long long int constexpr NANOSECONDS_PER_MILLISECOND{1000000};
    const long long int constexpr NANOSECONDS_PER_SECOND{1000000000};
    const long long int constexpr NANOSECONDS_PER_MINUTE{60000000000};
    const long long int constexpr NANOSECONDS_PER_HOUR{3600000000000};
    const long long int constexpr NANOSECONDS_PER_DAY {86400000000000};

    const long long int constexpr MICROSECONDS_PER_MILLISECOND{1000};
    const long long int constexpr MICROSECONDS_PER_SECOND{1000000};
    const long long int constexpr MICROSECONDS_PER_MINUTE{60000000};
    const long long int constexpr MICROSECONDS_PER_HOUR{3600000000};
    const long long int constexpr MICROSECONDS_PER_DAY{86400000000};

    const long long int constexpr MILLISECONDS_PER_SECOND{1000};
    const long long int constexpr MILLISECONDS_PER_MINUTE{60000};
    const long long int constexpr MILLISECONDS_PER_HOUR{3600000};
    const long long int constexpr MILLISECONDS_PER_DAY{86400000};

    const long long int constexpr SECONDS_PER_MINUTE{60};
    const long long int constexpr SECONDS_PER_HOUR{3600};

    const long long int constexpr MINUTES_PER_HOUR{60};
    const long long int constexpr MINUTES_PER_DAY{1440};

    const long long int constexpr HOURS_PER_DAY{24};
}

#endif //QSERIALTERMINAL_QSERIALTERMINALUTILITIES_H
