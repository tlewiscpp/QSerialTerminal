#include "include/custommenu.h"

CustomMenu::CustomMenu(QWidget *parent) :
    QMenu{parent}
{
    connect(this, SIGNAL(aboutToShow()), this, SLOT(emitCustomShowSignal()));
    connect(this, SIGNAL(aboutToHide()), this, SLOT(emitCustomHideSignal()));
}

void CustomMenu::emitCustomShowSignal()
{
    emit (CustomMenu::aboutToShow(this));
}

void CustomMenu::emitCustomHideSignal()
{
    emit (CustomMenu::aboutToHide(this));
}
