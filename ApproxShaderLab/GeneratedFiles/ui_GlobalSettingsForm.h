/********************************************************************************
** Form generated from reading UI file 'GlobalSettingsForm.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLOBALSETTINGSFORM_H
#define UI_GLOBALSETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GlobalSettingsForm
{
public:
    QGroupBox *groupBox;
    QLineEdit *txtCompileDir;
    QGroupBox *groupBox_2;
    QLineEdit *txtProjectDir;

    void setupUi(QWidget *GlobalSettingsForm)
    {
        if (GlobalSettingsForm->objectName().isEmpty())
            GlobalSettingsForm->setObjectName(QStringLiteral("GlobalSettingsForm"));
        GlobalSettingsForm->resize(310, 443);
        groupBox = new QGroupBox(GlobalSettingsForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 291, 51));
        txtCompileDir = new QLineEdit(groupBox);
        txtCompileDir->setObjectName(QStringLiteral("txtCompileDir"));
        txtCompileDir->setGeometry(QRect(10, 20, 271, 20));
        groupBox_2 = new QGroupBox(GlobalSettingsForm);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 80, 291, 51));
        txtProjectDir = new QLineEdit(groupBox_2);
        txtProjectDir->setObjectName(QStringLiteral("txtProjectDir"));
        txtProjectDir->setGeometry(QRect(10, 20, 271, 20));

        retranslateUi(GlobalSettingsForm);

        QMetaObject::connectSlotsByName(GlobalSettingsForm);
    } // setupUi

    void retranslateUi(QWidget *GlobalSettingsForm)
    {
        GlobalSettingsForm->setWindowTitle(QApplication::translate("GlobalSettingsForm", "GlobalSettingsForm", 0));
        groupBox->setTitle(QApplication::translate("GlobalSettingsForm", "\320\232\320\276\321\200\320\275\320\265\320\262\320\260\321\217 \320\277\320\260\320\277\320\272\320\260 \321\201\320\272\320\276\320\274\320\277\320\270\320\273\320\270\321\200\320\276\320\262\320\260\320\275\320\275\321\213\321\205 \321\210\320\265\320\271\320\264\320\265\321\200\320\276\320\262", 0));
        groupBox_2->setTitle(QApplication::translate("GlobalSettingsForm", "\320\232\320\276\321\200\320\275\320\265\320\262\320\260\321\217 \320\277\320\260\320\277\320\272\320\260 \321\201\320\276\321\205\321\200\320\260\320\275\320\265\320\275\320\275\321\213\321\205 \320\277\321\200\320\276\320\265\320\272\321\202\320\276\320\262 \321\210\320\265\320\271\320\264\320\265\321\200\320\276\320\262", 0));
    } // retranslateUi

};

namespace Ui {
    class GlobalSettingsForm: public Ui_GlobalSettingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLOBALSETTINGSFORM_H
