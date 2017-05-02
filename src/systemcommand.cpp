/***********************************************************************
*    systemcommand.cpp:                                                *
*    User defined class, SystemCommand, for a bash-like interface      *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for tjlutils:                               *
*    https://github.com/Pinguinsan/tjlutils                            *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a SystemCommand class       *
*    The class is made up entirely of static methods and constants     *
*    It is used to enable a bash-like iterface, allowing one to        *
*    retrieve the stdout/stderr from a call to the command interpreter *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "systemcommand.h"

const int SystemCommand::m_DEFAULT_MAX_OUTPUT = 2147483647;

SystemCommand::SystemCommand() : 
    m_command{""},
    m_maxOutput{m_DEFAULT_MAX_OUTPUT},
    m_hasError{false},
    m_repeatCommand{false}
{

}

SystemCommand::SystemCommand(const std::string &command) : 
    m_command{command},
    m_maxOutput{m_DEFAULT_MAX_OUTPUT},
    m_hasError{false},
    m_repeatCommand{false}
{

}

SystemCommand::SystemCommand(const std::string &command, int maxOutput) :
    m_command{command},
    m_maxOutput{maxOutput},
    m_hasError{false},
    m_repeatCommand{false}
{

}

SystemCommand &SystemCommand::operator+=(const std::string &rhs)
{
    this->m_command += rhs;
    return *this;
}

void SystemCommand::printCommand()
{
    std::cout << stripPipeFromCommand(this->m_command) << std::endl;
}

void SystemCommand::insertIntoCommand(int position, const std::string &stringToInsert)
{
    if (static_cast<unsigned int>(position) > this->m_command.length()) {
        return;
    }
    this->m_command.insert(position, stringToInsert);
    this->m_repeatCommand = false;
}

void SystemCommand::insertIntoCommand(int position, char charToInsert)
{
    std::string temp = "";
    temp += charToInsert;
    this->insertIntoCommand(position, temp);
}

int SystemCommand::maxOutput()
{
    return this->m_maxOutput;
}

std::string SystemCommand::command()
{
    return stripPipeFromCommand(this->m_command);
}

bool SystemCommand::hasError()
{
    return this->m_hasError;
}

void SystemCommand::appendToCommand(const std::string &stringToAppend)
{
    m_command += stringToAppend;
    this->m_repeatCommand = false;
}

int SystemCommand::returnValue()
{
    return this->m_returnValue;
}

void SystemCommand::setCommand(const std::string &command)
{
    this->m_repeatCommand = false;
    this->m_command = command;
    this->m_hasError = false;
    this->m_sizeOfOutput = 0;
    this->m_outputAsVector.clear();
}

void SystemCommand::setMaxOutput(int maxOutput)
{
    this->m_maxOutput = maxOutput;
}

std::string SystemCommand::outputAsString()
{
    std::string returnString = "";
    for (auto &it : this->m_outputAsVector) {
        returnString.append(it);
    }
    return returnString;
}

void SystemCommand::stripShellControlCharactersFromCommand()
{
    //TODO: Implement
    return;
}

std::vector<std::string> SystemCommand::outputAsVector()
{
    return this->m_outputAsVector;
}

void SystemCommand::execute()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
}

std::vector<std::string> SystemCommand::executeAndWaitForOutputAsVector()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
    return this->m_outputAsVector;
}

void SystemCommand::executeWithoutPipe()
{
    this->systemCommandLaunch(PipeStatus::WITHOUT_PIPE);
}

std::string SystemCommand::stripPipeFromCommand(const std::string &command)
{
    using namespace GeneralUtilities;
    std::string returnString{command};
    #if defined(_WIN32) && defined(_MSC_VER)
        returnString = stripAllFromString(this->m_command, "2>&1");
        returnString = stripAllFromString(this->m_command, ">");
    #else
        returnString = stripAllFromString(this->m_command, "2>&1");
        returnString = stripAllFromString(this->m_command, ">");
    #endif
    return returnString;
}


std::string SystemCommand::executeAndWaitForOutputAsString()
{
    this->systemCommandLaunch(PipeStatus::WITH_PIPE);
    return this->outputAsString();
}

int SystemCommand::sizeOfOutput()
{
    return this->m_sizeOfOutput;
}

void SystemCommand::verifyValidMaxOutput()
{
    if ((m_maxOutput <= 8) || (m_maxOutput > (std::numeric_limits<int>::max)())) {
        m_maxOutput = m_DEFAULT_MAX_OUTPUT;
    }
}

std::unique_ptr<wchar_t> SystemCommand::getWC(const char *c)
{
    const size_t cSize = strlen(c)+1;
    std::unique_ptr<wchar_t> wc{new wchar_t[cSize]};
    mbstowcs (wc.get(), c, cSize);
    return std::move(wc);
}

FILE *SystemCommand::popenHandler(const char *args, const char *method)
{
    #ifdef _WIN32
        return _popen(args, method);
    #else
        return popen(args, method);
    #endif
}

int SystemCommand::pcloseHandler(FILE *fp)
{
    #ifdef _WIN32
        return _pclose(fp);
    #else
        return pclose(fp);
    #endif
}

void SystemCommand::systemCommandLaunch(PipeStatus pipe)
{
    using namespace GeneralUtilities;
    this->m_command = stripPipeFromCommand(this->m_command);
    if (this->m_repeatCommand) {
        this->m_hasError = false;
        this->m_sizeOfOutput = 0;
        this->m_outputAsVector.clear();
    } else {
        this->m_repeatCommand = true;
    }
	verifyValidMaxOutput();
    if (pipe == PipeStatus::WITH_PIPE) {
        this->m_command += " 2>&1"; //Merges stderror with stdout
        FILE *fp;
        char path[PATH_MAX];

        fp = popenHandler(this->m_command.c_str(), "r");
        if (fp == NULL) {
            std::cout << "ERROR: Failed to execute command " << GeneralUtilities::tQuoted(this->m_command) << std::endl;
            this->m_returnValue = -1;
            return;
        }
        int outputSize{ 0 };
        while ((fgets(path, PATH_MAX, fp) != NULL) && (outputSize <= this->m_maxOutput)) {
            std::string formattedPath{static_cast<std::string>(path)};
            addFormattedThing(this->m_outputAsVector, formattedPath, [](const std::string &stringToStrip) -> std::string
            {
                std::string returnString{ stringToStrip };
                std::vector<std::string> newLines{ "\r\n", "\n\r", "\n" };
                for (std::vector<std::string>::const_iterator iter = newLines.begin(); iter != newLines.end(); iter++) {
                    if (returnString.find(*iter) != std::string::npos) {
                        size_t foundPosition = returnString.find(*iter);
                        returnString = returnString.substr(0, foundPosition);
                    }
                }
                return returnString;
            });
            outputSize += sizeof(*(std::end(this->m_outputAsVector)-1));
        }
        this->m_returnValue = pcloseHandler(fp);
    } else {
        this->m_command = stripPipeFromCommand(this->m_command);
        #if defined(_WIN32) || defined(__CYGWIN__)
            this->m_returnValue = system(this->m_command.c_str());
        #else
            this->m_returnValue = system(this->m_command.c_str())/256;
        #endif
    }
    this->m_hasError = (this->m_returnValue != 0);
}

/*
#if defined(_WIN32) || defined(__CYGWIN__)

std::string SystemCommand::asyncReadFromPipe() 
{ 
    DWORD bytesAvail = 0;
    if (!PeekNamedPipe(g_hChildStd_OUT_Rd, NULL, 0, NULL, &bytesAvail, NULL)) {
        return "";
    }
    if (bytesAvail) {
        DWORD dwRead{0};
        char chBuf[BUFSIZE]; 
        bool bSuccess{FALSE};
        std::string returnString{""};
        bSuccess = ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
        chBuf[dwRead] = '\0';
        if ((!bSuccess) || (dwRead == 0)) {
            return "";
        } else {
            return std::string{chBuf};
        }
    } else {
        return "";
    }
} 


std::vector<std::string> SystemCommand::readFromPipe()
{
    std::vector<std::string> returnVector;
    std::future<std::string> readTask{std::async(std::launch::async, asyncReadFromPipe)};
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    auto elapsedTime{std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count()};
    do {
        if (readTask.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            std::string returnString{readTask.get()};
            if (returnString != "") {
                returnVector.emplace_back(returnString);
                startTime = std::chrono::high_resolution_clock::now();
                endTime = std::chrono::high_resolution_clock::now();
                elapsedTime = 0;
            }
            readTask = std::async(std::launch::async, asyncReadFromPipe);
        }
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= 100);
    return returnVector;
}

int SystemCommand::createChildProcess()
{ 
    TCHAR szCmdLine[]=TEXT();
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE; 

    // Set up members of the PROCESS_INFORMATION structure. 
    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    // Set up members of the STARTUPINFO structure. 
    // This structure specifies the STDIN and STDOUT handles for redirection.
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.dwFlags = STARTF_USESHOWWINDOW;
    siStartInfo.wShowWindow = SW_HIDE;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcess(NULL,
        szCmdLine, 
        NULL, 
        NULL, 
        TRUE, 
        CREATE_NO_WINDOW, 
        NULL, 
        NULL, 
        &siStartInfo, 
        &piProcInfo);

   // If an error occurs, exit the application. 
    if ( ! bSuccess ) { 
        return 1;
    } else {
        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example. 
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        return 0;
    }
}

int SystemCommand::doWindowsChildProcessRead()
{
    HANDLE g_hChildStd_OUT_Rd{NULL};
    HANDLE g_hChildStd_OUT_Wr{NULL};



}

#endif
*/