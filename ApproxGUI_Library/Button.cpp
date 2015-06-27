#include "Button.h"
#include <QLabel>
#include "ButtonIconSet.h"

Button::Button(QString ButtonTypeName, QWidget *parent)
    : QPushButton(parent)
{
    resize(25, 25);
	m_IconSet = new ButtonIconSet(ButtonTypeName);
	m_lblIcon = new QLabel(this);
	m_lblIcon->setGeometry(0,0,width(),height());
	m_lblIcon->setPixmap(m_IconSet->Idle()->scaled(m_lblIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}



Button::~Button()
{
	delete m_IconSet;
}

/*void Button::paintEvent(QPaintEvent* )
{   
	QPixmap* pixmap;
    if (underMouse())
    {
        if (isDown())
        {
			pixmap = m_pixPressed;
        }
        else
        {
			pixmap = m_pixHovered;
        }
    }
	else
	{
		pixmap = m_pixIdle;
	}    
	
}*/

void Button::enterEvent(QEvent* e)
{
	m_lblIcon->setPixmap(m_IconSet->Hovered()->scaled(m_lblIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	return QPushButton::enterEvent(e);
}

void Button::leaveEvent(QEvent* e)
{
	m_lblIcon->setPixmap(m_IconSet->Idle()->scaled(m_lblIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	return QPushButton::leaveEvent(e);
}

void Button::mousePressEvent(QMouseEvent* e)
{
	m_lblIcon->setPixmap(m_IconSet->Pressed()->scaled(m_lblIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	return QPushButton::mousePressEvent(e);
}

void Button::mouseReleaseEvent(QMouseEvent* e)
{
	m_lblIcon->setPixmap(m_IconSet->Hovered()->scaled(m_lblIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	return QPushButton::mouseReleaseEvent(e);
}