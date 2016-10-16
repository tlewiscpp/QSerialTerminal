#include "scriptexecutor.h"

ScriptExecutor::ScriptExecutor(const std::string &scriptFilePath) :
    m_scriptReader{std::make_shared<ScriptReader>(scriptFilePath)}
{

}

void ScriptExecutor::setScriptFilePath(const std::string &scriptFilePath)
{
    this->m_scriptReader.reset();
    this->m_scriptReader = std::make_shared<ScriptReader>(scriptFilePath);
}

std::shared_ptr<ScriptReader> ScriptExecutor::scriptReader() const
{
    return this->m_scriptReader;
}

void ScriptExecutor::execute(MainWindow *mainWindow, std::shared_ptr<SerialPort> serialPort,
                             const std::function<void(MainWindow *, const std::string &)> &printRxResult,
                             const std::function<void(MainWindow *, const std::string &)> &printTxResult,
                             const std::function<void(MainWindow *, DelayType, int)> &printDelayResult)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if (!serialPort) {
        throw std::runtime_error(NULL_SERIAL_PORT_PASSED_TO_EXECUTE_STRING);
    }
    if (!mainWindow) {
        throw std::runtime_error(NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING);
    }
    if (!serialPort->isOpen()) {
        try {
            serialPort->openPort();
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }
    for (auto &it : (*this->m_scriptReader->commands())) {
        try {
            if (it.commandType() == SerialCommandType::WRITE) {
                serialPort->writeString(it.commandArgument());
                printTxResult(mainWindow, it.commandArgument());
            } else if (it.commandType() == SerialCommandType::READ) {
                printRxResult(mainWindow, serialPort->readString());
                /*
                std::string temp{stripNonAsciiCharacters(serialPort->readString())};
                if ((temp != "") && (!isWhitespace(temp))) {
                    printRxResult(stripLineEndings(temp));
                }*/
            } 
            else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                printDelayResult(mainWindow, DelayType::SECONDS, std::stoi(it.commandArgument()));
                delaySeconds(std::stoi(it.commandArgument()));
            } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                printDelayResult(mainWindow, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                delayMilliseconds(std::stoi(it.commandArgument()));
            } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                printDelayResult(mainWindow, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                delayMilliseconds(std::stoi(it.commandArgument()));
            } else {
                throw std::runtime_error(SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
            }
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }
}
