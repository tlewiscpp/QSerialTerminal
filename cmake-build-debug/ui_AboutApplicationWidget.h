/********************************************************************************
** Form generated from reading UI file 'AboutApplicationWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTAPPLICATIONWIDGET_H
#define UI_ABOUTAPPLICATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AboutApplicationWidget
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *lblProgramIcon;
    QLabel *lblProgramName;
    QLabel *lblProgramVersion;
    QLabel *lblProgramDescription;
    QLabel *lblProgramWebsite;
    QLabel *lblProgramCopyright;
    QTextEdit *tbAboutProgramLicense;
    QHBoxLayout *btnLicenseCloseGridLayout;
    QPushButton *btnLicense;
    QSpacerItem *btnLicenseCloseHorizontalSpacer;
    QPushButton *btnCloseDialog;

    void setupUi(QWidget *AboutApplicationWidget)
    {
        if (AboutApplicationWidget->objectName().isEmpty())
            AboutApplicationWidget->setObjectName(QStringLiteral("AboutApplicationWidget"));
        AboutApplicationWidget->resize(580, 494);
        verticalLayout_2 = new QVBoxLayout(AboutApplicationWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lblProgramIcon = new QLabel(AboutApplicationWidget);
        lblProgramIcon->setObjectName(QStringLiteral("lblProgramIcon"));
        QFont font;
        font.setPointSize(11);
        lblProgramIcon->setFont(font);
        lblProgramIcon->setPixmap(QPixmap(QString::fromUtf8(":/icons/icons/qserialterminal.png")));
        lblProgramIcon->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblProgramIcon);

        lblProgramName = new QLabel(AboutApplicationWidget);
        lblProgramName->setObjectName(QStringLiteral("lblProgramName"));
        QFont font1;
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        lblProgramName->setFont(font1);
        lblProgramName->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblProgramName);

        lblProgramVersion = new QLabel(AboutApplicationWidget);
        lblProgramVersion->setObjectName(QStringLiteral("lblProgramVersion"));
        QFont font2;
        font2.setPointSize(16);
        lblProgramVersion->setFont(font2);
        lblProgramVersion->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblProgramVersion);

        lblProgramDescription = new QLabel(AboutApplicationWidget);
        lblProgramDescription->setObjectName(QStringLiteral("lblProgramDescription"));
        QFont font3;
        font3.setPointSize(12);
        lblProgramDescription->setFont(font3);
        lblProgramDescription->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblProgramDescription);

        lblProgramWebsite = new QLabel(AboutApplicationWidget);
        lblProgramWebsite->setObjectName(QStringLiteral("lblProgramWebsite"));
        lblProgramWebsite->setFont(font);
        lblProgramWebsite->setTextFormat(Qt::AutoText);
        lblProgramWebsite->setAlignment(Qt::AlignCenter);
        lblProgramWebsite->setTextInteractionFlags(Qt::TextBrowserInteraction);

        verticalLayout->addWidget(lblProgramWebsite);

        lblProgramCopyright = new QLabel(AboutApplicationWidget);
        lblProgramCopyright->setObjectName(QStringLiteral("lblProgramCopyright"));
        lblProgramCopyright->setFont(font3);
        lblProgramCopyright->setTextFormat(Qt::AutoText);
        lblProgramCopyright->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(lblProgramCopyright);

        tbAboutProgramLicense = new QTextEdit(AboutApplicationWidget);
        tbAboutProgramLicense->setObjectName(QStringLiteral("tbAboutProgramLicense"));
        tbAboutProgramLicense->setMinimumSize(QSize(0, 200));

        verticalLayout->addWidget(tbAboutProgramLicense);


        verticalLayout_2->addLayout(verticalLayout);

        btnLicenseCloseGridLayout = new QHBoxLayout();
        btnLicenseCloseGridLayout->setObjectName(QStringLiteral("btnLicenseCloseGridLayout"));
        btnLicenseCloseGridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        btnLicense = new QPushButton(AboutApplicationWidget);
        btnLicense->setObjectName(QStringLiteral("btnLicense"));
        btnLicense->setFont(font2);

        btnLicenseCloseGridLayout->addWidget(btnLicense);

        btnLicenseCloseHorizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        btnLicenseCloseGridLayout->addItem(btnLicenseCloseHorizontalSpacer);

        btnCloseDialog = new QPushButton(AboutApplicationWidget);
        btnCloseDialog->setObjectName(QStringLiteral("btnCloseDialog"));
        btnCloseDialog->setFont(font2);

        btnLicenseCloseGridLayout->addWidget(btnCloseDialog);


        verticalLayout_2->addLayout(btnLicenseCloseGridLayout);


        retranslateUi(AboutApplicationWidget);

        QMetaObject::connectSlotsByName(AboutApplicationWidget);
    } // setupUi

    void retranslateUi(QWidget *AboutApplicationWidget)
    {
        AboutApplicationWidget->setWindowTitle(QApplication::translate("AboutApplicationWidget", "Form", Q_NULLPTR));
        lblProgramIcon->setText(QString());
        lblProgramName->setText(QString());
        lblProgramVersion->setText(QApplication::translate("AboutApplicationWidget", "vX.XX.X", Q_NULLPTR));
        lblProgramDescription->setText(QString());
#ifndef QT_NO_TOOLTIP
        lblProgramWebsite->setToolTip(QApplication::translate("AboutApplicationWidget", "<html><head/><body><p><span style=\" font-size:11pt;\">Open the upstream URL for QMineSweeper in a default web browser</span></p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        lblProgramWebsite->setText(QString());
        lblProgramCopyright->setText(QApplication::translate("AboutApplicationWidget", "Copyright 2016 - ", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnLicense->setToolTip(QApplication::translate("AboutApplicationWidget", "<html><head/><body><p>View license information about QMineSweeper</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnLicense->setText(QApplication::translate("AboutApplicationWidget", "License", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        btnCloseDialog->setToolTip(QApplication::translate("AboutApplicationWidget", "<html><head/><body><p>Close this window</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        btnCloseDialog->setText(QApplication::translate("AboutApplicationWidget", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutApplicationWidget: public Ui_AboutApplicationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTAPPLICATIONWIDGET_H
