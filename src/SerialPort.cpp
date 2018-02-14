/***********************************************************************
*    SerialPort.cpp:                                                   *
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
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <cstdio>
#include <cstring>
#include <utility>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <set>
#include <climits>

#if defined(_WIN32)
#    include <Windows.h>
#    include <io.h>
#    include <Fcntl.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <climits>
    #include <sys/file.h>
    #include <cerrno>
    #include <sys/signal.h>
#endif

#include "SerialPort.h"
#include <iostream>
#include <limits>

namespace CppSerialPort {

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::DataEight};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::StopOne};
const Parity SerialPort::DEFAULT_PARITY{Parity::ParityNone};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::Baud9600};
const FlowControl SerialPort::DEFAULT_FLOW_CONTROL{FlowControl::FlowOff};

#if defined(_WIN32)
    const char *SerialPort::AVAILABLE_PORT_NAMES_BASE{R"(\\.\COM)"};
    const char *SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const char *SerialPort::SERIAL_PORT_REGISTRY_PATH{R"(HARDWARE\DEVICEMAP\SERIALCOMM\)"};
    const std::string SerialPort::DEFAULT_LINE_ENDING{"\r\n"};
#else
const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"/dev/ttyS", "/dev/ttyACM", "/dev/ttyUSB",
                                                                      "/dev/ttyAMA", "/dev/ttyrfcomm", "/dev/ircomm",
                                                                      "/dev/cuau", "/dev/cuaU", "/dev/rfcomm"};
const std::string SerialPort::DEFAULT_LINE_ENDING{"\n"};

#endif

const std::vector<std::string> SerialPort::SERIAL_PORT_NAMES{SerialPort::generateSerialPortNames()};

SerialPort::SerialPort(const std::string &name, BaudRate baudRate, DataBits dataBits, StopBits stopBits, Parity parity, FlowControl flowControl, const std::string &lineEnding) :
        m_portName{name},
        m_portNumber{0},
        m_baudRate{baudRate},
        m_stopBits{stopBits},
        m_dataBits{dataBits},
        m_parity{parity},
        m_flowControl{flowControl},
        m_isOpen{false}
{
    this->setLineEnding(lineEnding);
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
#if defined(_WIN32)
    this->m_serialPortHandle = INVALID_HANDLE_VALUE;
#else
    this->m_fileStream = nullptr;
#endif //defined(_WIN32)
}


#if !defined(_WIN32)
int SerialPort::getFileDescriptor() const
{
    return fileno(this->m_fileStream);
}
#endif //!defined(_WIN32)


void SerialPort::openPort()
{
	if (!isAvailableSerialPort(this->portName())) {
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): ERROR: " + this->portName() + " is not a currently available serial port (is something else using it?)");
	}
#if defined(_WIN32)

    this->m_serialPortHandle = CreateFileA(this->m_portName.c_str(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if(this->m_serialPortHandle == INVALID_HANDLE_VALUE) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): CreateFileA(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE, HANDLE): Unable to open serial port " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
	}

    //Get full configuration)
	if (!GetCommConfig(this->m_serialPortHandle, &this->m_portSettings, &this->m_portSettings.dwSize)) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): GetCommConfig(HANDLE, LPCOMMCONFIG, LPDWORD): Unable to get current communication configuration for  " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
	}

    //Get DCB settings
	if (!GetCommState(this->m_serialPortHandle, &(this->m_portSettings.dcb))) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): GetCommState(HANDLE, LPDCB): Unable to get current communication state for  " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
	}

    /*set up parameters*/
    this->m_portSettings.dcb.fBinary=TRUE;
    this->m_portSettings.dcb.fInX=FALSE;
    this->m_portSettings.dcb.fOutX=FALSE;
    this->m_portSettings.dcb.fAbortOnError=FALSE;
    this->m_portSettings.dcb.fNull=FALSE;
#else
    this->m_fileStream = fopen(this->portName().c_str(), "r+");
    if (!this->m_fileStream) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): fopen(const char *, const char *): Unable to open FILE pointer for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    if(flock(this->getFileDescriptor(), LOCK_EX | LOCK_NB) != 0) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): flock(int, int): Unable to lock serial port " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
	}

	if (tcgetattr(this->getFileDescriptor(), &this->m_oldPortSettings) != 0) {
		const auto errorCode = getLastError();
		throw std::runtime_error("CppSerialPort::SerialPort::openPort(): tcgetattr(int, termios *): Unable to get current attributes for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
	}
    memset(&this->m_portSettings, 0, sizeof(this->m_portSettings));
    this->m_portSettings = this->m_oldPortSettings;
    cfmakeraw(&this->m_portSettings);

    this->m_portSettings.c_lflag &= (~(ICANON|ECHO|ECHOE|ECHOK|ECHONL|ISIG));
    this->m_portSettings.c_iflag &= (~(INPCK|IGNPAR|PARMRK|ISTRIP|ICRNL|IXANY));
    this->m_portSettings.c_oflag &= (~OPOST);
    this->m_portSettings.c_cc[VMIN]= 0;
    this->m_portSettings.c_cflag |= (CLOCAL | CREAD);
#endif

	this->m_isOpen = true;
    this->setBaudRate(this->m_baudRate);
    this->setDataBits(this->m_dataBits);
    this->setStopBits(this->m_stopBits);
    this->setParity(this->m_parity);
    this->setFlowControl(this->m_flowControl);
    this->setReadTimeout(this->readTimeout());

    this->enableDTR();
    this->enableRTS();
}

void SerialPort::setReadTimeout(int timeout)
{
    IByteStream::setReadTimeout(timeout);
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    COMMTIMEOUTS commTimeouts{};
    commTimeouts.ReadIntervalTimeout         = MAXDWORD;
    commTimeouts.ReadTotalTimeoutMultiplier  = 0;
    commTimeouts.ReadTotalTimeoutConstant    = static_cast<DWORD>(this->readTimeout());
    commTimeouts.WriteTotalTimeoutMultiplier = 0;
    commTimeouts.WriteTotalTimeoutConstant   = static_cast<DWORD>(this->writeTimeout());

    if(!SetCommTimeouts(this->m_serialPortHandle, &commTimeouts)) {
        auto errorCode = getLastError();
        this->closePort();
		throw std::runtime_error("CppSerialPort::SerialPort::setReadTimeout(int timeout): SetCommTimeouts(HANDLE, COMMTIMEOUTS*): Unable to set timeout settings for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    if (this->readTimeout() == 0) {
        fcntl(this->getFileDescriptor(), F_SETFL, O_NDELAY);
    } else {
        fcntl(this->getFileDescriptor(), F_SETFL, O_SYNC);
        this->m_portSettings.c_cc[VTIME] = static_cast<cc_t>(this->readTimeout() / 100);
        tcsetattr(this->getFileDescriptor(), TCSANOW, &this->m_portSettings);
    }
#endif //defined(_WIN32)
}


int SerialPort::getLastError() {
#if defined(_WIN32)
	return static_cast<int>(GetLastError());
#else
	return errno;
#endif //defined(_WIN32)
}

std::string SerialPort::getErrorString(int errorCode) {
	char errorString[PATH_MAX];
	memset(errorString, '\0', PATH_MAX);
#if defined(_WIN32)
	wchar_t *wideErrorString{ nullptr };
	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
        static_cast<DWORD>(errorCode),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&wideErrorString),
		0,
		nullptr
	);
	size_t converted{ 0 };
	auto conversionResult = wcstombs_s(&converted, errorString, PATH_MAX, wideErrorString, PATH_MAX);
	(void)conversionResult;
	//wcstombs(errorString, wideErrorString, PATH_MAX);
	LocalFree(wideErrorString);
#else
    auto strerrorCode = strerror_r(errorCode, errorString, PATH_MAX);
    if (strerrorCode == nullptr) {
        std::cerr << "strerror_r(int, char *, int): error occurred" << std::endl;
        return "";
    }
#endif //defined(_WIN32)
	return IByteStream::stripLineEndings(errorString);
}

char SerialPort::read(bool *readTimeout)
{
#if defined(_WIN32)
    if (!this->m_readBuffer.empty()) {
        if (readTimeout) *readTimeout = false;
        char returnValue{this->m_readBuffer[0]};
        this->m_readBuffer = this->m_readBuffer.popFront();
        return returnValue;
    }

	static char readStuff[SERIAL_PORT_BUFFER_MAX];
	memset(readStuff, '\0', SERIAL_PORT_BUFFER_MAX);

	DWORD commErrors{};
	COMSTAT commStatus{};
	auto clearErrorsResult = ClearCommError(this->m_serialPortHandle, &commErrors, &commStatus);
	if (clearErrorsResult == 0) {
		const auto errorCode = getLastError();
		std::cout << "ClearCommError(HANDLE, LPDWORD, LPCOMSTAT) error: " << toStdString(errorCode) << " (" << getErrorString(errorCode) << ")" << std::endl;
	}

    DWORD readBytes{0};
	DWORD maxBytes{ commStatus.cbInQue };
    if (commStatus.cbInQue == 0) {
        maxBytes = 1;
    }

    auto readResult = ReadFile(this->m_serialPortHandle, readStuff, maxBytes, &readBytes, nullptr);
    if (readResult == 0) {
		const auto errorCode = getLastError();
		std::cout << "ReadFile(HANDLE, LPVOID, DWORD, LPDWORD, LPDWORD) error: " << toStdString(errorCode) << " (" << getErrorString(errorCode) << ")" << std::endl;
		if (readTimeout) *readTimeout = true;
        return 0;
	}
	if ( ( readBytes <= 0 ) || (readStuff[0] == '\0') ) {
        if (readTimeout) *readTimeout = true;
		return 0;
    }
    for (size_t i = 0; i < readBytes; i++) {
        this->m_readBuffer += readStuff[i];
    }
    if (readTimeout) *readTimeout = false;
    char returnValue{this->m_readBuffer[0]};
    this->m_readBuffer = this->m_readBuffer.popFront();
    return returnValue;
#else
    if (!this->m_readBuffer.empty()) {
        char returnValue{this->m_readBuffer[0]};
        this->m_readBuffer.popFront();
        if (readTimeout) {
            *readTimeout = false;
        }
        return returnValue;
    }

    //Use select() to wait for data to arrive
    //At socket, then read and return
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

    if (select(this->getFileDescriptor() + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
        int bytesAvailable{0};
        ioctl(this->getFileDescriptor(), FIONREAD, &bytesAvailable);
        auto returnedBytes = fread(readStuff, sizeof(char), static_cast<size_t>(bytesAvailable), this->m_fileStream);
        if (returnedBytes <= 0) {
            if (readTimeout) {
                *readTimeout = true;
            }
            return 0;
        }
        for (size_t i = 0; i < returnedBytes; i++) {
            this->m_readBuffer += readStuff[i];
        }
        char returnValue{this->m_readBuffer[0]};
        this->m_readBuffer.popFront();
        if (readTimeout) {
            *readTimeout = false;
        }
        return returnValue;
    }
    if (readTimeout) {
        *readTimeout = true;
    }
    return 0;
#endif //defined(_WIN32)
}

ssize_t SerialPort::write(char c)
{
#if defined(_WIN32)
    DWORD writtenBytes{0};
	if (!WriteFile(this->m_serialPortHandle, &c, 1 , &writtenBytes, nullptr)) {
        auto errorCode = getLastError();
        (void)errorCode;
        //TODO: Check if errorCode is IO_NOT_COMPLETED or whatever
        return 0;
    }
    return static_cast<ssize_t>(writtenBytes);
#else
    auto writtenBytes = ::write(this->getFileDescriptor(), &c, 1);
    if (writtenBytes != 0) {
        return (getLastError() == EAGAIN ? 0 : writtenBytes);
    }
    return writtenBytes;
#endif //defined(_WIN32)
}

ssize_t SerialPort::write(const char *bytes, size_t numberOfBytes) {
#if defined(_WIN32)
	DWORD writtenBytes{ 0 };
	if (!WriteFile(this->m_serialPortHandle, bytes, static_cast<DWORD>(numberOfBytes), &writtenBytes, nullptr)) {
		auto errorCode = getLastError();
		(void)errorCode;
		//TODO: Check if errorCode is IO_NOT_COMPLETED or whatever
		return 0;
	}
	return static_cast<ssize_t>(writtenBytes);
#else
	auto writtenBytes = ::write(this->getFileDescriptor(), bytes, numberOfBytes);
	if (writtenBytes != 0) {
		return (getLastError() == EAGAIN ? 0 : writtenBytes);
	}
	return writtenBytes;
#endif //defined(_WIN32)
}

void SerialPort::closePort()
{
    if (!this->isOpen()) {
        return;
    }
    try {
#if defined(_WIN32)
        CancelIo(this->m_serialPortHandle);
        CloseHandle(this->m_serialPortHandle);
#else
        //TODO: Check error codes for these functions
        std::memcpy(&this->m_portSettings, &this->m_oldPortSettings, sizeof(this->m_portSettings));
        this->m_portSettings = this->m_oldPortSettings;
        this->applyPortSettings();
        flock(this->getFileDescriptor(), LOCK_UN);
        fclose(this->m_fileStream);
#endif
        this->m_isOpen = false;
    } catch (std::exception &e) {
        this->m_isOpen = false;
    }
}

modem_status_t SerialPort::getModemStatus() const
{
#if defined(_WIN32)
    modem_status_t status{0};
    if (GetCommModemStatus(this->m_serialPortHandle, &status) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::getModemStatus(): GetCommModemStatus(HANDLE, LPDWORD): Unable to get modem status for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    return status;
#else
    modem_status_t status{0};
    if(ioctl(this->getFileDescriptor(), TIOCMGET, &status) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::getModemStatus(): ioctl(int, int, int): Unable to get modem settings for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif //defined(_WIN32)
    return status;
}

void SerialPort::enableDTR()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    if (EscapeCommFunction(this->m_serialPortHandle, SETDTR) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::enableDTR(): EscapeCommFunction(HANDLE, DWORD): Unable to set DTR settings for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    modem_status_t status{this->getModemStatus()};
    status |= TIOCM_DTR;
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::enableDTR(): ioctl(int, int, int): Unable to set DTR for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif
}

void SerialPort::disableDTR()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    if (EscapeCommFunction(this->m_serialPortHandle, CLRDTR) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::disableDTR(): EscapeCommFunction(HANDLE, DWORD): Unable to reset DTR for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    modem_status_t status{this->getModemStatus()};
    status &= ~TIOCM_DTR;
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::disableDTR(): ioctl(int, int, int): Unable to reset DTR for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif
}

void SerialPort::enableRTS()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    if (EscapeCommFunction(this->m_serialPortHandle, SETRTS) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::enableRTS(): EscapeCommFunction(HANDLE, DWORD): Unable to set RTS for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    modem_status_t status{this->getModemStatus()};
    status |= TIOCM_RTS;
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::enableRTS(): ioctl(int, int, int): Unable to set RTS for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif
}

void SerialPort::disableRTS()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    if (EscapeCommFunction(this->m_serialPortHandle, CLRRTS) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::disableRTS(): EscapeCommFunction(HANDLE, DWORD): Unable to reset RTS for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    modem_status_t status{this->getModemStatus()};
    status &= ~TIOCM_RTS;
    if(ioctl(this->getFileDescriptor(), TIOCMSET, &status) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::disableRTS(): ioctl(int, int, int): Unable to reset DTR for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif
}

bool SerialPort::isDCDEnabled() const
{
    if (!this->isOpen()) {
        return false;
    }
#if defined(_WIN32)
    return static_cast<bool>(this->getModemStatus() & MS_RLSD_ON);
#else
    return static_cast<bool>(this->getModemStatus() & TIOCM_CAR);
#endif
}


bool SerialPort::isCTSEnabled() const
{
    if (!this->isOpen()) {
        return false;
    }
#if defined(_WIN32)
    return static_cast<bool>(this->getModemStatus() & MS_CTS_ON);
#else
    return static_cast<bool>(this->getModemStatus() & TIOCM_CTS);
#endif
}

bool SerialPort::isDSREnabled() const
{
    if (!this->isOpen()) {
        return false;
    }
#if defined(_WIN32)
    return static_cast<bool>(this->getModemStatus() & MS_DSR_ON);
#else
    return static_cast<bool>(this->getModemStatus() & TIOCM_DSR);
#endif
}

void SerialPort::flushRx()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    PurgeComm(this->m_serialPortHandle, PURGE_RXCLEAR | PURGE_RXABORT);
#else
    tcflush(this->getFileDescriptor(), TCIFLUSH);
#endif
}


void SerialPort::flushTx()
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    PurgeComm(this->m_serialPortHandle, PURGE_TXCLEAR | PURGE_TXABORT);
#else
    tcflush(this->getFileDescriptor(), TCOFLUSH);
#endif
}


bool SerialPort::isAvailableSerialPort(const std::string &name)
{
	auto availablePorts = availableSerialPorts();
#if defined(_WIN32)
    std::string copyName{name};
    copyName.erase(std::remove_if(copyName.begin(), copyName.end(), [](char c) { return ( (c == '.') || (c == '\\') ); }), copyName.end());
	return (availablePorts.find(copyName) != availablePorts.end());
#else
	return (availablePorts.find(name) != availablePorts.end());
#endif //defined(_WIN32)
}

bool SerialPort::isOpen() const
{
    return this->m_isOpen;
}



void SerialPort::setDataBits(DataBits dataBits)
{
    if (!this->isOpen()) {
        return;
    }
    if ( (this->m_stopBits == StopBits::StopTwo) && (dataBits == DataBits::DataFive) ) {
        throw std::runtime_error("CppSerialPort::SerialPort::setDataBits(DataBits): Five data bits cannot be used with two stop bits");
    }
#if defined(_WIN32)
    if ( (dataBits != DataBits::DataFive) && (this->m_stopBits == StopBits::StopOneFive) ) {
        throw std::runtime_error("CppSerialPort::SerialPort::setDataBits(DataBits): 1.5 stop bits can only be used with 5 data bits");
    }
    this->m_portSettings.dcb.ByteSize = static_cast<BYTE>(dataBits);
#else
    this->m_portSettings.c_cflag &= (~CSIZE);
    this->m_portSettings.c_cflag |= static_cast<tcflag_t>(dataBits);
#endif //defined(_WIN32)
    this->applyPortSettings();
    this->m_dataBits = dataBits;
}

void SerialPort::setBaudRate(BaudRate baudRate)
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    this->m_portSettings.dcb.BaudRate = static_cast<DWORD>(baudRate);
#else
    if (cfsetispeed(&this->m_portSettings, static_cast<speed_t>(baudRate)) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::setBaudRate(BaudRate): cfsetispeed(port_settings_t *, speed_t): Unable to set baud rate settings for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    if (cfsetospeed(&this->m_portSettings, static_cast<speed_t>(baudRate)) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::setBaudRate(BaudRate): cfsetospeed(port_settings_t *, speed_t): Unable to set baud rate settings for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
    /*
    this->m_portSettings.c_cflag &= ~(CBAUD);
    this->m_portSettings.c_cflag |= static_cast<speed_t>(baudRate);
    */
#endif //defined(_WIN32)
    this->applyPortSettings();
    this->m_baudRate = baudRate;
}

void SerialPort::setStopBits(StopBits stopBits)
{
    if (!this->isOpen()) {
        return;
    }
    if ( (stopBits == StopBits::StopTwo) && (this->m_dataBits == DataBits::DataFive) ) {
        throw std::runtime_error("CppSerialPort::SerialPort::setStopBits(StopBits): 2 stop bits can not be used with 5 data bits");
    }
#if defined(_WIN32)
    if ( (stopBits == StopBits::StopOneFive) && (this->m_dataBits != DataBits::DataFive) ) {
        throw std::runtime_error("CppSerialPort::SerialPort::setStopBits(StopBits): 1.5 stop bits can only be used with 5 data bits");
    }
    this->m_portSettings.dcb.StopBits = static_cast<BYTE>(stopBits);
#else
    if (stopBits == StopBits::StopOne) {
        this->m_portSettings.c_cflag &= (~CSTOPB);
    } else if (stopBits == StopBits::StopTwo){
        this->m_portSettings.c_cflag |= CSTOPB;
    }
#endif //defined(_WIN32)
    this->applyPortSettings();
    this->m_stopBits = stopBits;
}

void SerialPort::setParity(Parity parity)
{
    if (!this->m_isOpen) {
        return;
    }
#if defined(_WIN32)
    if (parity == Parity::ParityNone) {
        this->m_portSettings.dcb.fParity = FALSE;
    } else if (parity == Parity::ParityEven) {
        this->m_portSettings.dcb.fParity = TRUE;
    } else if (parity == Parity::ParityOdd) {
        this->m_portSettings.dcb.fParity = TRUE;
    } else if (parity == Parity::ParityMark) {
        this->m_portSettings.dcb.fParity = TRUE;
    } else if (parity == Parity::ParitySpace) {
        this->m_portSettings.dcb.fParity = TRUE;
    }
    this->m_portSettings.dcb.Parity = static_cast<unsigned char>(parity);
#else
    if ( (parity == Parity::ParitySpace) && (this->m_dataBits == DataBits::DataEight) ) {
        throw std::runtime_error("CppSerialPort::SerialPort::setParity(Parity): Eight data bits cannot be used with space parity");
    }
    if (parity == Parity::ParityNone) {
        this->m_portSettings.c_cflag &= (~PARENB);
        this->m_portSettings.c_iflag &= (~INPCK);
        this->m_portSettings.c_iflag |= IGNPAR;
    } else if (parity == Parity::ParityEven) {
        this->m_portSettings.c_cflag |= PARENB;
        this->m_portSettings.c_iflag |= INPCK; //Set parity
        this->m_portSettings.c_iflag &= (~IGNPAR); //Reset ignore parity
    } else if (parity == Parity::ParityOdd) {
        this->m_portSettings.c_cflag |= (PARENB | PARODD);
        this->m_portSettings.c_iflag |= INPCK; //Set parity
        this->m_portSettings.c_iflag &= (~IGNPAR); //Reset ignore parity
    } else if (parity == Parity::ParitySpace) {
        //Simulate space by adding extra data bit
        this->setDataBits(static_cast<DataBits>(static_cast<int>(this->m_dataBits) + 1));
    }
#endif //defined(_WIN32)
    this->applyPortSettings();
    this->m_parity = parity;
}

void SerialPort::setFlowControl(FlowControl flowControl)
{
    if (!this->isOpen()) {
        return;
    }
#if defined(_WIN32)
    if (flowControl == FlowControl::FlowOff) {
        this->m_portSettings.dcb.fOutxCtsFlow = FALSE;
        this->m_portSettings.dcb.fRtsControl = RTS_CONTROL_DISABLE;
        this->m_portSettings.dcb.fInX = FALSE;
        this->m_portSettings.dcb.fOutX = FALSE;
    } else if (flowControl == FlowControl::FlowXonXoff) {
        this->m_portSettings.dcb.fOutxCtsFlow = FALSE;
        this->m_portSettings.dcb.fRtsControl = RTS_CONTROL_DISABLE;
        this->m_portSettings.dcb.fInX = TRUE;
        this->m_portSettings.dcb.fOutX = TRUE;
    } else if (flowControl == FlowControl::FlowHardware) {
        this->m_portSettings.dcb.fOutxCtsFlow = TRUE;
        this->m_portSettings.dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
        this->m_portSettings.dcb.fInX = FALSE;
        this->m_portSettings.dcb.fOutX = FALSE;
    }
#else
    if (flowControl == FlowControl::FlowOff) {
        this->m_portSettings.c_cflag &= (~CRTSCTS);
        this->m_portSettings.c_iflag &= (~(IXON | IXOFF | IXANY));
    } else if (flowControl == FlowControl::FlowXonXoff) {
        this->m_portSettings.c_cflag &= (~CRTSCTS);
        this->m_portSettings.c_iflag |= (IXON|IXOFF|IXANY);
    } else if (flowControl == FlowControl::FlowHardware) {
        this->m_portSettings.c_cflag |= CRTSCTS;
        this->m_portSettings.c_iflag &= (~(IXON|IXOFF|IXANY));
    }
#endif //defined(_WIN32)
    this->applyPortSettings();
    this->m_flowControl = flowControl;
}

void SerialPort::applyPortSettings()
{
#if defined(_WIN32)
    if (SetCommConfig(this->m_serialPortHandle, &this->m_portSettings, sizeof(COMMCONFIG)) == 0) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::applyPortSettings(): SetCommConfig(HANDLE, COMMCONFIG, DWORD): Unable to apply serial port attributes for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#else
    if (tcsetattr(this->getFileDescriptor(), TCSANOW, &this->m_portSettings) == -1) {
        const auto errorCode = getLastError();
        throw std::runtime_error("CppSerialPort::SerialPort::applyPortSettings(): tcsetattr(int, int, termios *): Unable to apply serial port attributes for " + this->portName() + ": error code " + toStdString(errorCode) + " (" + getErrorString(errorCode) + ')');
    }
#endif //defined(_WIN32)
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

FlowControl SerialPort::flowControl() const
{
    return this->m_flowControl;
}

std::string SerialPort::portName() const
{
#if defined(_WIN32)
    std::string copyName{this->m_portName};
    copyName.erase(std::remove_if(copyName.begin(), copyName.end(), [](char c) { return ( (c == '.') || (c == '\\') ); }), copyName.end());
    return copyName;
#else
    return this->m_portName;
#endif //defined(_WIN32)
}

std::unordered_set<std::string> SerialPort::availableSerialPorts()
{
    std::unordered_set<std::string> returnSet;
#if defined(_WIN32)
    try {
        HKEY hRegistryKey;
        LONG operationResult{ RegOpenKeyExA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, 0, KEY_READ, &hRegistryKey) };
        if (operationResult != ERROR_SUCCESS) {
            return returnSet;
        }
        for (DWORD index = 0; ; index++) {
            char SubKeyName[PATH_MAX];
            DWORD cName{ PATH_MAX };
            DWORD cbData{ PATH_MAX };
            char hRegistryKeyValue[PATH_MAX];
            operationResult = RegEnumValueA(hRegistryKey, index, SubKeyName, &cName, nullptr, nullptr, nullptr, nullptr);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            operationResult = RegGetValueA(HKEY_LOCAL_MACHINE, SERIAL_PORT_REGISTRY_PATH, SubKeyName, RRF_RT_REG_SZ, nullptr, hRegistryKeyValue, &cbData);
            if (operationResult != ERROR_SUCCESS) {
                break;
            }
            returnSet.emplace(hRegistryKeyValue);
        }
        RegCloseKey(hRegistryKey);
        return returnSet;
    } catch (std::exception &e) {
        (void)e;
        return returnSet;
    }
#else
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (IByteStream::fileExists(std::string{it})) {
            returnSet.emplace(it);
        }
    }
    return returnSet;
#endif
}

std::vector<std::string> SerialPort::generateSerialPortNames()
{
    std::vector<std::string> returnSet;
#if defined(_WIN32)
	for (int i = 0; i < UCHAR_MAX; i++) {
		returnSet.push_back(AVAILABLE_PORT_NAMES_BASE + toStdString(i));
	}
#else
	for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
		for (int i = 0; i < UCHAR_MAX; i++) {
			returnSet.push_back(it + toStdString(i));
		}
	}
#endif //defined(_WIN32)
    return returnSet;
}

bool SerialPort::isValidSerialPortName(const std::string &serialPortName)
{
#if defined(_WIN32)
	auto foundCom = serialPortName.find("COM");
	if ( (foundCom == std::string::npos) || (foundCom != 0) || (serialPortName.length() == 3)) {
		return false;
	}
	try {
		int comNumber{ std::stoi(serialPortName.substr(strlen("COM"))) };
		return ((comNumber > 0) && (comNumber < UCHAR_MAX));
	} catch (std::exception &e) {
		(void)e;
		return false;
	}
#else
    for (auto &it : SerialPort::AVAILABLE_PORT_NAMES_BASE) {
        for (int i = 0; i < UCHAR_MAX; i++) {
            if (serialPortName == (it + toStdString(i))) {
                return true;
            }
        }
    }
    return false;
#endif
}

void SerialPort::putBack(char c)
{
    if (this->m_readBuffer.length() > 0) {
        this->m_readBuffer = c + this->m_readBuffer;
    } else {
#if !defined(_WIN32)
        ungetc(c, this->m_fileStream);
#endif //!defined(_WIN32)
    }
}

std::pair<int, std::string> SerialPort::getPortNameAndNumber(const std::string &name)
{
#if defined(_WIN32)
    auto foundCom = name.find("COM");
	if ((foundCom != 0) || (name.length() == 3)) {
		throw std::runtime_error("CppSerialPort::SerialPort::getPortNameAndNumber(const std::string &): ERROR: " + name + " is an invalid serial port name");
	}
	try {
		int comNumber{ std::stoi(name.substr(3)) };
            return std::make_pair(comNumber, AVAILABLE_PORT_NAMES_BASE + toStdString(comNumber));
	} catch (std::exception &e) {
		(void)e;
		throw std::runtime_error("CppSerialPort::SerialPort::getPortNameAndNumber(const std::string &): ERROR: " + name + " is an invalid serial port name");
	}
#else
    std::string str{name};
    auto iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
    str = name;
    if (str.find("/dev/tty") == std::string::npos) {
        str = "/dev/tty" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }
    str = name;
    if (str.find("/dev/") == std::string::npos) {
        str = "/dev/" + str;
    }
    iter = std::find(SERIAL_PORT_NAMES.cbegin(), SERIAL_PORT_NAMES.cend(), str);
    if (iter != SERIAL_PORT_NAMES.cend()) {
        return std::make_pair(static_cast<int>(std::distance(SERIAL_PORT_NAMES.begin(), iter)), str);
    }

    throw std::runtime_error("CppSerialPort::SerialPort::getPortNameAndNumber(const std::string &): ERROR: " + name + " is an invalid serial port name");
#endif
}

SerialPort::~SerialPort() {
    this->closePort();
}

}

//namespace CppSerialPort
