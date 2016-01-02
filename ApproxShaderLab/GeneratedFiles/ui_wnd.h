/********************************************************************************
** Form generated from reading UI file 'wnd.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WND_H
#define UI_WND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wnd
{
public:
    QLineEdit *txtShaderName;
    QLabel *label;
    QLineEdit *txtEntryPoint;
    QLabel *label_2;
    QComboBox *comboBox;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *txtWorkLib;

    void setupUi(QWidget *wnd)
    {
        if (wnd->objectName().isEmpty())
            wnd->setObjectName(QStringLiteral("wnd"));
        wnd->resize(209, 300);
        QPalette palette;
        QBrush brush(QColor(165, 165, 165, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(150, 150, 150, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        wnd->setPalette(palette);
        txtShaderName = new QLineEdit(wnd);
        txtShaderName->setObjectName(QStringLiteral("txtShaderName"));
        txtShaderName->setGeometry(QRect(10, 30, 191, 20));
        label = new QLabel(wnd);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 81, 16));
        txtEntryPoint = new QLineEdit(wnd);
        txtEntryPoint->setObjectName(QStringLiteral("txtEntryPoint"));
        txtEntryPoint->setGeometry(QRect(10, 80, 191, 20));
        label_2 = new QLabel(wnd);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 81, 16));
        comboBox = new QComboBox(wnd);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(10, 130, 151, 22));
        label_3 = new QLabel(wnd);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 110, 101, 16));
        label_4 = new QLabel(wnd);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 160, 81, 16));
        txtWorkLib = new QLineEdit(wnd);
        txtWorkLib->setObjectName(QStringLiteral("txtWorkLib"));
        txtWorkLib->setGeometry(QRect(10, 180, 191, 21));

        retranslateUi(wnd);

        QMetaObject::connectSlotsByName(wnd);
    } // setupUi

    void retranslateUi(QWidget *wnd)
    {
        wnd->setWindowTitle(QString());
        txtShaderName->setText(QApplication::translate("wnd", "newShader", 0));
        label->setText(QApplication::translate("wnd", "\320\230\320\274\321\217 \321\210\320\265\320\271\320\264\320\265\321\200\320\260", 0));
        txtEntryPoint->setText(QString());
        label_2->setText(QApplication::translate("wnd", "\320\242\320\276\321\207\320\272\320\260 \320\262\321\205\320\276\320\264\320\260", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("wnd", "5.0", 0)
         << QApplication::translate("wnd", "4.1", 0)
         << QApplication::translate("wnd", "4.0", 0)
        );
        label_3->setText(QApplication::translate("wnd", "\320\234\320\276\320\264\320\265\320\273\321\214 \321\210\320\265\320\271\320\264\320\265\321\200\320\260", 0));
        label_4->setText(QApplication::translate("wnd", "\320\240\320\260\320\261\320\276\321\207\320\260\321\217 \320\277\320\260\320\277\320\272\320\260", 0));
        txtWorkLib->setText(QApplication::translate("wnd", "../", 0));
    } // retranslateUi

};

namespace Ui {
    class wnd: public Ui_wnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WND_H
