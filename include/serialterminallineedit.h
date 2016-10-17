#ifndef QSERIALTERMINAL_SERIALTERMINALLINEEDIT_H
#define QSERIALTERMINAL_SERIALTERMINALLINEEDIT_H

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>

class SerialTerminalLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    SerialTerminalLineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *qke);

signals:
    void returnPressed(SerialTerminalLineEdit *aoeb);
    void upArrowPressed(SerialTerminalLineEdit *aoeb);
    void downArrowPressed(SerialTerminalLineEdit *aoeb);
    void escapePressed(SerialTerminalLineEdit *aoeb);
    void altPressed(SerialTerminalLineEdit *aoeb);
    void ctrlAPressed(SerialTerminalLineEdit *aoeb);
    void ctrlEPressed(SerialTerminalLineEdit *aoeb);
    void ctrlUPressed(SerialTerminalLineEdit *aoeb);
    void ctrlGPressed(SerialTerminalLineEdit *aoeb);
    void ctrlCPressed(SerialTerminalLineEdit *aoeb);
};

#endif //QSERIALTERMINAL_SERIALTERMINALLINEEDIT_H
