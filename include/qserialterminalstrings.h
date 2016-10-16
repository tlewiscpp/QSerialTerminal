#ifndef QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H
#define QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H

#include <string>
#include <QString>
#include <QDir>
#include <QFile>

namespace QSerialTerminalStrings
{
    extern const char *QUIT_PROMPT_STRING;
    extern const char *QUIT_PROMPT_WINDOW_TITLE_STRING;
    extern const char *INVALID_SETTINGS_DETECTED_STRING;
    extern const char *INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING;
    extern const char *MAIN_WINDOW_TITLE;
    extern const char *SETTINGS_DIALOG_WINDOW_TITLE;
    extern const char *SUCCESSFULLY_OPENED_SERIAL_PORT_STRING;
    extern const char *SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING;
    extern const char *MAIN_WINDOW_STYLESHEET;
    extern const char *BLUE_COLOR_STRING;
    extern const char *RED_COLOR_STRING;
    extern const char *GREEN_COLOR_STRING;
    extern const char *TERMINAL_RECEIVE_BASE_STRING;
    extern const char *TERMINAL_TRANSMIT_BASE_STRING;
    extern const char *TERMINAL_DELAY_BASE_STRING;
    extern const char *NO_SERIAL_PORTS_CONNECTED_STRING;
    extern const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING;
    extern const char *EMPTY_SCRIPT_STRING;
    extern const char *FILE_DOES_NOT_EXIST_STRING;
    extern const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING;
    extern const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING;
    extern const char *EXECUTING_SCRIPT_STRING;
    extern const char *FINISHED_EXECUTING_SCRIPT_STRING;
    extern const char *OPEN_SCRIPT_FILE_CAPTION;
    extern const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY;

    extern const char *DELAY_IDENTIFIER;
    extern const char *DELAY_SECONDS_IDENTIFIER;
    extern const char *DELAY_MILLISECONDS_IDENTIFIER;
    extern const char *DELAY_MICROSECONDS_IDENTIFIER;
    extern const char *WRITE_IDENTIFIER;
    extern const char *READ_IDENTIFIER;
    extern const char *SECONDS_SUFFIX_STRING;
    extern const char *MILLISECONDS_SUFFIX_STRING;
    extern const char *MICROSECONDS_SUFFIX_STRING;
    extern const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP;
    extern const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP;
    extern const char *SEND_BOX_ENABLED_TOOLTIP;
    extern const char *SEND_BOX_DISABLED_TOOLTIP;

    extern const char *NO_CLOSING_PARENTHESIS_FOUND_STRING;
    extern const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING;
    extern const char *NO_PARAMETER_SEPARATING_COMMA_STRING;
    extern const char *EXPECTED_HERE_STRING;
    extern const char *HERE_STRING;
    extern const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING;
    extern const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING;
    extern const char *GENERIC_CONFIG_WARNING_BASE_STRING;
    extern const char *GENERIC_CONFIG_WARNING_TAIL_STRING;
    extern const char *CONFIG_EXPRESSION_MALFORMED_STRING;
    extern const char *EXCEPTION_IN_CONSTRUCTOR_STRING;
    extern const char *SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING;
    extern const char *NULL_SERIAL_PORT_PASSED_TO_EXECUTE_STRING;
    extern const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING;
    extern const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING;
    extern const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING;
}


#endif //QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H
