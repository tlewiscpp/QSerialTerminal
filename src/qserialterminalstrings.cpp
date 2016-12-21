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
    const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING{"Select a serial port and press connect"};
    const char *OPEN_SCRIPT_FILE_CAPTION{"Open Script File"};
#if (defined(_WIN32) || defined(__CYGWIN__))
    const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY{"C:/Cygwin64/opt/GitHub/QSerialTerminal/scripts/"};
#else
    const char *OPEN_SCRIPT_FILE_DEFAULT_DIRECTORY{"/opt/GitHub/QSerialTerminal/scripts/"};
#endif
    const char *EMPTY_SCRIPT_STRING{"The loaded script does not contain any valid commands: "};
    const char *EMPTY_SCRIPT_WINDOW_TITLE_STRING{"Loaded Script Empty"};
    const char *FILE_DOES_NOT_EXIST_STRING{"The selected script file does not exist: "};
    const char *FILE_DOES_NOT_EXIST_WINDOW_TITLE_STRING{"Script Does Not Exist"};
    const char *EXECUTING_SCRIPT_STRING{"Executing script file: "};
    const char *FINISHED_EXECUTING_SCRIPT_STRING{"Finished executing script file: "};

    const char *SECONDS_SUFFIX_STRING{"sec"};
    const char *MILLISECONDS_SUFFIX_STRING{"ms"};
    const char *MICROSECONDS_SUFFIX_STRING{"us"};
    const char *ACTION_LOAD_SCRIPT_ENABLED_TOOLTIP{""};
    const char *ACTION_LOAD_SCRIPT_DISABLED_TOOLTIP{"Connect to a serial port to load a script"};
    const char *SEND_BOX_ENABLED_TOOLTIP{""};
    const char *SEND_BOX_DISABLED_TOOLTIP{"Connect to a serial port to begin sending and receiving data"};

    const char *NULL_SERIAL_PORT_PASSED_TO_EXECUTE_STRING{"WARNING: Null SerialPort passed to ScriptExecutor::execute(), skipping script execution"};
    const char *NULL_MAIN_WINDOW_PASSED_TO_EXECUTE_STRING{"WARNING: Null MainWindow passed to ScriptExecutor::execute(), skipping script execution"};

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
    const char *NO_AVAILABLE_SERIAL_PORTS_STRING{"No serial ports are connected to device, please connect a device"};
    const char *NO_AVAILABLE_SERIAL_PORTS_WINDOW_TITLE_STRING{"No Serial Ports Available"};
    const char *SERIAL_PORT_DISCONNECTED_STRING{"Serial port disconnected: "};
}
