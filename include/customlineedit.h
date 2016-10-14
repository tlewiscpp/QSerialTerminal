#ifndef QSERIALTERMINAL_CUSTOMLINEEDIT_H
#define QSERIALTERMINAL_CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    CustomLineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *qke);

signals:
    void enterPressed();
};

#endif //QSERIALTERMINAL_CUSTOMLINEEDIT_H
