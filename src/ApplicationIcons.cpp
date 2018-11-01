#include "ApplicationIcons.hpp"

ApplicationIcons *applicationIcons{nullptr};

ApplicationIcons::ApplicationIcons() :
    MAIN_WINDOW_ICON{QIcon{":/icons/icons/QSerialTerminal.png"}}
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
