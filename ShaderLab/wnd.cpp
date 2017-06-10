#include "wnd.h"

wnd::wnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    auto style = "QComboBox QAbstractItemView { selection-background-color: darkgray; background-color: lightgray;}";
    ui.shader_subsystem->setStyleSheet(style);
    ui.shader_language->setStyleSheet(style);
}

wnd::~wnd()
{

}
