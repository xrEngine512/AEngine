#include "ApproxGUIButton.h"

#include <QtCore/QtPlugin>
#include "myqtplugin.h"


MyQtPlugin::MyQtPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}

void MyQtPlugin::initialize(QDesignerFormEditorInterface * )
{
    if (initialized)
        return;
    
    initialized = true;
}

bool MyQtPlugin::isInitialized() const
{
    return initialized;
}

QWidget *MyQtPlugin::createWidget(QWidget *parent)
{
    return new ApproxGUIButton(parent);
}

QString MyQtPlugin::name() const
{
    return "ApproxGUIButton";
}

QString MyQtPlugin::group() const
{
    return "ApproxGUIElements";
}

QIcon MyQtPlugin::icon() const
{
    return QIcon();
}

QString MyQtPlugin::toolTip() const
{
    return QString();
}

QString MyQtPlugin::whatsThis() const
{
    return QString();
}

bool MyQtPlugin::isContainer() const
{
    return false;
}

QString MyQtPlugin::domXml() const
{
    return "<widget class=\"ApproxGUIButton\" name=\"apbtn\">\n"
        " <property name=\"geometry\">\n"
        "  <rect>\n"
        "   <x>0</x>\n"
        "   <y>0</y>\n"
        "   <width>100</width>\n"
        "   <height>25</height>\n"
        "  </rect>\n"
        " </property>\n"
        "</widget>\n";
}

QString MyQtPlugin::includeFile() const
{
    return "ApproxGUIButton.h";
}

