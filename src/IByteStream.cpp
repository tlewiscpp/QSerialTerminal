/***********************************************************************
*    IByteStream.cpp:                                                  *
*    IByteStream, base class for simple read and write operations      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.com/tlewiscpp/CppSerialPort                        *
*    This file may be distributed with the CppSerialPort library,      *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementaiton for the methods used in        *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <sstream>
#include "IByteStream.h"
#include <fstream>

namespace CppSerialPort {

#if defined(_WIN32)
const char *IByteStream::DEFAULT_LINE_ENDING{"\r\n"};
#    include <Windows.h>
#    include <shlwapi.h>
#else
#    include <unistd.h>
const char *IByteStream::DEFAULT_LINE_ENDING{"\n"};
#endif //defined(_WIN32)

const int IByteStream::DEFAULT_READ_TIMEOUT{1000};
const int IByteStream::DEFAULT_WRITE_TIMEOUT{1000};

IByteStream::IByteStream() :
	m_readTimeout{ DEFAULT_READ_TIMEOUT },
	m_writeTimeout{ DEFAULT_WRITE_TIMEOUT },
	m_lineEnding{ DEFAULT_LINE_ENDING },
	m_writeMutex{},
	m_readMutex{}
{

}

void IByteStream::setReadTimeout(int timeout)
{
    if (timeout < 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setReadTimeout(int): invariant failure (read timeout cannot be less than 0, " + toStdString(timeout) + " < 0)");
    }
    this->m_readTimeout = timeout;
}

int IByteStream::readTimeout() const
{
    return this->m_readTimeout;

}
void IByteStream::setWriteTimeout(int timeout)
{
    if (timeout < 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setWriteTimeout(int): invariant failure (write timeout cannot be less than 0, " + toStdString(timeout) + " < 0)");
    }
    this->m_writeTimeout = timeout;
}

int IByteStream::writeTimeout() const
{
    return this->m_writeTimeout;
}

std::string IByteStream::lineEnding() const
{
    return this->m_lineEnding;
}

void IByteStream::setLineEnding(const std::string &str)
{
    if (str.length() == 0) {
        throw std::runtime_error("CppSerialPort::IByteStream::setLineEnding str.length() == 0 (invariant failure)");
    }
    this->m_lineEnding = str;
}

void IByteStream::setLineEnding(char chr)
{
    if (chr == '\0') {
        throw std::runtime_error("CppSerialPort::IByteStream::setLineEnding chr == '\\0' (invariant failure)");
    }
    this->m_lineEnding = std::string(1, chr);
}

ssize_t IByteStream::writeLine(const std::string &str)
{
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
	std::string toWrite{ str + this->lineEnding() };
	return this->write(toWrite.c_str(), toWrite.length());
}

ssize_t IByteStream::write(const std::string &str)
{
    std::lock_guard<std::mutex> writeLock{this->m_writeMutex};
	return this->write(str.c_str(), str.length());
}

std::string IByteStream::readLine(bool *timeout)
{
    return this->readUntil(this->m_lineEnding, timeout);
}

std::string IByteStream::readUntil(const std::string &until, bool *timeout)
{
	std::lock_guard<std::mutex> readLock{ this->m_readMutex };
    uint64_t startTime{IByteStream::getEpoch()};
    std::string returnString{""};
    if (timeout) {
        *timeout = false;
    }
    do {
        int maybeChar{this->read()};
        if (maybeChar == 0) {
            continue;
        }
        returnString += static_cast<char>(maybeChar);
        if (endsWith(returnString, until)) {
            return returnString.substr(0, returnString.length() - until.length());
        }
    } while ((IByteStream::getEpoch() - startTime) <= static_cast<unsigned long>(this->m_readTimeout));
    if (timeout) {
        *timeout = true;
    }
    return returnString;
}

std::string IByteStream::readUntil(char until, bool *timeout)
{
    return this->readUntil(std::string(1, until), timeout);
}

int IByteStream::peek()
{
    int readChar{this->read()};
    this->putBack(readChar);
    return readChar;
}


bool IByteStream::available()
{
    return (this->peek() != '\0');
}

bool IByteStream::fileExists(const std::string &fileToCheck)
{
#if defined(_WIN32)
    std::ifstream readFromFile{};
    readFromFile.open(fileToCheck.c_str());
    return readFromFile.is_open();
#else
    return (access(fileToCheck.c_str(),F_OK) != -1);
#endif
}


#if defined(_MSC_VER)
#include <Windows.h>
uint64_t IByteStream::getEpoch()
{
    /*
     * Are you fucking kidding me Windows?
     * https://stackoverflow.com/a/1695332/4791654
     */
    FILETIME ft_now{};
    LONGLONG ll_now{(LONGLONG)ft_now.dwLowDateTime + ((LONGLONG)(ft_now.dwHighDateTime) << 32LL)};
    LONGLONG win32Epoch{ll_now/10000};
    LONGLONG unixEpoch{win32Epoch - 116444736000000000LL};
    return static_cast<uint64_t>(unixEpoch);
}
#else
#include <sys/time.h>
uint64_t IByteStream::getEpoch()
{
    struct timeval tv{};
    gettimeofday(&tv, NULL);
    return (static_cast<unsigned long long>(tv.tv_sec) * 1000) +
           (static_cast<unsigned long long>(tv.tv_usec) / 1000);
}

#endif //defined(_WIN32)

} //namespace CppSerialPort
