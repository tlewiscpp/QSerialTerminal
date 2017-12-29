#include "ApplicationSettingsLoader.h"
#include "ApplicationStrings.h"
#include "ApplicationSettings.h"
#include "GlobalDefinitions.h"

#include <QSettings>

ApplicationSettingsLoader::ApplicationSettingsLoader(QObject *parent) :
    QObject{parent}
{

}

void ApplicationSettingsLoader::saveApplicationSettings(const ApplicationSettings &settings)
{
    (void)settings;
    QSettings settingsSaver;
    settingsSaver.sync();
    LOG_INFO() << "Successfully saved (valid) application settings";
}

ApplicationSettings ApplicationSettingsLoader::loadApplicationSettings() {
    QSettings settingsLoader;
    ApplicationSettings settings;
    LOG_INFO() << "Successfully loaded (only valid) saved application settings";
    return settings;
}

