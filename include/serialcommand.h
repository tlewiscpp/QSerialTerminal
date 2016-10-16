#ifndef SERIALCOMMUNICATION_SERIALCOMMAND_H
#define SERIALCOMMUNICATION_SERIALCOMMAND_H

#include <string>

enum class SerialCommandType { DELAY_SECONDS, DELAY_MILLISECONDS, DELAY_MICROSECONDS, WRITE, READ, COMMAND_UNSPECIFIED };
enum class DelayType { SECONDS, MILLISECONDS, MICROSECONDS };

class SerialCommand
{
public:
    SerialCommand(SerialCommandType commandType, const std::string &commandArgument);
    SerialCommandType commandType() const;
    std::string commandArgument() const;
    void setCommandType(SerialCommandType commandType);
    void setCommandArgument(const std::string &commandArgument);

private:
    SerialCommandType m_commandType;
    std::string m_commandArgument;
};

#endif //SERIALCOMMUNICATION_SERIALCOMMAND_H