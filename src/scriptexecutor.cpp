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
                             const std::function<void(MainWindow *, DelayType, int)> &printDelayResult,
                             const std::function<void(MainWindow *, FlushType)> &printFlushResult,
                             const std::function<void(MainWindow *, LoopType, int, int)> &printLoopResult)
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
    int loop {false};
    int loopCount{0};
    std::vector<SerialCommand> loopSerialCommands;
    for (auto &it : (*this->m_scriptReader->commands())) {
        mainWindow->application()->processEvents();
        try {
            if (!loop) {
                if (it.commandType() == SerialCommandType::WRITE) {
                    serialPort->writeString(it.commandArgument());
                    printTxResult(mainWindow, it.commandArgument());
                } else if (it.commandType() == SerialCommandType::READ) {
                    printRxResult(mainWindow, serialPort->readString());
                } else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                    printDelayResult(mainWindow, DelayType::SECONDS, std::stoi(it.commandArgument()));
                    delaySecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                    printDelayResult(mainWindow, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                    delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                    printDelayResult(mainWindow, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                    delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                    printFlushResult(mainWindow, FlushType::RX);
                    serialPort->flushRX();
                } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                    printFlushResult(mainWindow, FlushType::TX);
                    serialPort->flushTX();
                } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                    printFlushResult(mainWindow, FlushType::RX_TX);
                    serialPort->flushRXTX();
                } else if (it.commandType() == SerialCommandType::LOOP_START) {
                    loop = true;
                    loopCount = std::stoi(it.commandArgument());
                    loopSerialCommands.clear();
                } else {
                    throw std::runtime_error(SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
                }
            } else {
                if (it.commandType() == SerialCommandType::LOOP_END) {
                    loop =  false;
                    doLoop(mainWindow, serialPort, printRxResult, printTxResult, printDelayResult, printFlushResult, printLoopResult, loopSerialCommands, loopCount);
                    loopSerialCommands.clear();
                } else {
                    loopSerialCommands.emplace_back(it);
                }
            }
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }
}

void ScriptExecutor::delaySecondsWithUpdate(int howLong, QApplication *update)
{
    int millisecondsToDelay{howLong * 1000};
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};

    do {
        update->processEvents();
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= millisecondsToDelay);
}


void ScriptExecutor::delayMillisecondsWithUpdate(int howLong, QApplication *update)
{
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};

    do {
        update->processEvents();
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}

void ScriptExecutor::delayMicrosecondsWithUpdate(int howLong, QApplication *update)
{
    auto startTime{std::chrono::high_resolution_clock::now()};
    auto endTime{std::chrono::high_resolution_clock::now()};
    long long int elapsedTime{0};

    do {
        update->processEvents();
        endTime = std::chrono::high_resolution_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    } while (elapsedTime <= howLong);
}



void ScriptExecutor::doLoop(MainWindow *mainWindow, std::shared_ptr<SerialPort> serialPort,
                const std::function<void(MainWindow *, const std::string &)> &printRxResult,
                const std::function<void(MainWindow *, const std::string &)> &printTxResult,
                const std::function<void(MainWindow *, DelayType, int)> &printDelayResult,
                const std::function<void(MainWindow *, FlushType)> &printFlushResult,
                const std::function<void(MainWindow *, LoopType, int, int)> &printLoopResult,
                const std::vector<SerialCommand> &commands,
                int loopCount)
{
    using namespace QSerialTerminalStrings;
    using namespace GeneralUtilities;
    if (loopCount == -1) {
        for (int i = 0; true; i++) {
            mainWindow->application()->processEvents();
            printLoopResult(mainWindow, LoopType::START, i, -1);
            for (auto &it : commands) {
                try {
                    if (it.commandType() == SerialCommandType::WRITE) {
                        serialPort->writeString(it.commandArgument());
                        printTxResult(mainWindow, it.commandArgument());
                    } else if (it.commandType() == SerialCommandType::READ) {
                        printRxResult(mainWindow, serialPort->readString());
                    } else if (it.commandType() == SerialCommandType::DELAY_SECONDS) {
                       printDelayResult(mainWindow, DelayType::SECONDS, std::stoi(it.commandArgument()));
                       delaySecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                   } else if (it.commandType() == SerialCommandType::DELAY_MILLISECONDS) {
                       printDelayResult(mainWindow, DelayType::MILLISECONDS, std::stoi(it.commandArgument()));
                       delayMillisecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                   } else if (it.commandType() == SerialCommandType::DELAY_MICROSECONDS) {
                       printDelayResult(mainWindow, DelayType::MICROSECONDS, std::stoi(it.commandArgument()));
                       delayMicrosecondsWithUpdate(std::stoi(it.commandArgument()), mainWindow->application());
                   } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                        printFlushResult(mainWindow, FlushType::RX);
                        serialPort->flushRX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                        printFlushResult(mainWindow, FlushType::TX);
                        serialPort->flushTX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                        printFlushResult(mainWindow, FlushType::RX_TX);
                        serialPort->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
        }
    } else {
        for (int i = 0 ; i < loopCount; i++) {
            mainWindow->application()->processEvents();
            printLoopResult(mainWindow, LoopType::START, i, loopCount);
            for (auto &it : commands) {
                try {
                    if (it.commandType() == SerialCommandType::WRITE) {
                        serialPort->writeString(it.commandArgument());
                        printTxResult(mainWindow, it.commandArgument());
                    } else if (it.commandType() == SerialCommandType::READ) {
                        printRxResult(mainWindow, serialPort->readString());
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
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX) {
                        printFlushResult(mainWindow, FlushType::RX);
                        serialPort->flushRX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_TX) {
                        printFlushResult(mainWindow, FlushType::TX);
                        serialPort->flushTX();
                    } else if (it.commandType() == SerialCommandType::FLUSH_RX_TX) {
                        printFlushResult(mainWindow, FlushType::RX_TX);
                        serialPort->flushRXTX();
                    }
                } catch (std::exception &e) {
                    throw std::runtime_error(e.what());
                }
            }
        }
    }
    printLoopResult(mainWindow, LoopType::END, loopCount-1, loopCount);
}
