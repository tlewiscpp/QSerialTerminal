#include "qserialterminalstrings.h"

namespace QSerialTerminalStrings
{

    const char *QUIT_PROMPT_STRING{"Are you sure you want to quit?"};
    const char *QUIT_PROMPT_WINDOW_TITLE_STRING{"Quit QSerialTerminal?"};
    const char *INVALID_SETTINGS_DETECTED_STRING{"Invalid settings detected, please reselect serial port settings: "};
    const char *INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING{"Invalid Serial Port Settings"};
    const char *MAIN_WINDOW_TITLE{"QSerialTerminal"};
    const char *SETTINGS_DIALOG_WINDOW_TITLE{"Serial Port Settings"};
    const char *SUCCESSFULLY_OPENED_SERIAL_PORT_STRING{"Successfully opened serial port "};
    const char *SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING{"Successfully closed serial port "};
    const char *MAIN_WINDOW_STYLESHEET{ "" };
    const char *BLUE_COLOR_STRING{"blue"};
    const char *RED_COLOR_STRING{"red"};
    const char *GREEN_COLOR_STRING{"green"};
    const char *GRAY_COLOR_STRING{"gray"};
    const char *ORANGE_COLOR_STRING{"orange"};
    const char *TERMINAL_RECEIVE_BASE_STRING{"Rx << "};
    const char *TERMINAL_TRANSMIT_BASE_STRING{"Tx >> "};
    const char *TERMINAL_DELAY_BASE_STRING{"Delay <> "};
    const char *TERMINAL_FLUSH_RX_BASE_STRING{"Flush vv "};
    const char *TERMINAL_FLUSH_TX_BASE_STRING{"Flush ^^ "};
    const char *TERMINAL_FLUSH_RX_TX_BASE_STRING{"Flush ^v "};
    const char *NO_SERIAL_PORTS_CONNECTED_STRING{"No serial ports connected"};
    const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING{"Use the connect button to connect to a serial port and begin sending data"};
    const char *OPEN_SCRIPT_FILE_CAPTION{"Open Script File"};
    const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY{static_cast<std::string>(QDir::homePath().toStdString() + static_cast<std::string>("/QSerialTerminal/")).c_str()};
    const char *EMPTY_SCRIPT_STRING{"The loaded script does not contain any valid commands: "};
    const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING{"Loaded Script Empty"};
    const char *FILE_DOES_NOT_EXIST_STRING{"The selected script file does not exist: "};
    const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING{"Script Does Not Exist"};
    const char *EXECUTING_SCRIPT_STRING{"Executing script file: "};
    const char *FINISHED_EXECUTING_SCRIPT_STRING{"Finished executing script file: "};
    const char *DELAY_IDENTIFIER{"delay"};
    const char *DELAY_SECONDS_IDENTIFIER{"delayseconds("};
    const char *DELAY_MILLISECONDS_IDENTIFIER{"delaymilliseconds("};
    const char *DELAY_MICROSECONDS_IDENTIFIER{"delaymicroseconds("};
    const char *WRITE_IDENTIFIER{"write("};
    const char *READ_IDENTIFIER{"read("};
    const char *LOOP_IDENTIFIER{"loop("};
    const char *FLUSH_RX_IDENTIFIER{"flushrx"};
    const char *FLUSH_TX_IDENTIFIER{"flushtx"};
    const char *FLUSH_TX_RX_IDENTIFIER{"flushtxrx"};
    const char *FLUSH_RX_TX_IDENTIFIER{"flushrxtx"};
    const char *SECONDS_SUFFIX_STRING{"sec"};
    const char *MILLISECONDS_SUFFIX_STRING{"ms"};
    const char *MICROSECONDS_SUFFIX_STRING{"us"};
    const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP{""};
    const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP{"Connect to a serial port to load a script"};
    const char *SEND_BOX_ENABLED_TOOLTIP{""};
    const char *SEND_BOX_DISABLED_TOOLTIP{"Connect to a serial port to begin sending and receiving data"};
    const char *NO_CLOSING_PARENTHESIS_FOUND_STRING{"    No matching parenthesis found, ignoring option"};
    const char *NO_CLOSING_QUOTATION_MARKS_FOUND_STRING{"    No matching quotation marks found, ingoring option"};
    const char *NO_PARAMETER_SEPARATING_COMMA_STRING{"    No parameter separating comma found, ignoring option"};
    const char *EXPECTED_HERE_STRING{"^---expected here"};
    const char *HERE_STRING{"^---here"};
    const char *WRITE_PARAMETER_MUST_BE_IN_QUOTATIONS_STRING{"    Write() parameter must be enclosed in parentheses, ignoring option"};
    const char *DELAY_SECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelaySeconds() parameter is not an integer, ignoring option"};
    const char *DELAY_MILLISECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMilliseconds() parameter is not an integer, ignoring option"};
    const char *DELAY_MICROSECONDS_PARAMETER_NOT_AN_INTEGER_STRING{"    DelayMicroseconds() parameter is not an integer, ignoring option"};
    const char *GENERIC_CONFIG_WARNING_BASE_STRING{"WARNING: line "};
    const char *GENERIC_CONFIG_WARNING_TAIL_STRING{" of configuration file:"};
    const char *CONFIG_EXPRESSION_MALFORMED_STRING{"    expression is malformed/has invalid syntax, ignoring option"};
    const char *EXCEPTION_IN_CONSTRUCTOR_STRING{"WARNING: Standard exception caught in ScriptFileReader constructor: "};
    const char *SERIAL_COMMAND_TYPE_NOT_IMPLEMENTED_STRING{"WARNING: SerialCommandType not implemented, skipping command: "};
    const char *NULL_SERIAL_PORT_PASSED_TO_EXECUTE_STRING{"WARNING: Null SerialPort passed to ScriptExecutor::execute(), skipping script execution"};
    const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING{"WARNING: Null MainWindow passed to ScriptExecutor::execute(), skipping script execution"};
    const char *SCRIPT_FILE_DOES_NOT_EXISTS_STRING{"WARNING: Script file does not exist, skipping script: "};
    const char *UNABLE_TO_OPEN_SCRIPT_FILE_STRING{"WARNING: Unable to open script file, skipping script: "};
    const char *LOOP_COUNT_PARAMETER_NOT_AN_INTEGER_STRING{"LoopCount() parameter is not an integer, ignoring option"};
    const char *UNTERMINATED_LOOP_STRING{"WARNING: The script contains an unterminated loop,  skipping script execution"};
    const char *UNEXPECTED_LOOP_CLOSING_STRING{"WARNING: A loop closure was found, but no loop was currently being populated, ignoring option"};
    const char *CLOSING_LOOP_IDENTIFIER{"}"};
    const char *BEGINNING_INFINITE_LOOP_STRING{"***Beginning infinite loop***"};
    const char *INFINITE_LOOP_COUNT_TAIL_STRING{"/infinite)"};
    const char *LOOPS_TAIL_STRING{" loops***"};
    const char *BEGINNING_LOOPS_BASE_STRING{"***Starting "};
    const char *ENDING_LOOPS_BASE_STRING{"***Finished "};
    const char *BEGIN_LOOP_BASE_STRING{"Begin loop ("};
    const char *END_LOOP_BASE_STRING{"End loop ("};
    const char *CANCEL_SCRIPT_STRING{"Cancel Script"};
    const char *SEND_STRING{"Send"};
    const char *CANCELED_EXECUTING_SCRIPT_STRING{"Canceled execution of script file: "};
    const char *COMMAND_HISTORY_CONTEXT_MENU_STRING{"Command History"};


}
