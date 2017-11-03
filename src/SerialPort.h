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

#include <string>
#include <vector>
#include <sstream>

#include "IByteStream.h"

namespace CppSerialPort {

enum class Parity { EVEN, ODD, NONE };

#if (defined(_WIN32) || defined(__CYGWIN__))
enum class StopBits {
    ONE = 1,
    TWO = 2
};
enum class DataBits {
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8
};
enum class BaudRate {
    BAUD110 = 110,
    BAUD300 = 300,
    BAUD600 = 600,
    BAUD1200 = 1200,
    BAUD2400 = 2400,
    BAUD4800 = 4800,
    BAUD9600 = 9600,
    BAUD19200 = 19200,
    BAUD38400 = 38400,
    BAUD57600 = 57600,
    BAUD115200 = 115200,
    BAUD128000 = 128000,
    BAUD256000 = 256000,
    BAUD500000 = 500000,
    BAUD1000000 = 1000000
};
#else
#include <termios.h>

enum class StopBits {
    ONE = 0,
    TWO = CSTOPB
};
enum class DataBits {
    FIVE = CS5,
    SIX = CS6,
    SEVEN = CS7,
    EIGHT = CS8
};
enum class BaudRate {
    BAUD50 = B50,
    BAUD75 = B75,
    BAUD110 = B110,
    BAUD134 = B134,
    BAUD150 = B150,
    BAUD200 = B200,
    BAUD300 = B300,
    BAUD600 = B600,
    BAUD1200 = B1200,
    BAUD1800 = B1800,
    BAUD2400 = B2400,
    BAUD4800 = B4800,
    BAUD9600 = B9600,
    BAUD19200 = B19200,
    BAUD38400 = B38400,
    BAUD57600 = B57600,
    BAUD115200 = B115200,
    BAUD230400 = B230400,
    BAUD460800 = B460800,
    BAUD500000 = B500000,
    BAUD576000 = B576000,
    BAUD921600 = B921600,
    BAUD1000000 = B1000000,
    BAUD1152000 = B1152000,
    BAUD1500000 = B1500000,
    BAUD2000000 = B2000000,
    BAUD2500000 = B2500000,
    BAUD3000000 = B3000000,
    BAUD3500000 = B3500000,
    BAUD4000000 = B4000000
};
#endif

    using HANDLE = void *;

class SerialPort : public IByteStream
{
public:
    explicit SerialPort(const std::string &name);
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

    friend inline bool operator==(const SerialPort &lhs, const SerialPort &rhs) {
        (void)lhs;
        (void)rhs;
        return false;
    }

    SerialPort(SerialPort &&other) = delete;
    SerialPort &operator=(const SerialPort &rhs) = delete;
    SerialPort &operator=(SerialPort &&rhs) = delete;
    SerialPort(const SerialPort &other) = delete;

    void openPort() override;
    void closePort() override;
    int read() override;
    static BaudRate parseBaudRateFromRaw(const std::string &baudRate);
    static DataBits parseDataBitsFromRaw(const std::string &dataBits);
    static StopBits parseStopBitsFromRaw(const std::string &stopBits);
    static Parity parseParityFromRaw(const std::string &parity);

    static std::string baudRateToString(BaudRate baudRate);
    static std::string stopBitsToString(StopBits stopBits);
    static std::string dataBitsToString(DataBits dataBits);
    static std::string parityToString(Parity parity);

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
    void flushRx() override;
    void flushTx() override;

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
    std::string m_readBuffer;
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

    std::pair<int, int> parseParity(Parity parity);

    void putBack(int c) override;

    int getFileDescriptor() const;

#if (defined(_WIN32) || defined(__CYGWIN__))
    static const char *DTR_RTS_ON_IDENTIFIER;
        static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256};
        static const char *SERIAL_PORT_REGISTRY_PATH;
        HANDLE m_serialPortHandle;
        int m_fileDescriptor;
#else
    static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256*9};
    int m_serialPortPool[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
    termios m_newPortSettings;
    termios m_oldPortSettings[NUMBER_OF_POSSIBLE_SERIAL_PORTS];
#endif

};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_SERIALPORT_H
