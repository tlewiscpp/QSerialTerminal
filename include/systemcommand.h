/***********************************************************************
*    systemcommand.h:                                                  *
*    User defined class, SystemCommand, for a bash-like interface      *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations for a SystemCommand class        *
*    The class is made up entirely of static methods and constants     *
*    It is used to enable a bash-like iterface, allowing one to        *
*    retrieve the stdout/stderr from a call to the command interpreter *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_SYSTEMCOMMAND_H
#define TJLUTILS_SYSTEMCOMMAND_H

#if defined(_WIN32) && defined(_MSC_VER)
    #if _MSC_VER < 1700
        #error This library needs at least a C++11 compliant compiler
    #endif
#else
    #if __cplusplus <= 199711L
        #error This library needs at least a C++11 compliant compiler
    #endif
#endif

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <ctime>
#include <cstdio>
#include <cassert>
#include <memory>
#include <cstring>
#include <functional>
#include "generalutilities.h"

#ifndef PATH_MAX
    #define PATH_MAX 128
#endif

#if defined(_WIN32) || defined(_MSC_VERSION)
    #include <cstdlib>
/*
    #include <windows.h> 
    #include <assert.h>
    #include <tchar.h>
    #include <stdio.h> 
    #include <strsafe.h>
    #include <stdarg.h> 
    #include <stdlib.h> 
    #include <wtypes.h>
*/
#else
    #include <unistd.h>
#endif

class SystemCommand
{
public:
    
    SystemCommand();
    SystemCommand(std::initializer_list<std::string> command) : SystemCommand(*std::begin(command)) {}
    SystemCommand(const std::string &command);
    SystemCommand(const std::string &command, int maxOutput);
    
    void execute();
    void executeWithoutPipe();
    std::vector<std::string> executeAndWaitForOutputAsVector();
    std::string executeAndWaitForOutputAsString();
    void insertIntoCommand(int position, const std::string &stringToInsert);
    void insertIntoCommand(int position, char charToInsert);
    void printCommand();

    int maxOutput();

    std::string command();
    bool hasError();
    int returnValue();
    int sizeOfOutput();

    std::string outputAsString();
    std::vector<std::string> outputAsVector();
    
    void setCommand(const std::string &command);
    void appendToCommand(const std::string &stringToAppend);
    void setMaxOutput(int maxOutput);
    std::unique_ptr<wchar_t> getWC(const char *c);

    FILE *popenHandler(const char *args, const char *method);
    int pcloseHandler(FILE *fp);
    SystemCommand &operator+=(const std::string &rhs);

private:

    enum class PipeStatus {
        WITH_PIPE,
        WITHOUT_PIPE
    };
    int m_returnValue;
    std::string m_command;
    int m_maxOutput;
    bool m_hasError;
    int m_sizeOfOutput;
    bool m_repeatCommand;
    std::vector<std::string> m_outputAsVector;
    const static int  m_DEFAULT_MAX_OUTPUT;

    void verifyValidMaxOutput();
    void stripShellControlCharactersFromCommand();
    void systemCommandLaunch(PipeStatus pipe);
    std::string stripPipeFromCommand(const std::string &command);
};

#endif //TJLUTILS_SYSTEMCOMMAND_H
