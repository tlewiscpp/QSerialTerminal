#ifndef QSERIALTERMINAL_CUSTOMMENU_H
#define QSERIALTERMINAL_CUSTOMMENU_H

#include <QAction>
#include <QMenu>

class CustomMenu : public QMenu
{

    Q_OBJECT
public:
    CustomMenu(QWidget *parent = 0);

signals:
    void aboutToShow(CustomMenu *customMenu);
    void aboutToHide(CustomMenu *customMenu);

private slots:
    void emitCustomShowSignal();
    void emitCustomHideSignal();

};

#endif //QSERIALTERMINAL_CUSTOMMENU_H
