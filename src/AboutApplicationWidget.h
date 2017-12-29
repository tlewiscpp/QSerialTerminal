#ifndef TRMIDOUBLECHECKBOX_ABOUTAPPLICATIONWIDGET_H
#define TRMIDOUBLECHECKBOX_ABOUTAPPLICATIONWIDGET_H

namespace Ui {
    class AboutApplicationWidget;
}

#include <memory>
#include <QWidget>
#include <QtWidgets/QTextEdit>

class QDesktopWidget;

class AboutApplicationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutApplicationWidget(QWidget *parent = nullptr);
    ~AboutApplicationWidget() override;

    AboutApplicationWidget(const AboutApplicationWidget &rhs) = delete;
    AboutApplicationWidget(AboutApplicationWidget &&rhs) = delete;
    AboutApplicationWidget &operator=(const AboutApplicationWidget &rhs) = delete;
    AboutApplicationWidget &operator=(AboutApplicationWidget &&rhs) = delete;

signals:
    void aboutToClose();
    void keyPressEvent(QKeyEvent *event) override;

public:
    void closeEvent(QCloseEvent *ce) override;

    int addLicenseTab(const QString &licenseName, const QString &licensePath);

protected:
    void onLicenseButtonClicked(bool checked);
    void onCloseButtonClicked(bool checked);
private:
    Ui::AboutApplicationWidget *m_ui;
    int m_licenseHiddenHeight;
    std::map<std::string, QTextEdit *> m_licenseTabs;

    void populateLicenseText();
    void clearLicenseText();

};

#endif //TRMIDOUBLECHECKBOX_ABOUTAPPLICATIONWIDGET_H
