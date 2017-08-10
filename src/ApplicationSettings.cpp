#include "ApplicationSettings.h"
#include <string>

namespace GlobalSettings
{
    const char *PROGRAM_NAME{"QSerialTerminal"};
    const char *PROGRAM_LONG_NAME{"QSerialTerminal"};
    const char *REMOTE_URL{"https://github.com/tlewiscpp/QSerialTerminal"};
    const char *AUTHOR_NAME{"Tyler Lewis"};
    const int SOFTWARE_MAJOR_VERSION{1};
    const int SOFTWARE_MINOR_VERSION{1};
    const int SOFTWARE_PATCH_VERSION{0};

    #if defined(__GNUC__)
        const char *COMPILER_NAME{"g++"};
        const int COMPILER_MAJOR_VERSION{__GNUC__};
        const int COMPILER_MINOR_VERSION{__GNUC_MINOR__};
        const int COMPILER_PATCH_VERSION{__GNUC_PATCHLEVEL__};
    #elif defined(_MSC_VER)
        const char *COMPILER_NAME{"msvc"};
        const int COMPILER_MAJOR_VERSION{_MSC_VER};
        const int COMPILER_MINOR_VERSION{0};
        const int COMPILER_PATCH_VERSION{0};
    #else
        const char *COMPILER_NAME{"unknown"};
        const int COMPILER_MAJOR_VERSION{0};
        const int COMPILER_MINOR_VERSION{0};
        const int COMPILER_PATCH_VERSION{0};
    #endif

}

ApplicationSettings::ApplicationSettings()
{

}
