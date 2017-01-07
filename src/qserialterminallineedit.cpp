#include "qserialterminallineedit.h"

QSerialTerminalLineEdit::QSerialTerminalLineEdit(QWidget *parent) :
    QLineEdit{parent}
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(showCommandHistoryContextMenu(const QPoint &)));
}

void QSerialTerminalLineEdit::keyPressEvent(QKeyEvent *qke)
{
    if (qke) {
        if ((qke->key() == Qt::Key_Enter) || (qke->key() == Qt::Key_Return)) {
            emit returnPressed(this);
        } else if (qke->key() == Qt::Key_Up) {
            emit (upArrowPressed(this));
        } else if (qke->key() == Qt::Key_Down) {
            emit (downArrowPressed(this));
        } else if (qke->key() == Qt::Key_Escape) {
            emit (escapePressed(this));
        } else if (qke->key() == Qt::Key_Alt) {
            emit (altPressed(this));
        } else if ((qke->key() == Qt::Key_A) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlAPressed(this));
        } else if ((qke->key() == Qt::Key_E) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlEPressed(this));
        } else if ((qke->key() == Qt::Key_U) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlUPressed(this));
        } else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlGPressed(this));
        }  else if ((qke->key() == Qt::Key_G) && (qke->modifiers().testFlag(Qt::ControlModifier))) {
            emit (ctrlCPressed(this));
        } else {
            return QLineEdit::keyPressEvent(qke);
        }
    }
}

void QSerialTerminalLineEdit::showCommandHistoryContextMenu(const QPoint &point)
{
    emit (commandHistoryContextMenuRequested(point));
}
