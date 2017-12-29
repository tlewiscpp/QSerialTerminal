/***********************************************************************
*    SerialPort.h:                                                     *
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
#include <unordered_set>


namespace CppSerialPort {

enum class FlowControl {
    FlowOff,
    FlowHardware,
    FlowXonXoff
};


#if defined(_WIN32)
#include <Windows.h>
using modem_status_t = DWORD;

enum class StopBits {
    StopOne     = ONESTOPBIT,
    StopOneFive = ONE5STOPBITS,
    StopTwo     = TWOSTOPBITS
};

enum class Parity : unsigned char {
    ParityNone  = NOPARITY,
    ParityOdd   = ODDPARITY,
    ParityEven  = EVENPARITY,
    ParityMark  = MARKPARITY,
    ParitySpace = SPACEPARITY
};

enum class DataBits {
    DataFive = 5,
    DataSix = 6,
    DataSeven = 7,
    DataEight = 8
};

enum class BaudRate {
    Baud110     = CBR_110,
    Baud300     = CBR_300,
    Baud600     = CBR_600,
    Baud1200    = CBR_1200,
    Baud2400    = CBR_2400,
    Baud4800    = CBR_4800,
    Baud9600    = CBR_9600,
    Baud19200   = CBR_19200,
    Baud38400   = CBR_38400,
    Baud57600   = CBR_57600,
    Baud115200  = CBR_115200,
    Baud128000  = CBR_128000,
    Baud256000  = CBR_256000
};

#else
using modem_status_t = int;
#include <termios.h>
enum class Parity {
    ParityEven,
    ParityOdd,
    ParityNone,
    ParitySpace
};
enum class StopBits {
    StopOne,
    StopTwo
};
enum class DataBits {
    DataFive = CS5,
    DataSix = CS6,
    DataSeven = CS7,
    DataEight = CS8
};
enum class BaudRate {
    Baud50 = B50,
    Baud75 = B75,
    Baud110 = B110,
    Baud134 = B134,
    Baud150 = B150,
    Baud200 = B200,
    Baud300 = B300,
    Baud600 = B600,
    Baud1200 = B1200,
    Baud1800 = B1800,
    Baud2400 = B2400,
    Baud4800 = B4800,
    Baud9600 = B9600,
    Baud19200 = B19200,
    Baud38400 = B38400,
    Baud57600 = B57600,
    Baud115200 = B115200,
    Baud230400 = B230400,
    Baud460800 = B460800,
    Baud500000 = B500000,
    Baud576000 = B576000,
    Baud921600 = B921600,
    Baud1000000 = B1000000,
    Baud1152000 = B1152000,
    Baud1500000 = B1500000,
    Baud2000000 = B2000000,
    Baud2500000 = B2500000,
    Baud3000000 = B3000000,
    Baud3500000 = B3500000,
    Baud4000000 = B4000000
};
#endif

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
	SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity, FlowControl flowControl);
	SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, DataBits dataBits, Parity parity, FlowControl flowControl);


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
	~SerialPort() override;

	void openPort() override;
    void closePort() override;
    char read() override;
    void setReadTimeout(int timeout) override;

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
    ssize_t write(char c) override;
	ssize_t write(const char *bytes, size_t numberOfBytes) override;

    void setBaudRate(BaudRate baudRate);
    void setStopBits(StopBits stopBits);
    void setParity(Parity parity);
    void setDataBits(DataBits dataBits);
    void setFlowControl(FlowControl flowControl);

    BaudRate baudRate() const;
    StopBits stopBits() const;
    DataBits dataBits() const;
    Parity parity() const;
    FlowControl flowControl() const;


    static const StopBits DEFAULT_STOP_BITS;
    static const Parity DEFAULT_PARITY;
    static const BaudRate DEFAULT_BAUD_RATE;
    static const DataBits DEFAULT_DATA_BITS;
    static const FlowControl DEFAULT_FLOW_CONTROL;
    static const std::string DEFAULT_LINE_ENDING;

    static std::unordered_set<std::string> availableSerialPorts();
    static bool isValidSerialPortName(const std::string &serialPortName);

    static const long DEFAULT_RETRY_COUNT;

private:
    std::string m_readBuffer;
    std::string m_portName;
    int m_portNumber;
    BaudRate m_baudRate;
    StopBits m_stopBits;
    DataBits m_dataBits;
    Parity m_parity;
    FlowControl m_flowControl;
    bool m_isOpen;

    static const long constexpr SERIAL_PORT_BUFFER_MAX{4096};
    static const long constexpr SINGLE_MESSAGE_BUFFER_MAX{4096};

    static bool isAvailableSerialPort(const std::string &name);
    static std::pair<int, std::string> getPortNameAndNumber(const std::string &name);
    static std::vector<std::string> generateSerialPortNames();

    static const std::vector<std::string> SERIAL_PORT_NAMES;
    void putBack(char c) override;

    int getFileDescriptor() const;

	static int getLastError();
	static std::string getErrorString(int errorCode);

#if (_WIN32)
	static const char *AVAILABLE_PORT_NAMES_BASE;
    static const char *DTR_RTS_ON_IDENTIFIER;
    static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256};
    static const char *SERIAL_PORT_REGISTRY_PATH;
    HANDLE m_serialPortHandle;
    COMMCONFIG m_portSettings;
#else
	FILE *m_fileStream;
	static const std::vector<const char *> AVAILABLE_PORT_NAMES_BASE;
    static const int constexpr NUMBER_OF_POSSIBLE_SERIAL_PORTS{256*9};
    termios m_portSettings;
    termios m_oldPortSettings;
#endif
    void applyPortSettings();
        modem_status_t getModemStatus() const; };

} //namespace CppSerialPort


#endif //CPPSERIALPORT_SERIALPORT_H
