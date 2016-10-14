#include "qserialterminalutilities.h"

namespace QSerialTerminalUtilities
{
    void logString(const std::string &str) { std::cout << str << std::endl; }
    std::string toString(const std::string &str) { return str; }
    std::string toString(const char *str) { return static_cast<std::string>(str); }

    QString toQString(const std::string &convert) { return QString::fromStdString(convert); }
    QString toQString(const char *convert) { return QString::fromStdString(static_cast<std::string>(convert)); }
    QString toQString(const QString &convert) { return convert; }

    int roundIntuitively(double numberToRound)
    {
        double tempContainer{numberToRound - static_cast<int>(numberToRound)};
        if (tempContainer >= 0.5) {
            return (static_cast<int>(numberToRound) + 1);
        } else {
            return static_cast<int>(numberToRound);
        }
    }

    int randomBetween(int lowLimit, int highLimit)
    {
        static bool srandSeeded{false};
        if (!srandSeeded) {
            srand(time(NULL));
            srandSeeded = true;
        }
        const int divisor{(RAND_MAX/highLimit) + 1};
        int returnValue{lowLimit - 1};
        do {
            returnValue = (rand() / divisor);
        } while ((returnValue > highLimit) || (returnValue < lowLimit));
        return returnValue;
    }
}
