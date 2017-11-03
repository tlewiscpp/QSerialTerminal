/***********************************************************************
*    ibytestream.h:                                                    *
*    IByteStream, base class for simple read and write operations      *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a header file for CppSerialPort:                          *
*    https://github.serial/tlewiscpp/CppSerialPort                     *
*    This file may be distributed with the CppSerialPort library,      *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an base class IByteStream     *
*    It includes very basic IO operations like read and write          *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with CppSerialPort                           *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef CPPSERIALPORT_IBYTESTREAM_H
#define CPPSERIALPORT_IBYTESTREAM_H

#include <string>
#include <sstream>
#include <mutex>

namespace CppSerialPort {

class IByteStream
{
public:
    IByteStream();
    virtual ~IByteStream() = default;

    virtual int read() = 0;
    virtual ssize_t write(int) = 0;

    virtual std::string portName() const = 0;
    virtual bool isOpen() const = 0;
    virtual void openPort() = 0;
    virtual void closePort() = 0;
    virtual void flushRx() = 0;
    virtual void flushTx() = 0;

    bool available();
    int peek();
    void setReadTimeout(int timeout);
    int readTimeout() const;

    std::string lineEnding() const;
    void setLineEnding(const std::string &str);
    void setLineEnding(char c);

    virtual ssize_t writeLine(const std::string &str);

    virtual std::string readLine(bool *timeout = nullptr);
    virtual std::string readUntil(const std::string &until, bool *timeout = nullptr);
    virtual std::string readUntil(char until, bool *timeout = nullptr);

protected:
    virtual void putBack(int c) = 0;

    bool endsWith (const std::string &fullString, const std::string &ending);

    template<typename T> static inline std::string toStdString(const T &t) {
        return dynamic_cast<std::ostringstream &>(std::ostringstream{""} << t).str();
    }

    static inline std::string vaToStdString() {
        return "";
    }

    template <typename First, typename ...Args> static inline std::string vaToStdString(First first, Args ...args) {
        return toStdString(first) + vaToStdString(args...);
    }

    static bool fileExists(const std::string &filePath);

private:
    int m_readTimeout;
    std::string m_lineEnding;
    std::mutex m_writeMutex;

    static uint64_t getEpoch();


    static const char *DEFAULT_LINE_ENDING;
    static const int DEFAULT_READ_TIMEOUT;

    ssize_t write(const std::string &str);

};

} //namespace CppSerialPort

#endif //CPPSERIALPORT_IBYTESTREAM_H
