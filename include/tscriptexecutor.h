/***********************************************************************
*    tscriptexecutor.h:                                                *
*    TScriptReader, script executor for TStream objects                *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/Pinguinsan/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the declarations of a TScripExecutor class that   *
*    executes TStreamCommand objects directly on a TStream object      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#ifndef TJLUTILS_SCRIPTEXECUTOR_H
#define TJLUTILS_SCRIPTEXECUTOR_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <functional>
#include <cstdlib>
#include <utility>

#include <generalutilities.h>
#include <tstream.h>
#include <tscriptreader.h>

class TScriptExecutor
{
public:
    TScriptExecutor(const std::string &scriptFilePath);
    void setScriptFilePath(const std::string &scriptFilePath);
    std::string scriptFilePath() const;
    bool hasCommands() const;
    size_t numberOfCommands() const;
    
    template <typename ... RxArgs, typename ... TxArgs, typename ... DelayArgs, typename ... FlushArgs, typename ... LoopArgs>
    void execute(std::shared_ptr<TStream> ioStream, 
                 const std::function<void(RxArgs...)> &printRxResult, 
                 const std::function<void(TxArgs...)> &printTxResult,
                 const std::function<void(DelayArgs...)> &printDelayResult,
                 const std::function<void(FlushArgs...)> &printFlushResult,
                 const std::function<void(LoopArgs...)> &printLoopResult)
    {
        using namespace GeneralUtilities;
        if (!ioStream) {
            throw std::runtime_error(NULL_IO_STREAM_PASSED_TO_EXECUTE_STRING);
        }
        if (!ioStream->isOpen()) {
            try {
                ioStream->openPort();
            } catch (std::exception &e) {
                throw std::runtime_error(e.what());
            }
        }
        int loop {false};
        int loopCount{0};
        this->m_tScriptCommands = doUnrollLoopCommands(*this->m_tScriptReader->commands());
        for (auto &it : this->m_tScriptCommands) {
            try {
                if (it.commandType() == TStreamCommandType::WRITE) {
                    ioStream->writeLine(it.commandArgument());
                    printTxResult(it.commandArgument());
                } else if (it.commandType() == TStreamCommandType::READ) {
                    printRxResult(ioStream->readLine());
                } else if (it.commandType() == TStreamCommandType::DELAY_SECONDS) {
                    printDelayResult(DelayType::SECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delaySeconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::DELAY_MILLISECONDS) {
                    printDelayResult(DelayType::MILLISECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delayMilliseconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::DELAY_MICROSECONDS) {
                    printDelayResult(DelayType::MICROSECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delayMilliseconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::FLUSH_RX) {
                    printFlushResult(FlushType::RX);
                    ioStream->flushRX();
                } else if (it.commandType() == TStreamCommandType::FLUSH_TX) {
                    printFlushResult(FlushType::TX);
                    ioStream->flushTX();
                } else if (it.commandType() == TStreamCommandType::FLUSH_RX_TX) {
                    printFlushResult(FlushType::RX_TX);
                    ioStream->flushRXTX();
                } else {
                    throw std::runtime_error(COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
                }
            } catch (std::exception &e) {
                throw std::runtime_error(e.what());
            }
        }
    }


    template <typename InstanceArg, typename ... RxArgs, typename ... TxArgs, typename ... DelayArgs, typename ... FlushArgs, typename ... LoopArgs>
    void execute(InstanceArg *instanceArg,
                 std::shared_ptr<TStream> ioStream, 
                 const std::function<void(InstanceArg *, RxArgs...)> &printRxResult, 
                 const std::function<void(InstanceArg *, TxArgs...)> &printTxResult,
                 const std::function<void(InstanceArg *, DelayArgs...)> &printDelayResult,
                 const std::function<void(InstanceArg *, FlushArgs...)> &printFlushResult,
                 const std::function<void(InstanceArg *, LoopArgs...)> &printLoopResult)
    {
        using namespace GeneralUtilities;
        (void)printLoopResult;
        if (!ioStream) {
            throw std::runtime_error(NULL_IO_STREAM_PASSED_TO_EXECUTE_STRING);
        }
        if (!ioStream->isOpen()) {
            try {
                ioStream->openPort();
            } catch (std::exception &e) {
                throw std::runtime_error(e.what());
            }
        }
        int loop {false};
        int loopCount{0};
        this->m_tScriptCommands = doUnrollLoopCommands(*this->m_tScriptReader->commands());
        (void)loop;
        (void)loopCount;
        for (auto &it : this->m_tScriptCommands) {
            try {
                if (it.commandType() == TStreamCommandType::WRITE) {
                    ioStream->writeLine(it.commandArgument());
                    printTxResult(instanceArg, it.commandArgument());
                } else if (it.commandType() == TStreamCommandType::READ) {
                    printRxResult(instanceArg, ioStream->readLine());
                } else if (it.commandType() == TStreamCommandType::DELAY_SECONDS) {
                    printDelayResult(instanceArg, DelayType::SECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delaySeconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::DELAY_MILLISECONDS) {
                    printDelayResult(instanceArg, DelayType::MILLISECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delayMilliseconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::DELAY_MICROSECONDS) {
                    printDelayResult(instanceArg, DelayType::MICROSECONDS, GeneralUtilities::decStringToInt(it.commandArgument()));
                    delayMilliseconds(GeneralUtilities::decStringToInt(it.commandArgument()));
                } else if (it.commandType() == TStreamCommandType::FLUSH_RX) {
                    printFlushResult(instanceArg, FlushType::RX);
                    ioStream->flushRX();
                } else if (it.commandType() == TStreamCommandType::FLUSH_TX) {
                    printFlushResult(instanceArg, FlushType::TX);
                    ioStream->flushTX();
                } else if (it.commandType() == TStreamCommandType::FLUSH_RX_TX) {
                    printFlushResult(instanceArg, FlushType::RX_TX);
                    ioStream->flushRXTX();
                } else {
                    throw std::runtime_error(COMMAND_TYPE_NOT_IMPLEMENTED_STRING + it.commandArgument());
                }
            } catch (std::exception &e) {
                throw std::runtime_error(e.what());
            }
        }
    }
private:
    std::shared_ptr<TScriptReader> m_tScriptReader;
    std::vector<TStreamCommand> m_tScriptCommands;

    std::vector<TStreamCommand> doUnrollLoopCommands(const std::vector<TStreamCommand> &tStreamCommands);
    bool containsLoopStart(const std::vector<TStreamCommand> &commands);
    std::pair<int, int> findInnerLoopIndexes(const std::vector<TStreamCommand> &tStreamCommands);


};


#endif //TJLUTILS_SCRIPTEXECUTOR_H
