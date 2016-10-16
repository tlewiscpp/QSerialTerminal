#include "scriptreader.h"

ScriptReader::ScriptReader(const std::string &scriptFilePath) :
    m_scriptFilePath{scriptFilePath},
    m_commands{std::make_shared<std::vector<SerialCommand>>()}
{
    using namespace FileUtilities;
    using namespace GeneralUtilities;
    using namespace QSerialTerminalStrings;
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
    for (std::vector<std::string>::const_iterator iter = buffer.begin(); iter != buffer.end(); iter++) {
        try {
            std::string copyString{*iter};
            std::transform(copyString.begin(), copyString.end(), copyString.begin(), ::tolower);
            //TODO: Replace with regex for searching
            size_t foundDelayPosition{copyString.find(DELAY_IDENTIFIER)};
            size_t foundWritePosition{copyString.find(WRITE_IDENTIFIER)};
            size_t foundReadPosition{copyString.find(READ_IDENTIFIER)};
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
            if (foundDelayPosition != std::string::npos) {
                std::string targetDelay{""};
                std::string identifier{""};
                size_t foundDelaySecondsPosition{copyString.find(DELAY_SECONDS_IDENTIFIER)};
                size_t foundDelayMillisecondsPosition{copyString.find(DELAY_MILLISECONDS_IDENTIFIER)};
                size_t foundDelayMicrosecondsPosition{copyString.find(DELAY_MICROSECONDS_IDENTIFIER)};
                SerialCommandType commandType{SerialCommandType::COMMAND_UNSPECIFIED};
                if (foundDelaySecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_SECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_SECONDS;
                } else if (foundDelayMillisecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MILLISECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_MILLISECONDS;
                } else if (foundDelayMicrosecondsPosition != std::string::npos) {
                    identifier = static_cast<std::string>(DELAY_MICROSECONDS_IDENTIFIER);
                    commandType = SerialCommandType::DELAY_MICROSECONDS;
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
                targetDelay = copyString.substr(identifier.length());
                targetDelay = targetDelay.substr(0, targetDelay.find(")"));

                try {
                    int delay{std::stoi(targetDelay)};
                    (void)delay;
                    this->m_commands->emplace_back(commandType, targetDelay);
                } catch (std::exception &e) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    if (commandType == SerialCommandType::DELAY_SECONDS) {
                        std::cout << DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == SerialCommandType::DELAY_MILLISECONDS) {
                        std::cout << DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING << std::endl;
                    } else if (commandType == SerialCommandType::DELAY_MICROSECONDS) {
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
                targetString = iter->substr(static_cast<std::string>(WRITE_IDENTIFIER).length(), iter->find(")"));
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
                targetString = targetString.substr(targetString.find("\"")+1);
                targetString = targetString.substr(0, targetString.find("\""));
                this->m_commands->emplace_back(SerialCommandType::WRITE, targetString);
            } else if (foundReadPosition != std::string::npos) {
                if (copyString.find(")") == std::string::npos) {
                    std::cout << GENERIC_CONFIG_WARNING_BASE_STRING << currentLine << GENERIC_CONFIG_WARNING_TAIL_STRING << std::endl;
                    std::cout << NO_CLOSING_PARENTHESIS_FOUND_STRING << std::endl;
                    std::cout << *iter << std::endl;
                    std::cout << tWhitespace(iter->length()-1) << HERE_STRING << std::endl;
                    continue;
                }
                this->m_commands->emplace_back(SerialCommandType::READ, "");
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
}

std::shared_ptr<std::vector<SerialCommand>> ScriptReader::commands() const
{
    return this->m_commands;
}
