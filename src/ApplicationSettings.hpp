#ifndef TRMIDOUBLECHECKBOX_APPLICATIONSETTINGS_H
#define TRMIDOUBLECHECKBOX_APPLICATIONSETTINGS_H

#include <string>

namespace GlobalSettings
{
    extern const char *PROGRAM_NAME;
    extern const char *PROGRAM_LONG_NAME;
    extern const char *PROGRAM_DESCRIPTION;
    extern const char *REMOTE_URL;
    extern const char *AUTHOR_NAME;
    extern const int SOFTWARE_MAJOR_VERSION;
    extern const int SOFTWARE_MINOR_VERSION;
    extern const int SOFTWARE_PATCH_VERSION;

    #if defined(__GNUC__)
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #elif defined(_MSC_VER)
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #else
    extern const char *COMPILER_NAME;
    extern const int COMPILER_MAJOR_VERSION;
    extern const int COMPILER_MINOR_VERSION;
    extern const int COMPILER_PATCH_VERSION;
    #endif
}


class ApplicationSettings
{
public:
    ApplicationSettings();

private:

};

#endif //TRMIDOUBLECHECKBOX_APPLICATIONSETTINGS_H
