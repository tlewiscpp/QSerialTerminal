#include "serialcommand.h"

SerialCommand::SerialCommand(SerialCommandType commandType, const std::string &commandArgument) :
    m_commandType{commandType},
    m_commandArgument{commandArgument}
{

}

SerialCommandType SerialCommand::commandType() const
{
    return this->m_commandType;
}

std::string SerialCommand::commandArgument() const
{
    return this->m_commandArgument;
}

void SerialCommand::setCommandType(SerialCommandType commandType)
{
    this->m_commandType = commandType;
}

void SerialCommand::setCommandArgument(const std::string &commandArgument)
{
    this->m_commandArgument = commandArgument;
}