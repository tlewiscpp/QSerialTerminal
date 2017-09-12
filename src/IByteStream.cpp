/***********************************************************************
*    ibytestream.cpp:                                                  *
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
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include <sstream>
#include <zconf.h>
#include "IByteStream.h"

namespace CppSerialPort {

#if defined(_WIN32)
const char *IByteStream::DEFAULT_LINE_ENDING{"\r\n"};
#else
const char *IByteStream::DEFAULT_LINE_ENDING{"\n"};
#endif //defined(_WIN32)
const int IByteStream::DEFAULT_READ_TIMEOUT{1000};

IByteStream::IByteStream() :
        m_readTimeout{DEFAULT_READ_TIMEOUT},
        m_lineEnding{DEFAULT_LINE_ENDING}
{

}

void IByteStream::setReadTimeout(int timeout)
{
    if (timeout < 0) {
        throw std::runtime_error("IByteStream::setReadTimeout < 0 (invariant failure)");
    }
    this->m_readTimeout = timeout;
}

int IByteStream::readTimeout() const
{
    return this->m_readTimeout;
}

std::string IByteStream::lineEnding() const
{
    return this->m_lineEnding;
}

void IByteStream::setLineEnding(const std::string &str)
{
    if (str.length() == 0) {
        throw std::runtime_error("IByteStream::setLineEnding str.length() == 0 (invariant failure)");
    }
    this->m_lineEnding = str;
}

ssize_t IByteStream::writeLine(const std::string &str)
{
    ssize_t returnValue = 0;
    for (auto &it : str) {
        returnValue += this->write(static_cast<int>(it));
    }
    for (auto &it : this->m_lineEnding) {
        returnValue += this->write(static_cast<int>(it));
    }
    return returnValue;
}

ssize_t IByteStream::write(const std::string &str)
{
    ssize_t returnValue{0};
    for (auto &it : str) {
        returnValue += this->write(static_cast<int>(it));
    }
    return returnValue;
}
std::string IByteStream::readLine(bool *timeout)
{
    return this->readUntil(this->m_lineEnding, timeout);
}

std::string IByteStream::readUntil(const std::string &until, bool *timeout)
{
    uint64_t startTime{IByteStream::getEpoch()};
    std::stringstream returnString{""};
    do {
        int maybeChar{this->read()};
        if (maybeChar == 0) {
            continue;
        }
        returnString << static_cast<char>(maybeChar);
        size_t foundPosition{returnString.str().find(until)};
        if (foundPosition != std::string::npos) {
            return returnString.str().substr(0, foundPosition);
        }
    } while ((IByteStream::getEpoch() - startTime) <= static_cast<unsigned long>(this->m_readTimeout));
    if (timeout) {
        *timeout = true;
    }
    return returnString.str();
}

std::string IByteStream::readUntil(char until, bool *timeout)
{
    return this->readUntil(std::string{1, until}, timeout);
}

int IByteStream::peek()
{
    int readChar{this->read()};
    this->putBack(readChar);
    return readChar;
}

bool IByteStream::fileExists(const std::string &fileToCheck)
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
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (static_cast<unsigned long long>(tv.tv_sec) * 1000) +
           (static_cast<unsigned long long>(tv.tv_usec) / 1000);
}

bool IByteStream::available()
{
    return (this->peek() != '\0');
}

bool IByteStream::endsWith(const std::string &fullString, const std::string &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

#endif //defined(_MSC_VER)

} //namespace CppSerialPort
