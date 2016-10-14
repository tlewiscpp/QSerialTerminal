#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent) :
    QLineEdit{parent}
{
    this->installEventFilter(this);
}

void CustomLineEdit::keyPressEvent(QKeyEvent *qke)
{
    if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
        emit(enterPressed());
    } else {
        return QLineEdit::keyPressEvent(qke);
    }
}
