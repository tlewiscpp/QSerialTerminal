#ifndef QSERIALTERMINAL_ABOUTAPPLICATIONWIDGET_H
#define QSERIALTERMINAL_ABOUTAPPLICATIONWIDGET_H

namespace Ui {
    class AboutApplicationWidget;
}

#include <memory>
#include <QWidget>

class QDesktopWidget;

class AboutApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    AboutApplicationWidget(QWidget *parent = nullptr);
    ~AboutApplicationWidget();

signals:
    void aboutToClose();
    void keyPressEvent(QKeyEvent *event) override;

public:
    void closeEvent(QCloseEvent *ce) override;

private slots:
    void onLicenseButtonClicked(bool checked);
    void onCloseButtonClicked(bool checked);
private:
    std::unique_ptr<Ui::AboutApplicationWidget> m_ui;
    int m_licenseHiddenHeight;

    void populateLicenseText();
    void clearLicenseText();
};

#endif //QSERIALTERMINAL_ABOUTAPPLICATIONWIDGET_H
