#include "wnd.h"

wnd::wnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.txtEntryPoint->setToolTip(QStringLiteral("По умолчанию используется 'main'"));
    ui.comboBox->setStyleSheet("QComboBox QAbstractItemView { selection-background-color: darkgray; background-color: lightgray;}");
}

wnd::~wnd()
{

}
