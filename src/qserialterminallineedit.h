#ifndef QSERIALTERMINAL_QSERIALTERMINALLINEEDIT_H
#define QSERIALTERMINAL_QSERIALTERMINALLINEEDIT_H

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>
#include <QPoint>

class QSerialTerminalLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QSerialTerminalLineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *qke);

public slots:
    void showCommandHistoryContextMenu(const QPoint &point);

signals:
    void returnPressed(QSerialTerminalLineEdit *aoeb);
    void upArrowPressed(QSerialTerminalLineEdit *aoeb);
    void downArrowPressed(QSerialTerminalLineEdit *aoeb);
    void escapePressed(QSerialTerminalLineEdit *aoeb);
    void altPressed(QSerialTerminalLineEdit *aoeb);
    void ctrlAPressed(QSerialTerminalLineEdit *aoeb);
    void ctrlEPressed(QSerialTerminalLineEdit *aoeb);
    void ctrlUPressed(QSerialTerminalLineEdit *aoeb);
    void ctrlGPressed(QSerialTerminalLineEdit *aoeb);
    void ctrlCPressed(QSerialTerminalLineEdit *aoeb);
    void commandHistoryContextMenuRequested(const QPoint &point);
};

#endif //QSERIALTERMINAL_QSERIALTERMINALLINEEDIT_H
