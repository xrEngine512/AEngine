/********************************************************************************
** Form generated from reading UI file 'shaderlabgui.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHADERLABGUI_H
#define UI_SHADERLABGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShaderLabGUI
{
public:

    void setupUi(QWidget *ShaderLabGUI)
    {
        if (ShaderLabGUI->objectName().isEmpty())
            ShaderLabGUI->setObjectName(QStringLiteral("ShaderLabGUI"));
        ShaderLabGUI->resize(1079, 860);

        retranslateUi(ShaderLabGUI);

        QMetaObject::connectSlotsByName(ShaderLabGUI);
    } // setupUi

    void retranslateUi(QWidget *ShaderLabGUI)
    {
        ShaderLabGUI->setWindowTitle(QApplication::translate("ShaderLabGUI", "ApproxShaderLab", 0));
    } // retranslateUi

};

namespace Ui {
    class ShaderLabGUI: public Ui_ShaderLabGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHADERLABGUI_H
