/***********************************************************************
*    tscriptexecutor.cpp:                                              *
*    TScriptReader, script executor for TStream objects                *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a source file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a TScriptExecutor class that*
*    executes TStreamCommand objects directly on a TStream object      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "tscriptexecutor.h"

TScriptExecutor::TScriptExecutor(const std::string &tScriptFilePath) :
    m_tScriptReader{std::make_shared<TScriptReader>(tScriptFilePath)}
{

}

std::string TScriptExecutor::scriptFilePath() const
{
    return this->m_tScriptReader->scriptFilePath();
}

bool TScriptExecutor::hasCommands() const
{
    return (this->m_tScriptReader->commands()->size() > 0);
}

size_t TScriptExecutor::numberOfCommands() const
{
    return this->m_tScriptReader->commands()->size();
}

void TScriptExecutor::setScriptFilePath(const std::string &tScriptFilePath)
{
    this->m_tScriptReader.reset();
    this->m_tScriptReader = std::make_shared<TScriptReader>(tScriptFilePath);
}

std::vector<TStreamCommand> TScriptExecutor::doUnrollLoopCommands(const std::vector<TStreamCommand> &tStreamCommands)
{
    std::vector<TStreamCommand> copyCommands{tStreamCommands};
    if (!TScriptExecutor::containsLoopStart(copyCommands)) {
        return copyCommands;
    }
    while (TScriptExecutor::containsLoopStart(copyCommands)) {
        std::pair<int, int> innerLoopPositions = findInnerLoopIndexes(copyCommands);
        std::vector<TStreamCommand> toUnroll{};
        std::vector<TStreamCommand> temp{};
        int numberOfLoops{GeneralUtilities::decStringToInt(copyCommands.at(innerLoopPositions.first).commandArgument())};
        for (int loops = 0; loops < numberOfLoops; loops++) {
            for (int i = innerLoopPositions.first+1; i < innerLoopPositions.second; i++) {
                toUnroll.emplace_back(copyCommands.at(i));
            }
        }
        for (int i = 0; i < innerLoopPositions.first; i++) {
            temp.emplace_back(copyCommands.at(i));
        }
        for (auto &it : toUnroll) {
            temp.emplace_back(it);
        }
        for (unsigned int i = innerLoopPositions.second+1; i < copyCommands.size(); i++) {
            temp.emplace_back(copyCommands.at(i));
        }
        copyCommands = temp; 
    }
    return copyCommands;
}
std::pair<int, int> TScriptExecutor::findInnerLoopIndexes(const std::vector<TStreamCommand> &tStreamCommands)
{
    int lastLoopStartPosition{0};
    int lastLoopEndPosition{0};
    for (unsigned int i = 0; i < tStreamCommands.size(); i++) {
        if (tStreamCommands.at(i).commandType() == TStreamCommandType::LOOP_START) {
            lastLoopStartPosition = i;
        }
    }

    for (unsigned int i = lastLoopStartPosition; i < tStreamCommands.size(); i++) {
        if (tStreamCommands.at(i).commandType() == TStreamCommandType::LOOP_END) {
            if (lastLoopStartPosition == 0) {
                lastLoopEndPosition = lastLoopStartPosition + i;
            } else {
                lastLoopEndPosition = lastLoopStartPosition + i - 1;
            }
            break;
        }
    }
    return std::make_pair(lastLoopStartPosition, lastLoopEndPosition);
}

bool TScriptExecutor::containsLoopStart(const std::vector<TStreamCommand> &commands) 
{
    for (auto &it : commands) {
        if (it.commandType() == TStreamCommandType::LOOP_START) {
            return true;
        }
    }
    return false;
}
