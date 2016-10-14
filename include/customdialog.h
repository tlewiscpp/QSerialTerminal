#ifndef QSERIALTERMINAL_CUSTOMDIALOG_H
#define QSERIALTERMINAL_CUSTOMDIALOG_H

#include <QDialog>

class CustomDialog : public QDialog
{
    Q_OBJECT
public:

signals:
    void aboutToClose();

public:
    void closeEvent(QCloseEvent *ce)
    {
        emit (aboutToClose());
        QDialog::closeEvent(ce);
    }
};

#endif //QSERIALTERMINAL_CUSTOMDIALOG_H
