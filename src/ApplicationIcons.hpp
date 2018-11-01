#ifndef QSERIALTERMINAL_APPLICATIONICONS_H
#define QSERIALTERMINAL_APPLICATIONICONS_H

#include <QIcon>
#include <QString>

class ApplicationIcons
{
public:
    static void initializeInstance();
    ~ApplicationIcons();
    const QIcon MAIN_WINDOW_ICON;
private:
    ApplicationIcons();
};

extern ApplicationIcons *applicationIcons;

#endif //QSERIALTERMINAL_APPLICATIONICONS_H
