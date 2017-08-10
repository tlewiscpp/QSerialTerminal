/***********************************************************************
*    serialport.h:                                                     *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.com/tlewiscpp/CppSerialPort                        *
*    This file may be distributed with the CppSerialPort library,      *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a SerialPort class            *
*    It is used to connect to RS232 compliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef CPPSERIALPORT_SERIALPORT_H
#define CPPSERIALPORT_SERIALPORT_H

#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
#include <termios.h>
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif


#include <string>
#include <vector>
#include <sstream>

#include "IByteStream.h"

namespace CppSerialPort {

enum class StopBits { ONE, TWO };
enum class DataBits { FIVE, SIX, SEVEN, EIGHT };
enum class Parity { EVEN, ODD, NONE };

#if (defined(_WIN32) || defined(__CYGWIN__))
enum class BaudRate { BAUD110, BAUD300, BAUD600, BAUD1200,
                      BAUD2400, BAUD4800, BAUD9600, BAUD19200,
                      BAUD38400, BAUD57600, BAUD115200,
                      BAUD128000, BAUD256000, BAUD500000, BAUD1000000 };
#else
enum class BaudRate { BAUD50, BAUD75, BAUD110, BAUD134, BAUD150,
    BAUD200, BAUD300, BAUD600, BAUD1200, BAUD1800,
    BAUD2400, BAUD4800, BAUD9600, BAUD19200, BAUD38400,
    BAUD57600, BAUD115200, BAUD230400, BAUD460800,
    BAUD500000, BAUD576000, BAUD921600, BAUD1000000,
    BAUD1152000, BAUD1500000, BAUD2000000, BAUD2500000,
    BAUD3000000, BAUD3500000, BAUD4000000 };
#endif

class SerialPort : public IByteStream
{
public:
    SerialPort(const std::string &name);
    SerialPort(const std::string &name, BaudRate baudRate);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits);
    SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, Parity parity);
    SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, DataBits dataBits, Parity parity);

    SerialPort(const std::string &name, DataBits dataBits);
    SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits);
    SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, DataBits dataBits, Parity parity);
    SerialPort(const std::string &name, StopBits stopBits);
    SerialPort(const std::string &name, StopBits stopBits, Parity parity);
    SerialPort(const std::string &name, Parity parity);

    SerialPort(SerialPort &&other);

    friend bool operator==(const SerialPort &lhs, const SerialPort &rhs);
    SerialPort &operator=(const SerialPort &rhs) = delete;
    SerialPort(const SerialPort &other) = delete;

    void openPort() override;
    void closePort() override;
    int read() override;

    std::string readLine(bool *timeout) override;

    BaudRate parseBaudRateFromRaw(const std::string &baudRate);

    std::string baudRateToString(BaudRate baudRate);

    std::string stopBitsToString(StopBits stopBits);

    std::string dataBitsToString(DataBits dataBits);

    std::string parityToString(Parity parity);

public:
    std::string portName() const override;
    bool isOpen() const override;

    bool isDCDEnabled() const;
    bool isCTSEnabled() const;
    bool isDSREnabled() const;
    void enableDTR();
    void disableDTR();
    void enableRTS();
    void disableRTS();
    void flushRX();
    void flushTX();

    void setBaudRate(BaudRate baudRate);
    void setStopBits(StopBits stopBits);
    void setParity(Parity parity);
    void setDataBits(DataBits dataBits);

    BaudRate baudRate() const;
    StopBits stopBits() const;
    DataBits dataBits() const;
    Parity parity() const;

    static const DataBits DEFAULT_DATA_BITS;
    static const StopBits DEFAULT_STOP_BITS;
    static const Parity DEFAULT_PARITY;
    static const BaudRate DEFAULT_BAUD_RATE;
    static const std::string DEFAULT_LINE_ENDING;

    static std::vector<std::string> availableSerialPorts();
    static bool isValidSerialPortName(const std::string &serialPortName);

    static const long DEFAULT_RETRY_COUNT;

private:
#if (defined(_WIN32) || defined(__CYGWIN__))
    static const char *DTR_RTS_ON_IDENTIFIER;
        static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256};
        static const char *SERIAL_PORT_REGISTRY_PATH;
        HANDLE m_serialPort[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
#else
    static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256*9};
    int m_serialPort[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
    struct termios m_oldPortSettings[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
    struct termios m_newPortSettings;
#endif
    std::string m_portName;
    int m_portNumber;
    FILE *m_fileStream;
    BaudRate m_baudRate;
    StopBits m_stopBits;
    DataBits m_dataBits;
    Parity m_parity;
    bool m_isOpen;

    ssize_t write(int byteToWrite) override;

    static const long constexpr SERIAL_PORT_BUFFER_MAX{4096};
    static const long constexpr SINGLE_MESSAGE_BUFFER_MAX{4096};

    static bool isAvailableSerialPort(const std::string &name);
    static std::pair<int, std::string> getPortNameAndNumber(const std::string &name);
    static std::vector<std::string> generateSerialPortNames();

    static const std::vector<const char *> AVAILABLE_PORT_NAMES_BASE;
    static const std::vector<std::string> SERIAL_PORT_NAMES;

    int parseDataBits(DataBits dataBits);
    int parseStopBits(StopBits stopBits);
    std::pair<int, int> parseParity(Parity parity);
    static int parseBaudRate(BaudRate baudRate);

    static DataBits parseDataBitsFromRaw(const std::string &dataBits);
    static StopBits parseStopBitsFromRaw(const std::string &stopBits);
    static Parity parseParityFromRaw(const std::string &parity);

    void putBack(int c);
};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_SERIALPORT_H
