/***********************************************************************
*    aboutqmswidget.cpp:                                               *
*    Custom QWidget for showing information about QMineSweeper         *
*    Copyright (c) 2017 Tyler Lewis                                    *
************************************************************************
*    This is a source file for QMineSweeper:                           *
*    https://github.com/tlewiscpp/QMineSweeper                         *
*    The source code is released under the GNU LGPL                    *
*    This file holds the implementation of an AboutQmsWidget class     *
*    The AboutQmsWidget class is a window showing information about    *
*    QMineSweeper, using the same format used by the program GHex      *
*                                                                      *
*    You should have received a copy of the GNU Lesser General         *
*    Public license along with QMineSweeper                            *
*    If not, see <http://www.gnu.org/licenses/>                        *
***********************************************************************/

#include "AboutApplicationWidget.h"
#include "ui_AboutApplicationWidget.h"

#include "GlobalDefinitions.h"
#include "ApplicationStrings.h"
#include "ApplicationSettings.h"

#include <QDesktopWidget>
#include <QDateTime>
#include <QPushButton>
#include <QtGui/QTextCursor>

AboutApplicationWidget::AboutApplicationWidget(QWidget *parent) :
    m_ui{new Ui::AboutApplicationWidget{}},
    m_licenseHiddenHeight{-1}
{

    using namespace ApplicationStrings;
    using namespace GlobalSettings;
    this->m_ui->setupUi(this);

    this->m_ui->lblProgramName->setText(PROGRAM_NAME);
    this->m_ui->lblProgramDescription->setText(PROGRAM_DESCRIPTION);
    QString currentText{this->m_ui->lblProgramCopyright->text()};
    QString currentYear{QS_NUMBER(QDateTime{}.currentDateTime().date().year())};
    this->m_ui->lblProgramCopyright->setText(QString{"%1 %2 %3"}.arg(currentText, currentYear, AUTHOR_NAME));
    this->m_ui->lblProgramVersion->setText(QString{"v%1.%2.%3"}.arg(QS_NUMBER(SOFTWARE_MAJOR_VERSION), QS_NUMBER(SOFTWARE_MINOR_VERSION), QS_NUMBER(SOFTWARE_PATCH_VERSION)));
    this->m_ui->lblProgramWebsite->setText(QString{"<html><head/><body><p><a href=\"%1\"><span style=\" text-decoration: underline; color:#007af4;\">QMineSweeper website</span></a></p></body></html>"}.arg(REMOTE_URL));
    this->m_ui->lblProgramWebsite->setOpenExternalLinks(true);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->m_ui->tbAboutProgramLicense->setVisible(false);

    this->connect(this->m_ui->btnLicense, &QPushButton::clicked, this, &AboutApplicationWidget::onLicenseButtonClicked);
    this->connect(this->m_ui->btnCloseDialog, &QPushButton::clicked, this, &AboutApplicationWidget::onCloseButtonClicked);
}

AboutApplicationWidget::~AboutApplicationWidget()
{

}

/* onAboutQmsWindowLicenseButtonClicked() : called when the license button on the
 * AboutQmsWindow is clicked. This method displays the licence for QMineSweeper*/
void AboutApplicationWidget::onLicenseButtonClicked(bool checked)
{
    (void)checked;
    if (this->m_ui->tbAboutProgramLicense->isVisible()) {
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
    this->m_ui->tbAboutProgramLicense->setVisible(!this->m_ui->tbAboutProgramLicense->isVisible());
    //this->setFixedSize(this->minimumSize());
}

void AboutApplicationWidget::populateLicenseText()
{
    QFile licenseFile{ApplicationStrings::LICENSE_PATH};
    licenseFile.open(QIODevice::OpenModeFlag::ReadOnly);
    this->m_ui->tbAboutProgramLicense->append(licenseFile.readAll());
    licenseFile.close();
    this->m_ui->tbAboutProgramLicense->moveCursor(QTextCursor::Start);
    this->m_ui->tbAboutProgramLicense->ensureCursorVisible();
}

void AboutApplicationWidget::clearLicenseText()
{
    this->m_ui->tbAboutProgramLicense->clear();
}

/* onAboutQmsWindowCloseButtonClicked() : called when the close button on the
 * AboutQmsWindow is clicked. This method closes the window */
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

