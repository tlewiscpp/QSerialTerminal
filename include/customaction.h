#ifndef QSERIALTERMINAL_CUSTOMACTION_H
#define QSERIALTERMINAL_CUSTOMACTION_H

#include <QAction>

class CustomAction : public QAction
{

    Q_OBJECT
public:
    CustomAction(const QString &text, int index, QWidget *parent = 0);
    int index() const;
    void setIndex(int index);

private slots:
    void emitCustomSignal(bool);

signals:
    void triggered(CustomAction *customAction, bool checked);

private:
    int m_index;

};

#endif //QSERIALTERMINAL_CUSTOMACTION_H
