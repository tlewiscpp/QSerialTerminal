/***********************************************************************
*    serialport.cpp:                                                   *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.serial/tlewiscpp/CppSerialPort                     *
*    This file may be distributed with the entire CppSerialPort library*
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SerialPort class          *
*    It is used to connect to RS232 compliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <chrono>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <future>
#include <deque>
#include <mutex>
#include <wchar.h>
#if (defined(_WIN32) || defined(__CYGWIN__))
    #include <Windows.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <limits.h>
    #include <sys/file.h>
    #include <errno.h>
#endif
#include <set>
#include <fcntl.h>

#include "SerialPort.h"

namespace CppSerialPort {

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::EIGHT};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::ONE};
const Parity SerialPort::DEFAULT_PARITY{Parity::NONE};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::BAUD115200};

#if defined(_WIN32) || defined(__CYGWIN__)
const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{R"(\\.\COM)"};
    const char *SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const char *SerialPort::SERIAL_PORT_REGISTRY_PATH{R"(HARDWARE\DEVICEMAP\SERIALCOMM\)"};
#else
const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"/dev/ttyS", "/dev/ttyACM", "/dev/ttyUSB",
                                                                      "/dev/ttyAMA", "/dev/ttyrfcomm", "/dev/ircomm",
                                                                      "/dev/cuau", "/dev/cuaU", "/dev/rfcomm"};
#endif

//#if defined(_WIN32) || defined(__CYGWIN__)
//    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"110", "300", "600", "1200", "2400", "4800",
//                                                                    "9600", "19200", "38400", "57600", "115200",
//                                                                    "128000", "256000", "500000", "1000000"};
//#else
//    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"50", "75", "110", "134", "150", "200", "300",
//                                                                    "600", "1200", "1800", "2400", "4800", "9600", "19200",
//                                                                    "38400", "57600", "115200", "230400", "460800", "500000",
//                                                                    "576000", "921600", "1000000", "1152000", "1500000",
//                                                                    "2000000", "2500000", "3000000", "3500000", "4000000"};
//#endif

const std::vector<std::string> SerialPort::SERIAL_PORT_NAMES{SerialPort::generateSerialPortNames()};

SerialPort::SerialPort(const std::string &name) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, dataBits, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits) :
        SerialPort(name, baudRate, stopBits, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, Parity parity) :
        SerialPort(name, baudRate, DEFAULT_STOP_BITS, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, Parity parity) :
        SerialPort(name, baudRate, stopBits, DEFAULT_DATA_BITS, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity) :
        SerialPort(name, baudRate, stopBits, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, dataBits, DEFAULT_PARITY)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, dataBits, DEFAULT_PARITY)

{
}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, StopBits stopBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, dataBits, parity)
{

}

SerialPort::SerialPort(const std::string &name, DataBits dataBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, dataBits, parity)
{
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, DEFAULT_DATA_BITS, DEFAULT_PARITY)
{
}

SerialPort::SerialPort(const std::string &name, StopBits stopBits, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, stopBits, DEFAULT_DATA_BITS, parity)
{
}

SerialPort::SerialPort(const std::string &name, Parity parity) :
        SerialPort(name, DEFAULT_BAUD_RATE, DEFAULT_STOP_BITS, DEFAULT_DATA_BITS, parity)
{

}

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, DataBits dataBits, Parity parity) :
        m_portName{name},
        m_portNumber{0},
        m_fileStream{nullptr},
        m_baudRate{baudRate},
        m_stopBits{stopBits},
        m_dataBits{dataBits},
        m_parity{parity},
        m_isOpen{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

int SerialPort::getFileDescriptor() const
{
#if defined(_WIN32) || defined(__CYGWIN__)
    //return fileno(this->m_fileStream);
    return this->m_fileDescriptor;
    //return _get_osfhandle(reinterpret_cast<intptr_t>(this->m_serialPortHandle));
#else
    return fileno(this->m_fileStream);
#endif
}

void SerialPort::openPort()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    std::string mode{""};
    mode += "baud=" + std::to_string(static_cast<int>(this->m_baudRate)) + " ";
    mode += "data=" + std::to_string(static_cast<int>(this->m_dataBits)) + " ";
    mode += "parity=" + std::string(1, static_cast<char>(parseParity(this->m_parity).first)) + " ";
    mode += "stop=" + std::to_string(static_cast<int>(this->m_stopBits)) + " ";
    mode += DTR_RTS_ON_IDENTIFIER;

    this->m_serialPortHandle = CreateFileA(this->m_portName.c_str(),
                                                  GENERIC_READ|GENERIC_WRITE,
                                                  0,                          /* no share  */
                                                  NULL,                       /* no security */
                                                  OPEN_EXISTING,
                                                  0,                          /* no threads */
                                                  NULL);                      /* no templates */

    if(this->m_serialPortHandle == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName);
    }

    DCB portSettings;
    memset(&portSettings, 0, sizeof(portSettings));
    portSettings.DCBlength = sizeof(portSettings);

    if(!BuildCommDCBA(mode.c_str(), &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set dcb settings for serial port " + this->m_portName);
    }

    if(!SetCommState(this->m_serialPortHandle, &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set cfg settings for serial port " + this->m_portName);
    }

    COMMTIMEOUTS Cptimeouts;
    Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
    Cptimeouts.ReadTotalTimeoutConstant    = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant   = 0;

    if(!SetCommTimeouts(this->m_serialPortHandle, &Cptimeouts)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set timeout settings for serial port " + this->m_portName);
    }
    this->m_fileDescriptor = _open_osfhandle(reinterpret_cast<intptr_t>(this->m_serialPortHandle), 0);
    if (this->m_fileDescriptor == -1) {
        throw std::runtime_error("ERROR: Unable to get raw file descriptor from HANDLE*: _open_osfhandle returned " + toStdString(this->m_fileDescriptor));
    }
    /*
    this->m_fileStream = _fdopen(fileDescriptor, "rw");
    if (this->m_fileStream == nullptr) {
        throw std::runtime_error("ERROR: Unable to get FILE* fileDescriptor (which is " + toStdString(fileDescriptor) + ")");
    }
    */
    this->m_isOpen = true;
#else

    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    int status{0};
    int error{0};
    int baudRate{static_cast<int>(this->m_baudRate)};
    int cbits{static_cast<int>(this->m_dataBits)};
    std::pair<int, int> parityPair{parseParity(this->m_parity)};
    int cpar{parityPair.first};
    int ipar{parityPair.second};
    int bstop{static_cast<int>(this->m_stopBits)};
    /*
    this->m_serialPortHandle = open(this->m_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(this->m_serialPortHandle == -1) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName + " (is something else using it?)");
    }
    this->m_fileStream = fdopen(this->m_serialPortHandle, "rw");
    if (!this->m_fileStream) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to open serial port file stream for serial port" + this->m_portName);
    }
    */
    //this->m_fileStream = fdopen(this->m_serialPortHandle, "rw");
    this->m_fileStream = fopen(this->m_portName.c_str(), "r+");
    if (!this->m_fileStream) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to open serial port file stream for serial port" + this->m_portName);
    }

    if(flock(this->getFileDescriptor(), LOCK_EX | LOCK_NB) != 0) {
        this->closePort();
        throw std::runtime_error("ERROR: Another process has locked serial port " + this->m_portName);
    }

    error = tcgetattr(this->getFileDescriptor(), this->m_oldPortSettings + this->m_portNumber);
    if (error == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to read port settings for serial port " + this->m_portName);
    }
    memset(&this->m_newPortSettings, 0, sizeof(this->m_newPortSettings));

    this->m_newPortSettings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD;
    this->m_newPortSettings.c_iflag = ipar;
    this->m_newPortSettings.c_oflag = 0;
    this->m_newPortSettings.c_lflag = 0;
    this->m_newPortSettings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    this->m_newPortSettings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */

    cfsetispeed(&this->m_newPortSettings, baudRate);
    cfsetospeed(&this->m_newPortSettings, baudRate);

    error = tcsetattr(this->getFileDescriptor(), TCSANOW, &this->m_newPortSettings);
    if(error == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to adjust port settings for serial port " + this->m_portName);
    }

    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to get port status for serial port " + this->m_portName);
    }

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set port status for serial port " + this->m_portName);
    }

    this->m_isOpen = true;
#endif
}


std::pair<int, int> SerialPort::parseParity(Parity parity)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (parity == Parity::NONE) {
        return std::make_pair('n', 0);
    } else if (parity == Parity::EVEN) {
        return std::make_pair('e', 0);
    } else if (parity == Parity::ODD) {
        return std::make_pair('o', 0);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#else
    if (parity == Parity::NONE) {
        return std::make_pair(0, IGNPAR);
    } else if (parity == Parity::EVEN) {
        return std::make_pair(PARENB, INPCK);
    } else if (parity == Parity::ODD) {
        return std::make_pair( (PARENB | PARODD), INPCK);
    } else {
        return parseParity(DEFAULT_PARITY);
    }
    return parseParity(DEFAULT_PARITY);
#endif
}

int SerialPort::read()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    /*
    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }
    static char readStuff[SERIAL_PORT_BUFFER_MAX];
    memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

    DWORD readBytes{0};
    auto readResult = ReadFile(this->m_serialPortHandle, readStuff, SERIAL_PORT_BUFFER_MAX - 1, &readBytes, NULL);
    if ( (readBytes <= 0) || (readResult == false) ) {
        return 0;
    }
    this->m_readBuffer += std::string{readStuff};
    char returnValue{this->m_readBuffer.front()};
    this->m_readBuffer = this->m_readBuffer.substr(1);
    return static_cast<int>(returnValue);
    */

    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }

    // Initialize file descriptor sets
    fd_set read_fds{0, 0, 0};
    fd_set write_fds{0, 0, 0};
    fd_set except_fds{0, 0, 0};
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(this->getFileDescriptor(), &read_fds);

    struct timeval timeout{0, 0};
    timeout.tv_sec = 0;
    timeout.tv_usec = (this->readTimeout() * 1000);
    static char readStuff[SERIAL_PORT_BUFFER_MAX];
    memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

    (void)timeout;
    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    //if (select(this->getFileDescriptor() + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
        DWORD readBytes{0};
        auto readResult = ReadFile(this->m_serialPortHandle, readStuff, SERIAL_PORT_BUFFER_MAX - 1, &readBytes, NULL);
        if ( (readBytes <= 0) || (readResult <= 0) ) {
            return 0;
        }
        this->m_readBuffer += std::string{readStuff};
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    //}
    return 0;

#else
    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }

    // Initialize file descriptor sets
    fd_set read_fds{0, 0, 0};
    fd_set write_fds{0, 0, 0};
    fd_set except_fds{0, 0, 0};
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(this->getFileDescriptor(), &read_fds);

    struct timeval timeout{0, 0};
    timeout.tv_sec = 0;
    timeout.tv_usec = (this->readTimeout() * 1000);
    static char readStuff[SERIAL_PORT_BUFFER_MAX];
    memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

    // Wait for input to become ready or until the time out; the first parameter is
    // 1 more than the largest file descriptor in any of the sets
    if (select(this->getFileDescriptor() + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
        auto returnedBytes = fread(readStuff, sizeof(char), SERIAL_PORT_BUFFER_MAX, this->m_fileStream);
        this->m_readBuffer += std::string{readStuff};
        //auto returnedBytes = fread(&readValue, sizeof(char), 1, this->m_fileStream);
        if ((returnedBytes <= 0) || (readStuff[0] == '\0')) {
            return 0;
        }
        char returnValue{this->m_readBuffer.front()};
        this->m_readBuffer = this->m_readBuffer.substr(1);
        return static_cast<int>(returnValue);
    }
#endif
    return 0;
}

ssize_t SerialPort::write(int byteToSend)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    /*
    DWORD writtenBytes{0};
    WriteFile(this->m_serialPortHandle, &byteToSend, 1, &writtenBytes, NULL);
    return ( (writtenBytes == 0) ? 1 : 0);
    */
    auto writtenBytes = ::write(this->getFileDescriptor(), &byteToSend, 1);
    //auto writtenBytes = fwrite(&byteToSend, sizeof(char), 1, this->m_fileStream);
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#else
    auto writtenBytes = ::write(this->getFileDescriptor(), &byteToSend, 1);
    //auto writtenBytes = fwrite(&byteToSend, sizeof(char), 1, this->m_fileStream);
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#endif
}

void SerialPort::closePort()
{
    if (!this->isOpen()) {
        return;
    }
#if (defined(_WIN32) || defined(__CYGWIN__))
    CloseHandle(this->m_serialPortHandle);
    this->m_isOpen = false;
#else
    int status{0};
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_DTR;    /* turn off DTR */
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
    }

    tcsetattr(this->getFileDescriptor(), TCSANOW, this->m_oldPortSettings + this->m_portNumber);
    //close(this->getFileDescriptor());
    flock(this->getFileDescriptor(), LOCK_UN);
    fclose(this->m_fileStream);
    this->m_isOpen = false;
#endif
}


void SerialPort::enableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPortHandle, SETDTR);
#else
    int status{0};
    status |= TIOCM_DTR;    /* turn on DTR */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPortHandle, CLRDTR);
#else
    int status{0};
    status &= ~TIOCM_DTR;    /* turn off DTR */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::enableRTS()
{

#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPortHandle, SETRTS);
#else
    int status{0};
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableRTS()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPortHandle, CLRRTS);
#else
    int status;
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

bool SerialPort::isDCDEnabled() const
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, (LPDWORD)((void *)&status));
        return ((status&MS_RLSD_ON) != 0 ? true : false);
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_CAR);
#endif
}


bool SerialPort::isCTSEnabled() const
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, (LPDWORD)((void *)&status));
        return ((status&MS_CTS_ON) != 0 ? true : false);
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_CTS);
#endif
}

bool SerialPort::isDSREnabled() const
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    int status{0};
        GetCommModemStatus(this->m_serialPortHandle, (LPDWORD)((void *)&status));
        return ((status&MS_DSR_ON) != 0 ? true : false);
#else
    int status{0};
    ioctl(this->getFileDescriptor(), TIOCMGET, &status);
    return (status&TIOCM_DSR);
#endif
}

void SerialPort::flushRx()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    PurgeComm(this->m_serialPortHandle, PURGE_RXCLEAR | PURGE_RXABORT);
#else
    tcflush(this->getFileDescriptor(), TCIFLUSH);
#endif
    int readValue{0};
    do {
        readValue = this->read();
    } while (readValue != 0);
}


void SerialPort::flushTx()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    PurgeComm(this->m_serialPortHandle, PURGE_TXCLEAR | PURGE_TXABORT);
#else
    tcflush(this->getFileDescriptor(), TCOFLUSH);
#endif
}

bool SerialPort::isAvailableSerialPort(const std::string &name)
{
    for (auto &it : SerialPort::availableSerialPorts()) {
        if (name == it) {
            return true;
        }
    }
    return false;
}

std::pair<int, std::string> SerialPort::getPortNameAndNumber(const std::string &name)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    std::string str{name};
    int i{0};
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (it.find(str) != std::string::npos) {
            return std::make_pair(i, str);
        }
        i++;
    }
    throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
#else
    std::string str{name};
    auto iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
//    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
//        if (str == it) {
//            return std::make_pair(i, str);
//        }
//        i++;
//    }
    str = name;
    if (str.find("/dev/tty") == std::string::npos) {
        str = "/dev/tty" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
//    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
//        if (str == it) {
//            return std::make_pair(i, str);
//        }
//        i++;
//    }
    str = name;
    if (str.find("/dev/") == std::string::npos) {
        str = "/dev/" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
//    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
//        if (str == it) {
//            return std::make_pair(i, str);
//        }
//        i++;
//    }

    throw std::runtime_error(vaToStdString("ERROR: ", name, " is an invalid serial port name"));
#endif
}

bool SerialPort::isOpen() const
{
    return this->m_isOpen;
}

void SerialPort::setDataBits(DataBits dataBits)
{
    if (!this->m_isOpen) {
        this->m_dataBits = dataBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change data bits while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setBaudRate(BaudRate baudRate)
{
    if (!this->m_isOpen) {
        this->m_baudRate = baudRate;
    } else {
        throw std::runtime_error("ERROR: Cannot change baud rate while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setStopBits(StopBits stopBits)
{
    if (!this->m_isOpen) {
        this->m_stopBits = stopBits;
    } else {
        throw std::runtime_error("ERROR: Cannot change stop bits while serial port " + this->m_portName + " is open");
    }
}

void SerialPort::setParity(Parity parity)
{
    if (!this->m_isOpen) {
        this->m_parity = parity;
    } else {
        throw std::runtime_error("ERROR: Cannot change parity while serial port " + this->m_portName + " is open");
    }
}

BaudRate SerialPort::baudRate() const
{
    return this->m_baudRate;
}

StopBits SerialPort::stopBits() const
{
    return this->m_stopBits;
}

DataBits SerialPort::dataBits() const
{
    return this->m_dataBits;
}

Parity SerialPort::parity() const
{
    return this->m_parity;
}

std::string SerialPort::portName() const
{
    return this->m_portName;
}


BaudRate SerialPort::parseBaudRateFromRaw(const std::string &baudRate)
{
    std::string copyString{baudRate};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600;
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200;
    } else if (copyString == "128000") {
        return BaudRate::BAUD128000;
    } else if (copyString == "256000") {
        return BaudRate::BAUD256000;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#else
    if (copyString == "50") {
        return BaudRate::BAUD50;
    } else if (copyString == "75") {
        return BaudRate::BAUD75;
    } else if (copyString == "110") {
        return BaudRate::BAUD110;
    } else if (copyString == "134") {
        return BaudRate::BAUD134;
    } else if (copyString == "150") {
        return BaudRate::BAUD150;
    } else if (copyString == "200") {
        return BaudRate::BAUD200;
    } else if (copyString == "300") {
        return BaudRate::BAUD300;
    } else if (copyString == "600") {
        return BaudRate::BAUD600;
    } else if (copyString == "1200") {
        return BaudRate::BAUD1200;
    } else if (copyString == "1800") {
        return BaudRate::BAUD1800;
    } else if (copyString == "2400") {
        return BaudRate::BAUD2400;
    } else if (copyString == "4800") {
        return BaudRate::BAUD4800;
    } else if (copyString == "9600") {
        return BaudRate::BAUD9600;
    } else if (copyString == "19200") {
        return BaudRate::BAUD19200;
    } else if (copyString == "38400") {
        return BaudRate::BAUD38400;
    } else if (copyString == "57600") {
        return BaudRate::BAUD57600;
    } else if (copyString == "115200") {
        return BaudRate::BAUD115200;
    } else if (copyString == "230400") {
        return BaudRate::BAUD230400;
    } else if (copyString == "460800") {
        return BaudRate::BAUD460800;
    } else if (copyString == "500000") {
        return BaudRate::BAUD500000;
    } else if (copyString == "576000") {
        return BaudRate::BAUD576000;
    } else if (copyString == "921600") {
        return BaudRate::BAUD921600;
    } else if (copyString == "1000000") {
        return BaudRate::BAUD1000000;
    } else if (copyString == "1152000") {
        return BaudRate::BAUD1152000;
    } else if (copyString == "1500000") {
        return BaudRate::BAUD1500000;
    } else if (copyString == "2000000") {
        return BaudRate::BAUD2000000;
    } else if (copyString == "2500000") {
        return BaudRate::BAUD2500000;
    } else if (copyString == "3000000") {
        return BaudRate::BAUD3000000;
    } else if (copyString == "3500000") {
        return BaudRate::BAUD3500000;
    } else if (copyString == "4000000") {
        return BaudRate::BAUD4000000;
    } else {
        throw std::runtime_error("Invalid baud rate passed to parseBaudRateFromRaw(const std::string &): " + baudRate);
    }
#endif
}

DataBits SerialPort::parseDataBitsFromRaw(const std::string &dataBits)
{
    std::string copyString{dataBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "eight") || (copyString == "8")) {
        return DataBits::EIGHT;
    } else if ((copyString == "seven") || (copyString == "7")) {
        return DataBits::SEVEN;
    } else if ((copyString == "six") || (copyString == "6")) {
        return DataBits::SIX;
    } else if ((copyString == "five") || (copyString == "5")) {
        return DataBits::FIVE;
    } else {
        throw std::runtime_error("Invalid data bits passed to parseDataBitsFromRaw(const std::string &): " + dataBits);
    }
}

StopBits SerialPort::parseStopBitsFromRaw(const std::string &stopBits)
{
    std::string copyString{stopBits};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "one") || (copyString == "1")) {
        return StopBits::ONE;
    } else if ((copyString == "two" || copyString == "2")) {
        return StopBits::TWO;
    } else {
        throw std::runtime_error("Invalid stop bits passed to parseStopBitsFromRaw(const std::string &): " + stopBits);
    }
}

Parity SerialPort::parseParityFromRaw(const std::string &parity)
{
    std::string copyString{parity};
    std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
    if ((copyString == "none") || (copyString == "n")) {
        return Parity::NONE;
    } else if ((copyString == "even") || (copyString == "e")) {
        return Parity::EVEN;
    } else if ((copyString == "odd") || (copyString == "o")) {
        return Parity::ODD;
    } else {
        throw std::runtime_error("Invalid parity passed to parseParityFromRaw(const std::string &): " + parity);
    }
}

std::string SerialPort::baudRateToString(BaudRate baudRate)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600";
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200";
    } else if (baudRate == BaudRate::BAUD128000) {
        return "128000";
    } else if (baudRate == BaudRate::BAUD256000) {
        return "256000";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else {
        return "";
    }
#else
    if (baudRate == BaudRate::BAUD50) {
        return "50";
    } else if (baudRate == BaudRate::BAUD75) {
        return "75";
    } else if (baudRate == BaudRate::BAUD110) {
        return "110";
    } else if (baudRate == BaudRate::BAUD134) {
        return "134";
    } else if (baudRate == BaudRate::BAUD150) {
        return "150";
    } else if (baudRate == BaudRate::BAUD200) {
        return "200";
    } else if (baudRate == BaudRate::BAUD300) {
        return "300";
    } else if (baudRate == BaudRate::BAUD600) {
        return "600";
    } else if (baudRate == BaudRate::BAUD1200) {
        return "1200";
    } else if (baudRate == BaudRate::BAUD1800) {
        return "1800";
    } else if (baudRate == BaudRate::BAUD2400) {
        return "2400";
    } else if (baudRate == BaudRate::BAUD4800) {
        return "4800";
    } else if (baudRate == BaudRate::BAUD9600) {
        return "9600";
    } else if (baudRate == BaudRate::BAUD19200) {
        return "19200";
    } else if (baudRate == BaudRate::BAUD38400) {
        return "38400";
    } else if (baudRate == BaudRate::BAUD57600) {
        return "57600";
    } else if (baudRate == BaudRate::BAUD115200) {
        return "115200";
    } else if (baudRate == BaudRate::BAUD230400) {
        return "230400";
    } else if (baudRate == BaudRate::BAUD460800) {
        return "460800";
    } else if (baudRate == BaudRate::BAUD500000) {
        return "500000";
    } else if (baudRate == BaudRate::BAUD576000) {
        return "576000";
    } else if (baudRate == BaudRate::BAUD921600) {
        return "921600";
    } else if (baudRate == BaudRate::BAUD1000000) {
        return "1000000";
    } else if (baudRate == BaudRate::BAUD1152000) {
        return "1152000";
    } else if (baudRate == BaudRate::BAUD1500000) {
        return "1500000";
    } else if (baudRate == BaudRate::BAUD2000000) {
        return "2000000";
    } else if (baudRate == BaudRate::BAUD2500000) {
        return "2500000";
    } else if (baudRate == BaudRate::BAUD3000000) {
        return "3000000";
    } else if (baudRate == BaudRate::BAUD3500000) {
        return "3500000";
    } else if (baudRate == BaudRate::BAUD4000000) {
        return "4000000";
    } else {
        return "";
    }
#endif
}
std::string SerialPort::stopBitsToString(StopBits stopBits)
{
    if (stopBits == StopBits::ONE) {
        return "1";
    } else if (stopBits == StopBits::TWO) {
        return "2";
    } else {
        return "";
    }
}

std::string SerialPort::dataBitsToString(DataBits dataBits)
{
    if (dataBits == DataBits::EIGHT) {
        return "8";
    } else if (dataBits == DataBits::SEVEN) {
        return "7";
    } else if (dataBits == DataBits::SIX) {
        return "6";
    } else if (dataBits == DataBits::FIVE) {
        return "5";
    } else {
        return "";
    }
}

std::string SerialPort::parityToString(Parity parity)
{
    if (parity == Parity::EVEN) {
        return "Even";
    } else if (parity == Parity::ODD) {
        return "Odd";
    } else if (parity == Parity::NONE) {
        return "None";
    } else {
        return "";
    }
}

std::vector<std::string> SerialPort::availableSerialPorts()
{
    std::vector<std::string> returnVector;
#if (defined(_WIN32) || defined(__CYGWIN__))
    try {
        HKEY hRegistryKey;
        LONG operationResult{ RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, 0, KEY_READ, &hRegistryKey) };
        if (operationResult != ERROR_SUCCESS) {
            throw std::runtime_error("ERROR: Could not open registry path " + std::string{SERIAL_PORT_REGISTRY_PATH} + " for reading values");
        }
        // error checking by testing res omitted
        for (DWORD index = 0; ; index++) {
            char SubKeyName[PATH_MAX];
            DWORD cName{ PATH_MAX };
            DWORD cbData{ PATH_MAX };
            char hRegistryKeyValue[PATH_MAX];
            operationResult = RegEnumValueA(hRegistryKey, index, SubKeyName, &cName, NULL, NULL, NULL, NULL);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            operationResult = RegGetValueA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, SubKeyName, RRF_RT_REG_SZ, NULL, hRegistryKeyValue, &cbData);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            returnVector.emplace_back(hRegistryKeyValue);
        }
        RegCloseKey(hRegistryKey);
        std::set<std::string> uniques;
        for (auto &it : returnVector) {
            uniques.emplace(it);
        }
        std::vector<std::string> realReturn;
        for (auto &it : uniques) {
            realReturn.emplace_back(it);
        }
        return realReturn;
    } catch (std::exception &e) {
        (void)e;
        return returnVector;
    }
#else
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (IByteStream::fileExists(std::string{it})) {
            returnVector.emplace_back(it);
        }
    }
    std::set<std::string> uniques;
    for (auto &it : returnVector) {
        uniques.emplace(it);
    }
    std::vector<std::string> realReturn;
    for (auto &it : uniques) {
        realReturn.emplace_back(it);
    }
    return realReturn;
#endif
}

std::vector<std::string> SerialPort::generateSerialPortNames()
{
    std::vector<std::string> returnVector;
    for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
        for (int i = 0; i < 256; i++) {
            returnVector.push_back(it + toStdString(i));
        }
    }
    return returnVector;
}

bool SerialPort::isValidSerialPortName(const std::string &serialPortName)
{
#if defined(_WIN32) || defined(__CYGWIN__)
    for (int i = 0; i < 256; i++) {
            if (serialPortName == ("COM" + std::to_string(i))) {
                return true;
            }
        }
        return false;
#else
    for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
        for (int i = 0; i < 256; i++) {
            if (serialPortName == (it + toStdString(i))) {
                return true;
            }
        }
    }
    return false;
#endif
}

void SerialPort::putBack(int c)
{
    if (this->m_readBuffer.length() > 0) {
        this->m_readBuffer = static_cast<char>(c) + this->m_readBuffer;
    } else {
        ungetc(c, this->m_fileStream);
    }
}


} //namespace CppSerialPort
