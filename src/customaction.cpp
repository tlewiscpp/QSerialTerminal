#include "customaction.h"

CustomAction::CustomAction(const QString &text, int index, QWidget *parent) :
    QAction{text, parent},
    m_index{index}
{
    connect(this, SIGNAL(triggered(bool)), this, SLOT(emitCustomSignal(bool)));
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
    emit (triggered(this, checked));
}
