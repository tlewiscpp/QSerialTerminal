/***********************************************************************
*    serialport.cpp:                                                   *
*    SerialPort class, for connecting to an RS232 serial port          *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SerialPort class          *
*    It is used to connect to RS232 compliant serial ports             *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "serialport.h"

#include "generalutilities.h"

const DataBits SerialPort::DEFAULT_DATA_BITS{DataBits::EIGHT};
const StopBits SerialPort::DEFAULT_STOP_BITS{StopBits::ONE};
const Parity SerialPort::DEFAULT_PARITY{Parity::NONE};
const BaudRate SerialPort::DEFAULT_BAUD_RATE{BaudRate::BAUD115200};
const std::string SerialPort::DEFAULT_LINE_ENDING{"\r"};
const long SerialPort::DEFAULT_TIMEOUT{100};
const long SerialPort::DEFAULT_RETRY_COUNT{0};
const std::string SerialPort::DEFAULT_DATA_BITS_STRING{"8"};
const std::string SerialPort::DEFAULT_STOP_BITS_STRING{"1"};
const std::string SerialPort::DEFAULT_PARITY_STRING{"None"};
const std::string SerialPort::DEFAULT_BAUD_RATE_STRING{"115200"};
const std::vector<const char *> SerialPort::AVAILABLE_PARITY{"None", "Even", "Odd"};
const std::vector<const char *> SerialPort::AVAILABLE_STOP_BITS{"1", "2"};
const std::vector<const char *> SerialPort::AVAILABLE_DATA_BITS{"5", "6", "7", "8"};
const unsigned int SerialPort::ASCII_WHITESPACE_MAXIMUM_VALUE{32};
const unsigned int SerialPort::ASCII_HIGH_LIMIT{128};
const unsigned int SerialPort::ASCII_LOW_LIMIT{0};

#if defined(_WIN32) || defined(__CYGWIN__)
    const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"\\\\.\\COM"};
    const char *SerialPort::DTR_RTS_ON_IDENTIFIER{"dtr=on rts=on"};
    const char *SerialPort::SERIAL_PORT_REGISTRY_PATH{"HARDWARE\\DEVICEMAP\\SERIALCOMM\\"};
#else
    const std::vector<const char *> SerialPort::AVAILABLE_PORT_NAMES_BASE{"/dev/ttyS", "/dev/ttyACM", "/dev/ttyUSB", 
                                                                            "/dev/ttyAMA", "/dev/ttyrfserialm", "/dev/irserialm",
                                                                            "/dev/cuau", "/dev/cuaU", "/dev/rfcomm"};
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"110", "300", "600", "1200", "2400", "4800", 
                                                                    "9600", "19200", "38400", "57600", "115200", 
                                                                    "128000", "256000", "500000", "1000000"};
#else
    const std::vector<const char *> SerialPort::AVAILABLE_BAUD_RATE{"50", "75", "110", "134", "150", "200", "300",
                                                                    "600", "1200", "1800", "2400", "4800", "9600", "19200",
                                                                    "38400", "57600", "115200", "230400", "460800", "500000",
                                                                    "576000", "921600", "1000000", "1152000", "1500000",
                                                                    "2000000", "2500000", "3000000", "3500000", "4000000"};
#endif

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

SerialPort::SerialPort(SerialPort &&other) :
    m_portName{std::move(other.m_portName)},
    m_portNumber{std::move(other.m_portNumber)},
    m_baudRate{std::move(other.m_baudRate)},
    m_stopBits{std::move(other.m_stopBits)},
    m_dataBits{std::move(other.m_dataBits)},
    m_parity{std::move(other.m_parity)},
    m_lineEnding{std::move(other.m_lineEnding)},
    m_timeout{std::move(other.m_timeout)},
    m_retryCount{std::move(other.m_retryCount)},
    m_isOpen{std::move(other.m_isOpen)}
{

}


SerialPort::SerialPort(const std::string &name, BaudRate baudRate, StopBits stopBits, DataBits dataBits, Parity parity) :
    m_portName{name},
    m_portNumber{0},
    m_baudRate{baudRate},
    m_stopBits{stopBits},
    m_dataBits{dataBits},
    m_parity{parity},
    m_lineEnding{DEFAULT_LINE_ENDING},
    m_timeout{DEFAULT_TIMEOUT},
    m_retryCount{DEFAULT_RETRY_COUNT},
    m_isOpen{false},
    m_isListening{false},
    m_shutEmDown{false}
{
    std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(this->m_portName)};
    this->m_portNumber = truePortNameAndNumber.first;
    this->m_portName = truePortNameAndNumber.second;
}

void SerialPort::begin(long baud)
{
    this->closePort();
    BaudRate newBaudRate{BaudRate::BAUD115200};
    try {
        newBaudRate = parseBaudRateFromRaw(std::to_string(baud));
    } catch (std::exception &e) {
        std::cout << e.what();
        throw e;   
    }
    this->setBaudRate(newBaudRate);
    this->openPort();
}

void SerialPort::end()
{
    this->closePort();
}

bool operator==(const SerialPort &lhs, const SerialPort &rhs)
{
    return (lhs.portName() == rhs.portName());
}

char SerialPort::read()
{
    return this->rawRead();
}

void SerialPort::openPort()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    std::string mode{""};
    mode += "baud=" + std::to_string(parseBaudRate(this->m_baudRate)) + " ";
    mode += "data=" + std::to_string(parseDataBits(this->m_dataBits)) + " ";
    mode += "parity=" + std::string(1, static_cast<char>(parseParity(this->m_parity).first)) + " ";
    mode += "stop=" + std::to_string(parseStopBits(this->m_stopBits)) + " ";
    mode += DTR_RTS_ON_IDENTIFIER;

    this->m_serialPort[this->m_portNumber] = CreateFileA(this->m_portName.c_str(),
                                                      GENERIC_READ|GENERIC_WRITE,
                                                      0,                          /* no share  */
                                                      NULL,                       /* no security */
                                                      OPEN_EXISTING,
                                                      0,                          /* no threads */
                                                      NULL);                      /* no templates */

    if(this->m_serialPort[this->m_portNumber] == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName);
    }

    DCB portSettings;
    memset(&portSettings, 0, sizeof(portSettings));
    portSettings.DCBlength = sizeof(portSettings);

    if(!BuildCommDCBA(mode.c_str(), &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set dcb settings for serial port " + this->m_portName);
    }

    if(!SetCommState(this->m_serialPort[this->m_portNumber], &portSettings)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set cfg settings for serial port " + this->m_portName);
    }

    COMMTIMEOUTS Cptimeouts;
    Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
    Cptimeouts.ReadTotalTimeoutConstant    = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant   = 0;

    if(!SetCommTimeouts(this->m_serialPort[this->m_portNumber], &Cptimeouts)) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set timeout settings for serial port " + this->m_portName);
    }
    this->m_isOpen = true;
#else

    if (!isAvailableSerialPort(this->m_portName)) {
        throw std::runtime_error("ERROR: " + this->m_portName + " is not a currently available serial port (is something else using it?)");
    }
    int status{0};
    int error{0};
    int baudRate{parseBaudRate(this->m_baudRate)};
    int cbits{parseDataBits(this->m_dataBits)};
    std::pair<int, int> parityPair{parseParity(this->m_parity)};
    int cpar{parityPair.first};
    int ipar{parityPair.second};
    int bstop{parseStopBits(this->m_stopBits)};
    this->m_serialPort[this->m_portNumber] = open(this->m_portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(this->m_serialPort[this->m_portNumber] == -1) {
        throw std::runtime_error("ERROR: Unable to open serial port " + this->m_portName);
    }

    if(flock(this->m_serialPort[this->m_portNumber], LOCK_EX | LOCK_NB) != 0) {
        this->closePort();
        throw std::runtime_error("ERROR: Another process has locked serial port " + this->m_portName);
    }

    error = tcgetattr(this->m_serialPort[this->m_portNumber], this->m_oldPortSettings + this->m_portNumber);
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

    error = tcsetattr(this->m_serialPort[this->m_portNumber], TCSANOW, &this->m_newPortSettings);
    if(error == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to adjust port settings for serial port " + this->m_portName);
    }

    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to get port status for serial port " + this->m_portName);
    }

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        this->closePort();
        throw std::runtime_error("ERROR: Unable to set port status for serial port " + this->m_portName);
    }
    this->m_isOpen = true;
#endif
}


int SerialPort::parseBaudRate(BaudRate baudRate)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (baudRate == BaudRate::BAUD110) {
        return 110;
    } else if (baudRate == BaudRate::BAUD300) {
        return 300;
    } else if (baudRate == BaudRate::BAUD600) {
        return 600;
    } else if (baudRate == BaudRate::BAUD1200) {
        return 1200;
    } else if (baudRate == BaudRate::BAUD2400) {
        return 2400;
    } else if (baudRate == BaudRate::BAUD4800) {
        return 4800;
    } else if (baudRate == BaudRate::BAUD9600) {
        return 9600;
    } else if (baudRate == BaudRate::BAUD19200) {
        return 19200;
    } else if (baudRate == BaudRate::BAUD38400) {
        return 38400;
    } else if (baudRate == BaudRate::BAUD57600) {
        return 57600;
    } else if (baudRate == BaudRate::BAUD115200) {
        return 115200;
    } else if (baudRate == BaudRate::BAUD128000) {
        return 128000;
    } else if (baudRate == BaudRate::BAUD256000) {
        return 256000;
    } else if (baudRate == BaudRate::BAUD500000) {
        return 500000;
    } else if (baudRate == BaudRate::BAUD1000000) {
        return 1000000;
    } else {
        return parseBaudRate(DEFAULT_BAUD_RATE);
    }
    return parseBaudRate(DEFAULT_BAUD_RATE);
#else
    if (baudRate == BaudRate::BAUD50) {
        return B50;
    } else if (baudRate == BaudRate::BAUD75) {
        return B75;
    } else if (baudRate == BaudRate::BAUD110) {
        return B110;
    } else if (baudRate == BaudRate::BAUD134) {
        return B134;
    } else if (baudRate == BaudRate::BAUD150) {
        return B150; 
    } else if (baudRate == BaudRate::BAUD200) {
        return B200;
    } else if (baudRate == BaudRate::BAUD300) {
        return B300;
    } else if (baudRate == BaudRate::BAUD600) {
        return B600;
    } else if (baudRate == BaudRate::BAUD1200) {
        return B1200; 
    } else if (baudRate == BaudRate::BAUD1800) {
        return B1800;
    } else if (baudRate == BaudRate::BAUD2400) {
        return B2400;
    } else if (baudRate == BaudRate::BAUD4800) {
        return B4800;
    } else if (baudRate == BaudRate::BAUD9600) {
        return B9600; 
    } else if (baudRate == BaudRate::BAUD19200) {
        return B19200;
    } else if (baudRate == BaudRate::BAUD38400) {
        return B38400;
    } else if (baudRate == BaudRate::BAUD57600) {
        return B57600;
    } else if (baudRate == BaudRate::BAUD115200) {
        return B115200; 
    } else if (baudRate == BaudRate::BAUD230400) {
        return B230400;
    } else if (baudRate == BaudRate::BAUD460800) {
        return B460800;
    } else if (baudRate == BaudRate::BAUD500000) {
        return B500000;
    } else if (baudRate == BaudRate::BAUD576000) {
        return B576000; 
    } else if (baudRate == BaudRate::BAUD921600) {
        return B921600;
    } else if (baudRate == BaudRate::BAUD1000000) {
        return B1000000;
    } else if (baudRate == BaudRate::BAUD1152000) {
        return B1152000;
    } else if (baudRate == BaudRate::BAUD1500000) {
        return B1500000; 
    } else if (baudRate == BaudRate::BAUD2000000) {
        return B2000000;
    } else if (baudRate == BaudRate::BAUD2500000) {
        return B2500000;
    } else if (baudRate == BaudRate::BAUD3000000) {
        return B3000000;
    } else if (baudRate == BaudRate::BAUD3500000) {
        return B3500000; 
    } else if (baudRate == BaudRate::BAUD4000000) {
        return B4000000;
    } else {
        return parseBaudRate(DEFAULT_BAUD_RATE);
    }
    return parseBaudRate(DEFAULT_BAUD_RATE);
#endif
}

int SerialPort::parseDataBits(DataBits dataBits)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (dataBits == DataBits::EIGHT) {
        return 8;
    } else if (dataBits == DataBits::SEVEN) {
        return 7;
    } else if (dataBits == DataBits::SIX) {
        return 6;
    } else if (dataBits == DataBits::FIVE) {
        return 5;
    } else {
        return parseDataBits(DEFAULT_DATA_BITS);
    }
    return parseDataBits(DEFAULT_DATA_BITS);
#else
    if (dataBits == DataBits::EIGHT) {
        return CS8;
    } else if (dataBits == DataBits::SEVEN) {
        return CS7;
    } else if (dataBits == DataBits::SIX) {
        return CS6;
    } else if (dataBits == DataBits::FIVE) {
        return CS5;
    } else {
        return parseDataBits(DEFAULT_DATA_BITS);
    }
    return parseDataBits(DEFAULT_DATA_BITS);
#endif
} 

int SerialPort::parseStopBits(StopBits stopBits)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    if (stopBits == StopBits::ONE) {
        return 1;
    } else if (stopBits == StopBits::TWO) {
        return 2;
    } else {
        return parseStopBits(DEFAULT_STOP_BITS);
    }
    return parseStopBits(DEFAULT_STOP_BITS);
#else
    if (stopBits == StopBits::ONE) {
        return 0;
    } else if (stopBits == StopBits::TWO) {
        return CSTOPB;
    } else {
        return parseStopBits(DEFAULT_STOP_BITS);
    }
    return parseStopBits(DEFAULT_STOP_BITS);
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

unsigned char SerialPort::rawRead()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    std::unique_ptr<unsigned char> buffer{new unsigned char[SERIAL_PORT_BUFFER_MAX]};
    unsigned char charToReturn{0};
    long int returnedBytes{0};
    ReadFile(this->m_serialPort[this->m_portNumber], buffer.get(), 1, (LPDWORD)((void *)&returnedBytes), NULL);
    if (returnedBytes == 1) {
        charToReturn = buffer.get()[0];
    }
    return charToReturn;
#else
    std::unique_ptr<unsigned char> buffer{new unsigned char[SERIAL_PORT_BUFFER_MAX]};
    unsigned char charToReturn{0};
    long int returnedBytes{::read(this->m_serialPort[this->m_portNumber], buffer.get(), 1)};
    if(returnedBytes < 0) {
        if(errno == EAGAIN)  {
            return charToReturn;
        }
    } else if (returnedBytes == 1) {
        charToReturn = buffer.get()[0];
    } else {
        return charToReturn;
    }

    return charToReturn;
#endif
}

unsigned char SerialPort::timedRead()
{
    unsigned char byteRead{0};
    SteadyEventTimer eventTimer;
    eventTimer.start();
    do {
        byteRead = this->rawRead();
        if (byteRead != 0) {
            return byteRead;
        }
        eventTimer.update();
    } while (eventTimer.totalMilliseconds() < this->m_timeout);
    return 0;
}

ssize_t SerialPort::write(char byteToSend)
{
    return this->writeByte(byteToSend);
}

ssize_t SerialPort::write(const uint8_t *message, uint8_t messageLength)
{
    ssize_t bytesWritten{0};
    for (int i = 0; i < messageLength; i++) {
        this->writeByte(static_cast<char>(message[i]));
        bytesWritten++;
    }
    return bytesWritten;
}


ssize_t SerialPort::writeByte(char byteToSend)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    long int writtenBytes{0};
    WriteFile(this->m_serialPort[this->m_portNumber], &byteToSend, 1, (LPDWORD)((void *)&writtenBytes), NULL);
    return ( (writtenBytes < 0) ? 1 : 0);
#else
    long int writtenBytes{::write(this->m_serialPort[this->m_portNumber], &byteToSend, 1)};
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#endif
}

ssize_t SerialPort::writeBufferedBytes(unsigned char *buffer, long bufferSize)
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    long int writtenBytes;
    return (WriteFile(this->m_serialPort[this->m_portNumber], buffer, bufferSize, (LPDWORD)((void *)&writtenBytes), NULL) ? writtenBytes : -1);
#else
    long int writtenBytes{::write(this->m_serialPort[this->m_portNumber], buffer, bufferSize)};
    if(writtenBytes < 0) {
        return (errno == EAGAIN ? 0 : 1);
    }
    return writtenBytes;
#endif
}


void SerialPort::closePort()
{
    this->m_shutEmDown = true;
    this->m_isListening = false;
    if (!this->isOpen()) {
        return;
    }
    #if (defined(_WIN32) || defined(__CYGWIN__))
        CloseHandle(this->m_serialPort[this->m_portNumber]);
        this->m_isOpen = false;
    #else
        int status{0};
        if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
            //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
        }
        status &= ~TIOCM_DTR;    /* turn off DTR */
        status &= ~TIOCM_RTS;    /* turn off RTS */
        if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
            //std::cout << "WARNING: Unable to get port status while closing serial port " << this->m_portName << std::endl;
        }

        tcsetattr(this->m_serialPort[this->m_portNumber], TCSANOW, this->m_oldPortSettings + this->m_portNumber);
        close(this->m_serialPort[this->m_portNumber]);
        flock(this->m_serialPort[this->m_portNumber], LOCK_UN);
        this->m_isOpen = false;
    #endif
}


void SerialPort::enableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], SETDTR);
#else
    int status{0};
    status |= TIOCM_DTR;    /* turn on DTR */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableDTR()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], CLRDTR);
#else
    int status{0};
    status &= ~TIOCM_DTR;    /* turn off DTR */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling DTR for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::enableRTS()
{

#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], SETRTS);
#else
    int status{0};
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
          std::cout << "WARNING: Unable to get port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
    status |= TIOCM_RTS;    /* turn on RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while enabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

void SerialPort::disableRTS()
{
#if (defined(_WIN32) || defined(__CYGWIN__))
    EscapeCommFunction(this->m_serialPort[this->m_portNumber], CLRRTS);
#else
    int status;
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status) == -1) {
        std::cout << "WARNING: Unable to get port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
    status &= ~TIOCM_RTS;    /* turn off RTS */
    if(ioctl(this->m_serialPort[this->m_portNumber], TIOCMSET, &status) == -1) {
        std::cout << "WARNING: Unable to set port status while disabling RTS for serial port " << this->m_portName << std::endl;
    }
#endif
}

bool SerialPort::isDCDEnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_RLSD_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_CAR);
    #endif
}


bool SerialPort::isCTSEnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_CTS_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_CTS);
    #endif
}

bool SerialPort::isDSREnabled() const
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        int status{0};
        GetCommModemStatus(this->m_serialPort[this->m_portNumber], (LPDWORD)((void *)&status));
        return ((status&MS_DSR_ON) != 0 ? true : false);
    #else
        int status{0};
        ioctl(this->m_serialPort[this->m_portNumber], TIOCMGET, &status);
        return (status&TIOCM_DSR);
    #endif
}

void SerialPort::flush()
{
    return this->flushTXRX();
}

void SerialPort::flushRX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_RXCLEAR | PURGE_RXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCIFLUSH);
    #endif
}


void SerialPort::flushTX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_TXCLEAR | PURGE_TXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCOFLUSH);
    #endif
}


void SerialPort::flushRXTX()
{
    #if (defined(_WIN32) || defined(__CYGWIN__))
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_RXCLEAR | PURGE_RXABORT);
        PurgeComm(this->m_serialPort[this->m_portNumber], PURGE_TXCLEAR | PURGE_TXABORT);
    #else
        tcflush(this->m_serialPort[this->m_portNumber], TCIOFLUSH);
    #endif
}

void SerialPort::flushTXRX()
{
    return this->flushRXTX();
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
    int i{0};
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (str == it) {
            return std::make_pair(i, str);
        }
        i++;
    }
    str = name;
    if (str.find("/dev/tty") == std::string::npos) {
        str = "/dev/tty" + str;
    }
    i = 0;
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (str == it) {
            return std::make_pair(i, str);
        }
        i++;
    }
    str = name;
    if (str.find("/dev/") == std::string::npos) {
        str = "/dev/" + str;
    }
    i = 0;
    for (auto &it : SerialPort::SERIAL_PORT_NAMES) {
        if (str == it) {
            return std::make_pair(i, str);
        }
        i++;
    }

    throw std::runtime_error("ERROR: " + name + " is an invalid serial port name");
#endif
}

bool SerialPort::isOpen() const
{
    return this->m_isOpen;
}

ssize_t SerialPort::writeCString(const char *str)
{
    ssize_t writtenBytes{0};
    while (*str != 0) {
        writtenBytes += (this->writeByte(*(str++)));
    }
    return writtenBytes;
}


ssize_t SerialPort::writeLine(const std::string &str)
{
    std::string copyString{str};
    if (!SerialPort::endsWith(copyString, this->m_lineEnding)) {
        copyString += this->m_lineEnding;
    }
    return this->writeCString(copyString.c_str());
}

ssize_t SerialPort::writeLine(const char *str)
{
    return this->writeLine(std::string{str});
}

ssize_t SerialPort::writeLine(char chr)
{
    return this->writeLine(std::string{1, chr});
}

std::string SerialPort::readLine()
{
    SteadyEventTimer eventTimer;
    std::string returnString{""};
    eventTimer.start();
    do {
        this->syncStringListener();
        if (this->m_stringBuilderQueue.find(this->m_lineEnding) != std::string::npos) {
            returnString = this->m_stringBuilderQueue.substr(0, this->m_stringBuilderQueue.find(this->m_lineEnding));
            this->m_stringBuilderQueue = this->m_stringBuilderQueue.substr(this->m_stringBuilderQueue.find(this->m_lineEnding) + this->m_lineEnding.length());
            return returnString;
        }
        eventTimer.update();
    } while (eventTimer.totalMilliseconds() < this->m_timeout);
    return returnString;
}

std::string SerialPort::readUntil(char readUntil)
{
    return this->readUntil(std::string{1, readUntil});
}

std::string SerialPort::readUntil(const char *readUntil)
{
    return this->readUntil(std::string{readUntil});
}

std::string SerialPort::readUntil(const std::string &str)
{
    std::string tempLineEnding{this->m_lineEnding};
    this->m_lineEnding = str;
    std::string readString{this->readLine()};
    this->m_lineEnding = tempLineEnding;
    return readString;
}


void SerialPort::setPortName(const std::string &name)
{
    if (!this->m_isOpen) {
        std::pair<int, std::string> truePortNameAndNumber{getPortNameAndNumber(name)};
        this->m_portNumber = truePortNameAndNumber.first;
        this->m_portName = truePortNameAndNumber.second;
    } else {
        throw std::runtime_error("ERROR: Cannot change port handle(name) while serial port " + this->m_portName + " is open");
    }
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

void SerialPort::setLineEnding(const std::string &lineEnding)
{
    if ((lineEnding.length() == 0) || (SerialPort::isWhitespace(lineEnding))) {
        throw std::runtime_error("ERROR: Cannot set SerialPort::lineEnding() to whitespace or an empty string");
    }
    this->m_lineEnding = lineEnding;
}

bool SerialPort::isWhitespace(const std::string &stringToCheck)
{
    for (std::string::const_iterator iter = stringToCheck.begin(); iter != stringToCheck.end(); iter++) {
        if (static_cast<unsigned>(*iter) > ASCII_WHITESPACE_MAXIMUM_VALUE) {
            return false;
        } else if (((*iter) != '\r') || ((*iter) != '\n')) {
            return false;
        }
    }
    return true;
}

bool SerialPort::isWhitespace(char charToCheck)
{
    return SerialPort::isWhitespace(std::string{1, charToCheck});
}

bool SerialPort::endsWith(const std::string &stringToCheck, const std::string &matchString)
{
    if (matchString.size() > stringToCheck.size()) {
        return false;
    }
    return std::equal(matchString.rbegin(), matchString.rend(), stringToCheck.rbegin());
}

bool SerialPort::endsWith(const std::string &stringToCheck, char matchChar)
{
    return SerialPort::endsWith(stringToCheck, std::string{1, matchChar});
}

bool SerialPort::fileExists(const std::string &fileToCheck)
{
    #if defined(_WIN32) && !defined(__CYGWIN__)
        std::ifstream readFromFile;
        readFromFile.open(fileToCheck);
        if (readFromFile.good()) {
            readFromFile.close();
            return true;
        } else {
            return false;
        }
    #else
        return (access(fileToCheck.c_str(),F_OK) != -1);
    #endif
}

bool SerialPort::fileExists(const char *fileToCheck)
{
    return fileExists(std::string{fileToCheck});
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

std::string SerialPort::lineEnding() const
{
    return this->m_lineEnding;
}

long SerialPort::timeout() const
{
    return this->m_timeout;
}

long SerialPort::retryCount() const
{
    return this->m_retryCount;
}

void SerialPort::setTimeout(long timeout)
{
    this->m_timeout = timeout;
}

void SerialPort::setRetryCount(long retryCount)
{
    this->m_retryCount = retryCount;
}

int SerialPort::portNumber() const
{
    return this->m_portNumber;
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


void SerialPort::startAsyncListen()
{
    if (!this->m_isOpen) {
        return;
    }
    if (!this->m_isListening) {
        this->m_isListening = true;
        this->m_shutEmDown = false;
#if defined(__ANDROID__)
        if (this->m_asyncFuture) {
            delete this->m_asyncFuture;
        }
        this->m_asyncFuture = new std::thread{&SerialPort::asyncStringListener, this};
#else
        this->m_asyncFuture = std::async(std::launch::async,
                                         &SerialPort::asyncStringListener,
                                         this);
#endif
    }
}

void SerialPort::stopAsyncListen()
{
    this->m_shutEmDown = true;
    this->m_isListening = false;
}

bool SerialPort::isListening() const
{
    return this->m_isListening;
}

void SerialPort::asyncStringListener()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    do {
        unsigned char byteRead = this->timedRead();
        if (byteRead != 0) {
            ioMutexLock.lock();
            addToStringBuilderQueue(byteRead);
            ioMutexLock.unlock();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    } while (!this->m_shutEmDown);
}

void SerialPort::syncStringListener()
{
    std::unique_lock<std::mutex> ioMutexLock{this->m_ioMutex, std::defer_lock};
    long tempTimeout{this->m_timeout};
    long splitTimeout{this->m_timeout};
    EventTimer<std::chrono::steady_clock> eventTimer;
    eventTimer.start();
    do {
        this->setTimeout(splitTimeout);
        unsigned char byteRead{this->timedRead()};
        if ((tempTimeout - eventTimer.totalMilliseconds()) < 0) {
            splitTimeout = 0;
        } else {
            splitTimeout = tempTimeout - eventTimer.totalMilliseconds();
        }
        splitTimeout = (tempTimeout - eventTimer.totalMilliseconds());
        if (byteRead != 0) {
            ioMutexLock.lock();
            addToStringBuilderQueue(byteRead);
            ioMutexLock.unlock();
        } else {
            break;
        }
    } while (eventTimer.totalMilliseconds() <= this->m_timeout);
    this->m_timeout = tempTimeout;
}

void SerialPort::addToStringBuilderQueue(unsigned char byte)
{
    if (this->m_stringBuilderQueue.size() >= SINGLE_MESSAGE_BUFFER_MAX) {
        this->m_stringBuilderQueue = this->m_stringBuilderQueue.substr(1);
    }
    this->m_stringBuilderQueue += static_cast<char>(byte);
}

ssize_t SerialPort::available()
{
    this->syncStringListener();
    return this->m_stringBuilderQueue.size();
}

std::string SerialPort::peek()
{
    this->syncStringListener();
    return "";
}

char SerialPort::peekByte()
{
    this->syncStringListener();
    return 0;
}

unsigned char SerialPort::readByte()
{
    if (this->m_stringBuilderQueue.empty()) {
        return this->rawRead();
    } else {
        unsigned char returnChar{static_cast<unsigned char>(this->m_stringBuilderQueue.front())};
        this->m_stringBuilderQueue = this->m_stringBuilderQueue.substr(1);
        return returnChar;
    }
}


void SerialPort::putBack(char back)
{
    return SerialPort::putBack(std::string{1, back});
}

void SerialPort::putBack(const char *str)
{
    return SerialPort::putBack(std::string{str});
}

void SerialPort::putBack(const std::string &str)
{
    this->m_stringBuilderQueue = str + this->m_stringBuilderQueue;
}

BaudRate SerialPort::parseBaudRateFromRaw(const char *baudRate) { return parseBaudRateFromRaw(std::string{baudRate}); }
DataBits SerialPort::parseDataBitsFromRaw(const char *dataBits) { return parseDataBitsFromRaw(std::string{dataBits}); }
StopBits SerialPort::parseStopBitsFromRaw(const char *stopBits) { return parseStopBitsFromRaw(std::string{stopBits}); }
Parity SerialPort::parseParityFromRaw(const char *parity) { return parseParityFromRaw(std::string{parity}); }

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

std::string SerialPort::baudRateToString() const
{
    return SerialPort::baudRateToString(this->m_baudRate);
}

std::string SerialPort::stopBitsToString() const
{
    return SerialPort::stopBitsToString(this->m_stopBits);
}

std::string SerialPort::dataBitsToString() const
{
    return SerialPort::dataBitsToString(this->m_dataBits);
}

std::string SerialPort::parityToString() const
{
    return SerialPort::parityToString(this->m_parity);
}

std::vector<const char *> SerialPort::availableBaudRates()
{      
    return SerialPort::AVAILABLE_BAUD_RATE;
}

std::vector<const char *> SerialPort::availableParity()
{
    return SerialPort::AVAILABLE_PARITY;
}

std::vector<const char *> SerialPort::availableStopBits()
{
    return SerialPort::AVAILABLE_STOP_BITS;
}

std::vector<const char *> SerialPort::availableDataBits()
{
    return SerialPort::AVAILABLE_DATA_BITS;
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
        if (SerialPort::fileExists(it)) {
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
            returnVector.push_back(it + std::to_string(i));
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
                if (serialPortName == (it + std::to_string(i))) {
                    return true;
                }
            }
        }
        return false;
    #endif
}

std::string SerialPort::doUserSelectSerialPortName()
{
    return GeneralUtilities::doUserSelectParameter<std::string>(std::string{"serial port"}, 
                                             [](const std::string &str) -> std::string { return str; },
                                             SerialPort::availableSerialPorts(),
                                             ""); 
}

BaudRate SerialPort::doUserSelectBaudRate()
{
    std::function<BaudRate(const std::string &)> tempFunc{static_cast<BaudRate (*)(const std::string &)>(&SerialPort::parseBaudRateFromRaw)};
    return GeneralUtilities::doUserSelectParameter<BaudRate>(std::string{"baud rate"}, 
                                                        tempFunc,
                                                        SerialPort::availableBaudRates(),
                                                        SerialPort::DEFAULT_BAUD_RATE_STRING.c_str());
}

StopBits SerialPort::doUserSelectStopBits()
{
    return GeneralUtilities::doUserSelectParameter<StopBits>(std::string{"stop bits"}, 
                                             static_cast<StopBits (*)(const std::string &)>(&SerialPort::parseStopBitsFromRaw),
                                             SerialPort::availableStopBits(),
                                             SerialPort::DEFAULT_STOP_BITS_STRING.c_str());
}

DataBits SerialPort::doUserSelectDataBits()
{
    return GeneralUtilities::doUserSelectParameter<DataBits>(std::string{"data bits"}, 
                                             static_cast<DataBits (*)(const std::string &)>(&SerialPort::parseDataBitsFromRaw),
                                             SerialPort::availableDataBits(),
                                             SerialPort::DEFAULT_DATA_BITS_STRING.c_str());
}

Parity SerialPort::doUserSelectParity()
{
    return GeneralUtilities::doUserSelectParameter<Parity>(std::string{"parity"}, 
                                             static_cast<Parity (*)(const std::string &)>(&SerialPort::parseParityFromRaw),
                                             SerialPort::availableParity(),
                                             SerialPort::DEFAULT_PARITY_STRING.c_str());
}

std::shared_ptr<SerialPort> SerialPort::doUserSelectSerialPort()
{
    try {
        std::string serialPortName{SerialPort::doUserSelectSerialPortName()};
        BaudRate baudRate{SerialPort::doUserSelectBaudRate()};
        DataBits dataBits{SerialPort::doUserSelectDataBits()};
        StopBits stopBits{SerialPort::doUserSelectStopBits()};
        Parity parity{SerialPort::doUserSelectParity()};
        std::shared_ptr<SerialPort> serialPort{std::make_shared<SerialPort>(serialPortName,
                                                                            baudRate,
                                                                            dataBits,
                                                                            stopBits,
                                                                            parity)};
        return serialPort;
    } catch (std::exception &e) {
        (void)e;
        return nullptr;
    }            
}
