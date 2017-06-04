/***********************************************************************
*    tstream.h:                                                        *
*    TStream, abstract base class for simple read and write operations *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of an abstract base class TSream *
*    It includes very basic IO operations like read and write          *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_TSTREAM_H
#define TJLUTILS_TSTREAM_H

#include <iostream>
#include <string>

enum class TStreamCommandType { DELAY_SECONDS, DELAY_MILLISECONDS, DELAY_MICROSECONDS, WRITE, READ, FLUSH_RX, FLUSH_TX, FLUSH_RX_TX, LOOP_START, LOOP_END, COMMAND_UNSPECIFIED };
enum class DelayType { SECONDS, MILLISECONDS, MICROSECONDS };
enum class FlushType { RX, TX, RX_TX };
enum class LoopType { START, END };

class TStreamCommand
{
public:
    TStreamCommand(TStreamCommandType commandType, const std::string &commandArgument) :
        m_commandType{commandType},
        m_commandArgument{commandArgument} { }
    
    TStreamCommandType commandType() const { return this->m_commandType; }
    std::string commandArgument() const { return this->m_commandArgument; }
    void setCommandType(const TStreamCommandType &commandType) { this->m_commandType = commandType; }
    void setCommandArgument(const std::string &commandArgument) { this->m_commandArgument = commandArgument; }

private:
    TStreamCommandType m_commandType;
    std::string m_commandArgument;
};


#if defined(_WIN32) && !defined(__CYGWIN__)
    //using ssize_t = long;
#endif

class TStream
{
public:
    virtual ~TStream() { }

    virtual void setTimeout(long timeout) = 0;
    virtual long timeout() const = 0;
    virtual std::string lineEnding() const = 0;
    virtual void setLineEnding(const std::string &str) = 0;

    virtual ssize_t writeLine(const std::string &str) = 0;
    virtual ssize_t writeLine(const char *str) = 0;
    virtual ssize_t available() = 0;
    virtual bool isOpen() const = 0;
    virtual void openPort() = 0;
    virtual void closePort() = 0;

    virtual std::string portName() const = 0;
    virtual void flushRX() = 0;
    virtual void flushTX() = 0;
    virtual void flushRXTX() = 0;

    virtual std::string peek() = 0;
    virtual char peekByte() = 0;

    virtual void putBack(const std::string &str) = 0;
    virtual void putBack(const char *str) = 0;
    virtual void putBack(char back) = 0;

    virtual std::string readLine() = 0;
    virtual std::string readUntil(const std::string &until) = 0;
    virtual std::string readUntil(const char *until) = 0;
    virtual std::string readUntil(char until) = 0;

};

const char * const DELAY_IDENTIFIER{"delay"};
const char * const DELAY_SECONDS_IDENTIFIER{"delayseconds("};
const char * const DELAY_MILLISECONDS_IDENTIFIER{"delaymilliseconds("};
const char * const DELAY_MICROSECONDS_IDENTIFIER{"delaymicroseconds("};
const char * const WRITE_IDENTIFIER{"write("};
const char * const READ_IDENTIFIER{"read("};
const char * const LOOP_IDENTIFIER{"loop("};
const char * const FLUSH_IDENTIFIER{"flush"};
const char * const NO_CLOSING_PARENTHESIS_FOUND_STRING{"    No matching parenthesis found, ignoring option"};
const char * const NO_CLOSING_QUOTATION_MARKS_FOUND_STRING{"    No matching quotation marks found, ingoring option"};
const char * const NO_PARAMETER_SEPARATING_COMMA_STRING{"    No parameter separating comma found, ignoring option"};
const char * const NO_LOOP_COUNT_SPECIFIED_STRING{"    Loop() parameter must include an integer parameter (the number of loops to execute), ignoring option"};
const char * const EXPECTED_HERE_STRING{"^---expected here"};
const char * const HERE_STRING{"^---here"};
const char * const WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING{"    Write() parameter must be enclosed in parentheses, ignoring option"};
const char * const DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelaySeconds() parameter is not an integer, ignoring option"};
const char * const DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMilliseconds() parameter is not an integer, ignoring option"};
const char * const DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMicroseconds() parameter is not an integer, ignoring option"};
const char * const GENERIC_CONFIG_WARNING_BASE_STRING{"WARNING: line "};
const char * const GENERIC_CONFIG_WARNING_TAIL_STRING{" of configuration file:"};
const char * const CONFIG_EXPRESSION_MALFORMED_STRING{"    expression is malformed/has invalid syntax, ignoring option"};
const char * const EXCEPTION_IN_CONSTRUCTOR_STRING{"WARNING: Standard exception caught in TScriptFileReader constructor: "};
const char * const COMMAND_TYPE_NOT_IMPLEMENTED_STRING{"WARNING: Command type not implemented, skipping command: "};
const char * const NULL_IO_STREAM_PASSED_TO_EXECUTE_STRING{"WARNING: Null TStream passed to TScriptExecutor::execute(std::shared_ptr<TStream>), skipping script execution"};
const char * const SCRIPT_FILE_DOES_NOT_EXISTS_STRING{"WARNING: Script file does not exist, skipping script: "};
const char * const UNABLE_TO_OPEN_SCRIPT_FILE_STRING{"WARNING: Unable to open script file, skipping script: "};
const char * const LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING{"LoopCount() parameter is not an integer, ignoring option"};
const char * const UNTERMINATED_LOOP_STRING{"WARNING: The script contains an unterminated loop,  skipping script execution"};
const char * const UNEXPECTED_LOOP_CLOSING_STRING{"WARNING: A loop closure was found, but no loop was currently being populated, ignoring option"};
const char * const CLOSING_LOOP_IDENTIFIER{"}"};


#endif //TJLUTILS_TSTREAM_H
