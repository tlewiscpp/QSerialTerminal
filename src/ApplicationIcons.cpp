#include "ApplicationIcons.h"

ApplicationIcons *applicationIcons{nullptr};

ApplicationIcons::ApplicationIcons() :
    MAIN_WINDOW_ICON{QIcon{":/icons/icons/qserialterminal.png"}}
{

}

ApplicationIcons::~ApplicationIcons()
{
    //Destructor
}

void ApplicationIcons::initializeInstance() {
    if (applicationIcons == nullptr) {
        applicationIcons = new ApplicationIcons{};
    }
}
