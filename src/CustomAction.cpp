#include "CustomAction.h"

CustomAction::CustomAction(const QString &text, int index, QWidget *parent) :
    QAction{text, parent},
    m_index{index}
{
    connect(this, &CustomAction::triggered, this, &CustomAction::emitCustomSignal);
}

int CustomAction::index() const
{
    return this->m_index;
}

void CustomAction::setIndex(int index)
{
    this->m_index = index;
}

void CustomAction::emitCustomSignal(bool checked)
{
    emit (this->triggered(this, checked));
}
