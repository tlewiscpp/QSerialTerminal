#ifndef QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H
#define QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H

#include <string>
#include <QString>
#include <QDir>
#include <QFile>

namespace ApplicationStrings
{
const char * const QUIT_PROMPT_STRING{"Are you sure you want to quit?"};
const char * const QUIT_PROMPT_WINDOW_TITLE_STRING{"Quit QSerialTerminal?"};
const char * const INVALID_SETTINGS_DETECTED_STRING{"Invalid settings detected, please reselect serial port settings: "};
const char * const INVALID_SETTINGS_DETECTED_WINDOW_TITLE_STRING{"Invalid Serial Port Settings"};
const char * const MAIN_WINDOW_TITLE{"QSerialTerminal"};
const char * const SUCCESSFULLY_OPENED_SERIAL_PORT_STRING{"Successfully opened serial port "};
const char * const SUCCESSFULLY_CLOSED_SERIAL_PORT_STRING{"Successfully closed serial port "};
const char * const MAIN_WINDOW_STYLESHEET{ "" };
const char * const BLUE_COLOR_STRING{"blue"};
const char * const RED_COLOR_STRING{"red"};
const char * const LICENSE_PATH{":/licenses/licenses/LICENSE"};
const char * const ACTION_INDEX_PROPERTY_TAG{"Index"};

const char * const TERMINAL_RECEIVE_BASE_STRING{"Rx << "};
const char * const TERMINAL_TRANSMIT_BASE_STRING{"Tx >> "};
const char * const NO_SERIAL_PORTS_CONNECTED_STRING{"No serial ports connected"};
const char * const CONNECT_TO_SERIAL_PORT_TO_BEGIN_STRING{"Select a serial port and press connect"};

const char * const SEND_BOX_ENABLED_TOOLTIP{""};
const char * const SEND_BOX_DISABLED_TOOLTIP{"Connect to a serial port to begin sending and receiving data"};

const char * const COULD_NOT_OPEN_SERIAL_PORT_STRING{"Could not open serial port ("};
const char * const COULD_NOT_OPEN_SERIAL_PORT_WINDOW_TITLE_STRING{"Problem Opening Serial Port"};

const char * const COMMAND_HISTORY_CONTEXT_MENU_STRING{"Command History"};
const char * const NO_AVAILABLE_SERIAL_PORTS_STRING{"No serial ports are connected to device, please connect a device"};
const char * const NO_AVAILABLE_SERIAL_PORTS_WINDOW_TITLE_STRING{"No Serial Ports Available"};
const char * const SERIAL_PORT_DISCONNECTED_STRING{"Serial port disconnected: "};
}


#endif //QSERIALTERMINAL_QSERIALTERMINALSTRINGS_H
