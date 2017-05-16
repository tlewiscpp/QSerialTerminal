/***********************************************************************
*    tscriptreader.cpp:                                                *
*    TScriptReader, script reader for TStream objects                  *
*    Copyright (c) 2016 Tyler Lewis                                    *
************************************************************************
*    This is a header file for tjlutils:                               *
*    https://github.serial/tlewiscpp/tjlutils                         *
*    This file may be distributed with the entire tjlutils library,    *
*    but may also be distributed as a standalone file                  *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of a TScriptReader class that  *
*    reads and parses script files intended for use alongside the      *
*    TScriptExecutor to script commands for TStream objects            *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with tjlutils                                *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "tscriptreader.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

#include "fileutilities.h"
#include "generalutilities.h"

TScriptReader::TScriptReader(const std::string &scriptFilePath) :
    m_scriptFilePath{scriptFilePath},
    m_commands{std::make_shared<std::vector<TStreamCommand>>()}
{
    using namespace FileUtilities;
    using namespace GeneralUtilities;
    if (!fileExists(this->m_scriptFilePath)) {
        throw std::runtime_error(SCRIPT_FILE_DOES_NOT_EXISTS_STRING + tQuoted(this->m_scriptFilePath));
    }
    std::vector<std::string> buffer;
    std::ifstream readFromFile;
    readFromFile.open(this->m_scriptFilePath);
    if (readFromFile.is_open()) {
        std::string tempString{""};
        while (std::getline(readFromFile, tempString)) {
            buffer.emplace_back(tempString);
        }
        readFromFile.close();
    } else {
        throw std::runtime_error(UNABLE_TO_OPEN_SCRIPT_FILE_STRING + tQuoted(this->m_scriptFilePath));
    }
    buffer = trimWhitespace(buffer);
    int loops{0};
    int loopCount{0};
    for (std::vector<std::string>::const_iterator iter = buffer.begin(); iter != buffer.end(); iter++) {
        try {
            std::string copyString{*iter};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            //TODO: Replace with regex for searching
            size_t foundDelayPosition{copyString.find(DELAY_IDENTIFIER)};
            size_t foundWritePosition{copyString.find(WRITE_IDENTIFIER)};
            size_t foundReadPosition{copyString.find(READ_IDENTIFIER)};
            size_t foundFlushPosition{copyString.find(FLUSH_IDENTIFIER)};
            size_t foundLoopPosition{copyString.find(LOOP_IDENTIFIER)};
            size_t foundClosingLoopPosition{copyString.find(CLOSING_LOOP_IDENTIFIER)};
            if (copyString.length() != 0) {
                std::string otherCopy{copyString};
                unsigned int numberOfWhitespace{0};
                while (otherCopy.length() > 1 && isWhitespace(otherCopy[0])) {
                    stripFromString(otherCopy, ' ');
                    numberOfWhitespace++;
                }
                if (copyString.length() > numberOfWhitespace) {
                    if (copyString[numberOfWhitespace] == '#') {
                        continue;
                    }
                }
            } else {
                continue;
            }
            if (isWhitespace(copyString)) {
                continue;
            }

            long int currentLine{std::distance<std::vector<std::string>::const_iterator>(buffer.begin(), iter)+1};
            if (foundLoopPosition != std::string::npos) {
                std::string targetLoopCount{0};
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetLoopCount = getBetween("(", ")", copyString);
                if (trimWhitespace(targetLoopCount) == "") {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_LOOP_COUNT_SPECIFIED_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                } else {
                    try {
                        int temp{GeneralUtilities::decStringToInt(targetLoopCount)};
                        loopCount = temp > 0 ? temp : temp*-1;
                        loops++;
                        this->m_commands->emplace_back(TStreamCommandType::LOOP_START, GeneralUtilities::toString(loopCount));
                    } catch (std::exception &e) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->find(")")-1) << EXPECTED_HERE_STRING << std::endl;
                    }
                }
            } else if ((foundClosingLoopPosition != std::string::npos) && (trimWhitespace(copyString).length() == 1)) {
                if (loops == 0) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << UNEXPECTED_LOOP_CLOSING_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                } else {
                    loops--;
                    this->m_commands->emplace_back(TStreamCommandType::LOOP_END, "");
                } 
            } else if (foundDelayPosition != std::string::npos) {
                std::string targetDelay{""};
                std::string identifier{""};
                size_t foundDelaySecondsPosition{copyString.find(DELAY_SECONDS_IDENTIFIER)};
                size_t foundDelayMillisecondsPosition{copyString.find(DELAY_MILLISECONDS_IDENTIFIER)};
                size_t foundDelayMicrosecondsPosition{copyString.find(DELAY_MICROSECONDS_IDENTIFIER)};
                TStreamCommandType commandType{TStreamCommandType::COMMAND_UNSPECIFIED};
                if (foundDelaySecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_SECONDS_IDENTIFIER);
                    commandType = TStreamCommandType::DELAY_SECONDS;
                } else if (foundDelayMillisecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MILLISECONDS_IDENTIFIER);
                    commandType = TStreamCommandType::DELAY_MILLISECONDS;
                } else if (foundDelayMicrosecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MICROSECONDS_IDENTIFIER);
                    commandType = TStreamCommandType::DELAY_MICROSECONDS;
                } else {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                }
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetDelay = getBetween("(", ")", copyString);
                try {
                    long long int delay{GeneralUtilities::decStringToLongLong(targetDelay)};
                    this->m_commands->emplace_back(commandType, GeneralUtilities::toString(delay));
                } catch (std::exception &e) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    if (commandType == TStreamCommandType::DELAY_SECONDS) {
                        std::cout << DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == TStreamCommandType::DELAY_MILLISECONDS) {
                        std::cout << DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == TStreamCommandType::DELAY_MICROSECONDS) {
                        std::cout << DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    }
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->find(")")-1) << EXPECTED_HERE_STRING << std::endl;
                }
            } else if (foundWritePosition != std::string::npos) {
                std::string targetString{""};
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                targetString = getBetween("(", ")", *iter);
                if (targetString.find_last_of("\"") == targetString.find("\"")) {
                    if (targetString.find(targetString.find("\"")) != std::string::npos) {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << NO_CLOSING_QUOTATION_MARKS_FOUND_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->find("\"")) << EXPECTED_HERE_STRING << std::endl;
                        continue;
                    } else {
                        std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                        std::cout << WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING << std::endl;
                        std::cout << *iter << std::endl;
                        std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                        continue;
                    }
                }
                targetString = getBetween("\"", "\"", targetString);
                this->m_commands->emplace_back(TStreamCommandType::WRITE, targetString);
            } else if (foundReadPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                this->m_commands->emplace_back(TStreamCommandType::READ, "");
            } else if (foundFlushPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                this->m_commands->emplace_back(TStreamCommandType::FLUSH_RX, "");
            } else {
                std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                std::cout << CONFIG_EXPRESSION_MALFORMED_STRING << std::endl;
                std::cout << *iter << std::endl;
                std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
            }
        } catch (std::exception &e) {
            std::cout << EXCEPTION_IN_CONSTRUCTOR_STRING << e.what() << std::endl;
            this->m_commands->clear();
            return;
        }
    }
    if (loops != 0) {
        std::cout << UNTERMINATED_LOOP_STRING << std::endl;
        this->m_commands->clear();
        return; 
    }
}

std::string TScriptReader::scriptFilePath() const
{
    return this->m_scriptFilePath;
}

std::shared_ptr<std::vector<TStreamCommand>> TScriptReader::commands() const
{
    return this->m_commands;
}
