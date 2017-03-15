#pragma once

class QPixmap;
class QString;

class ButtonIconSet
{
	QPixmap* m_pixIdle, *m_pixHovered, *m_pixPressed;
public:
	ButtonIconSet(QString btnName);
	QPixmap* Idle()const;
	QPixmap* Hovered()const;
	QPixmap* Pressed()const;
	~ButtonIconSet();
};

