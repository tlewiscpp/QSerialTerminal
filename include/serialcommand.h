#ifndef SERIALCOMMUNICATION_SERIALCOMMAND_H
#define SERIALCOMMUNICATION_SERIALCOMMAND_H

#include <string>

enum class SerialCommandType { DELAY_SECONDS, DELAY_MILLISECONDS, DELAY_MICROSECONDS, WRITE, READ, FLUSH_RX, FLUSH_TX, FLUSH_RX_TX, LOOP_START, LOOP_END, COMMAND_UNSPECIFIED };
enum class DelayType { SECONDS, MILLISECONDS, MICROSECONDS };
enum class FlushType { RX, TX, RX_TX };
enum class LoopType { START, END };

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
