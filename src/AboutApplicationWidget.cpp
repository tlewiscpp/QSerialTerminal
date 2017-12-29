#include "AboutApplicationWidget.h"
#include "ui_AboutApplicationWidget.h"

#include "GlobalDefinitions.h"
#include "ApplicationStrings.h"
#include "ApplicationSettings.h"
#include "ApplicationIcons.h"

#include <QDesktopWidget>
#include <QDateTime>
#include <QPushButton>
#include <QTextCursor>
#include <QFile>
#include <QLabel>
#include <QPixmap>

AboutApplicationWidget::AboutApplicationWidget(QWidget *parent) :
    QWidget{parent},
    m_ui{new Ui::AboutApplicationWidget{}},
    m_licenseHiddenHeight{-1}
{

    using namespace ApplicationStrings;
    using namespace GlobalSettings;
    this->m_ui->setupUi(this);

    this->m_ui->lblProgramName->setText(GlobalSettings::PROGRAM_NAME);
    this->m_ui->lblProgramDescription->setText(GlobalSettings::PROGRAM_DESCRIPTION);
    QString currentText{this->m_ui->lblProgramCopyright->text()};
    QString currentYear{QS_NUMBER(QDateTime{}.currentDateTime().date().year())};
    this->m_ui->lblProgramCopyright->setText(QString{"%1 %2 %3"}.arg(currentText, currentYear, AUTHOR_NAME));
    this->m_ui->lblProgramVersion->setText(QString{"v%1.%2.%3"}.arg(QS_NUMBER(SOFTWARE_MAJOR_VERSION), QS_NUMBER(SOFTWARE_MINOR_VERSION), QS_NUMBER(SOFTWARE_PATCH_VERSION)));
    this->m_ui->lblProgramWebsite->setText(QString{
            R"(<html><head/><body><p><a href="%1"><span style=" text-decoration: underline; color:#007af4;">)" + QCoreApplication::applicationName() + " website</span></a></p></body></html>"}.arg(REMOTE_URL));
    this->m_ui->lblProgramWebsite->setOpenExternalLinks(true);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    this->m_ui->lblProgramIcon->setPixmap(applicationIcons->MAIN_WINDOW_ICON.pixmap(QSize{128, 128}));
    this->connect(this->m_ui->btnLicense, &QPushButton::clicked, this, &AboutApplicationWidget::onLicenseButtonClicked);
    this->connect(this->m_ui->btnCloseDialog, &QPushButton::clicked, this, &AboutApplicationWidget::onCloseButtonClicked);

    this->addLicenseTab(GlobalSettings::PROGRAM_NAME, ApplicationStrings::LICENSE_PATH);
    this->m_ui->twLicenses->setVisible(false);
}


void AboutApplicationWidget::onLicenseButtonClicked(bool checked)
{
    (void)checked;
    if (this->m_ui->twLicenses->isVisible()) {
        this->clearLicenseText();
        if (this->m_licenseHiddenHeight != -1) {
            QRect thisGeometry{this->geometry()};
            thisGeometry.setHeight(this->m_licenseHiddenHeight);
            this->setGeometry(thisGeometry);
        }
    } else {
        if (this->m_licenseHiddenHeight == -1) {
            this->m_licenseHiddenHeight = this->height();
        }
        this->populateLicenseText();
    }
    this->m_ui->twLicenses->setVisible(!this->m_ui->twLicenses->isVisible());
}

int AboutApplicationWidget::addLicenseTab(const QString &licenseName, const QString &licensePath)
{
    if (this->m_licenseTabs.find(licenseName.toStdString()) != this->m_licenseTabs.end()) {
        return -1;
    }
    QWidget *newTab{new QWidget{this->m_ui->twLicenses}};
    QGridLayout *newLayout{new QGridLayout{this->m_ui->twLicenses}};
    newLayout->setSpacing(0);
    newLayout->setContentsMargins(0, 0, 0, 0);
    QTextEdit *newTextEdit{new QTextEdit{newTab}};
    newTextEdit->setProperty(ApplicationStrings::LICENSE_PATH_KEY, QVariant{licensePath});
    newTextEdit->setReadOnly(true);
    newLayout->addWidget(newTextEdit);
    newTab->setLayout(newLayout);

    this->m_ui->twLicenses->addTab(newTab, licenseName);
    this->m_licenseTabs.emplace(licenseName.toStdString(), newTextEdit);
    return 0;
}

void AboutApplicationWidget::populateLicenseText()
{
    for (auto &it : this->m_licenseTabs) {
        QFile licenseFile{it.second->property(ApplicationStrings::LICENSE_PATH_KEY).toString()};
        if (!licenseFile.open(QIODevice::OpenModeFlag::ReadOnly)) {
            LOG_WARN() << QString{"Could not open license file %1"}.arg(it.second->property(ApplicationStrings::LICENSE_PATH_KEY).toString());
            continue;
        }
        it.second->append(licenseFile.readAll());
        licenseFile.close();
        it.second->moveCursor(QTextCursor::Start);
        it.second->ensureCursorVisible();
    }
}

void AboutApplicationWidget::clearLicenseText()
{
    for (auto &it : this->m_licenseTabs) {
        it.second->clear();
    }

}

void AboutApplicationWidget::onCloseButtonClicked(bool checked)
{
    (void)checked;
    this->close();
}

void AboutApplicationWidget::closeEvent(QCloseEvent *ce)
{
    emit (aboutToClose());
    QWidget::closeEvent(ce);
}


AboutApplicationWidget::~AboutApplicationWidget()
{
    delete this->m_ui;
}