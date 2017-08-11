#include "ApplicationStrings.h"

namespace ApplicationStrings
{
    const char *QUIT_PROMPT_STRING{"Are you sure you want to quit?"};
    const char *QUIT_PROMPT_WINDOW_TITLE_STRING{"Quit QSerialTerminal?"};
    const char *INVALID_SETTINGS_DETECTED_STRING{"Invalid settings detected, please reselect serial port settings: "};
    const char *INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING{"Invalid Serial Port Settings"};
    const char *MAIN_WINDOW_TITLE{"QSerialTerminal"};
    const char *SUCCESSFULLY_OPENED_SERIAL_PORT_STRING{"Successfully opened serial port "};
    const char *SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING{"Successfully closed serial port "};
    const char *MAIN_WINDOW_STYLESHEET{ "" };
    const char *BLUE_COLOR_STRING{"blue"};
    const char *RED_COLOR_STRING{"red"};
    const char *LICENSE_PATH{":/licenses/licenses/LICENSE"};

    const char *TERMINAL_RECEIVE_BASE_STRING{"Rx << "};
    const char *TERMINAL_TRANSMIT_BASE_STRING{"Tx >> "};
    const char *NO_SERIAL_PORTS_CONNECTED_STRING{"No serial ports connected"};
    const char *CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING{"Select a serial port and press connect"};

    const char *SEND_BOX_ENABLED_TOOLTIP{""};
    const char *SEND_BOX_DISABLED_TOOLTIP{"Connect to a serial port to begin sending and receiving data"};

    const char *COULD_NOT_OPEN_SERIAL_PORT_STRING{"Could not open serial port ("};
    const char *COULD_NOT_OPEN_SERIAL_PORT_WINDOW_TITLE_STRING{"Problem Opening Serial Port"};

    const char *COMMAND_HISTORY_CONTEXT_MENU_STRING{"Command History"};
    const char *NO_AVAILABLE_SERIAL_PORTS_STRING{"No serial ports are connected to device, please connect a device"};
    const char *NO_AVAILABLE_SERIAL_PORTS_WINDOW_TITLE_STRING{"No Serial Ports Available"};
    const char *SERIAL_PORT_DISCONNECTED_STRING{"Serial port disconnected: "};
}
