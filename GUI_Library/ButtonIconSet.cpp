#include "ButtonIconSet.h"
#include <qstring.h>
#include "ApproxGuiResManager.h"

ButtonIconSet::ButtonIconSet(QString btnName)
{
	QString qrcDir = ":/Editor/" + btnName + "/";
	m_pixHovered = g_ResManager->GetPic(qrcDir + "hovered.png");
	m_pixIdle = g_ResManager->GetPic(qrcDir + "idle.png");
	m_pixPressed = g_ResManager->GetPic(qrcDir + "pressed.png");
}

QPixmap* ButtonIconSet::Idle() const
{
	return m_pixIdle;
}

QPixmap* ButtonIconSet::Hovered() const
{
	return m_pixHovered;
}

QPixmap* ButtonIconSet::Pressed() const
{
	return m_pixPressed;
}

ButtonIconSet::~ButtonIconSet()
{
}
