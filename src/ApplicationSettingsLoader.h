#ifndef TRMIDOUBLECHECKBOX_APPLICATONSETTINGSLOADER_H
#define TRMIDOUBLECHECKBOX_APPLICATONSETTINGSLOADER_H

#include <QObject>
#include <utility>

class ApplicationSettings;

class ApplicationSettingsLoader : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationSettingsLoader(QObject *parent = nullptr);

    static ApplicationSettings loadApplicationSettings();
    static void saveApplicationSettings(const ApplicationSettings &settings);
private:

};

#endif // TRMIDOUBLECHECKBOX_APPLICATONSETTINGSLOADER_H
