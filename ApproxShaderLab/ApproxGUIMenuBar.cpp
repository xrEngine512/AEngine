#include "ApproxGUIMenuBar.h"

namespace ApproxGUI
{
    ApproxGUIMenuBar::ApproxGUIMenuBar(QWidget *parent)
        : QWidget(parent)
    {        
    }

    void ApproxGUIMenuBar::addWidget(QWidget* widget)
    {
        widget->setParent(this);
    }

    ApproxGUIMenuBar::~ApproxGUIMenuBar()
    {

    }
}