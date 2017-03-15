#include "LinkingPoint.h"
#include <ApproxGuiResManager.h>
#include <qevent.h>

namespace ASL
{
	LinkingPoint::LinkingPoint(QWidget *parent)
		: QLabel(parent), m_refCount(0)
	{
	}

	LinkingPoint::~LinkingPoint()
	{

	}

	void LinkingPoint::enterEvent(QEvent* e)
	{
		setCursor(Qt::CursorShape::PointingHandCursor);
		BaseClass::enterEvent(e);
	}

	void LinkingPoint::leaveEvent(QEvent* e)
	{
		unsetCursor();
		BaseClass::leaveEvent(e);
	}

	void LinkingPoint::setInUse()
	{
		setPixmap(g_ResManager->GetPic(":/ShaderEditor/flag_point_in_use.png")->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	}
	void LinkingPoint::setInFocus()
	{
		setPixmap(g_ResManager->GetPic(":/ShaderEditor/flag_point_in_use_in_focus.png")->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	}
	void LinkingPoint::setNotInUse()
	{
		setPixmap(g_ResManager->GetPic(":/ShaderEditor/flag_point_empty.png")->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	}

	void LinkingPoint::addRef()
	{
		m_refCount++;
		setInUse();
	}

	void LinkingPoint::deleteRef()
	{
		if (m_refCount == 0)
			setNotInUse();
		else
		{
			m_refCount--;
			if (m_refCount == 0)
				setNotInUse();
		}
	}

	void LinkingPoint::mousePressEvent(QMouseEvent* e)
	{
		if (e->buttons().testFlag(Qt::LeftButton))
		{
			e->accept();
		}
		BaseClass::mousePressEvent(e);
	}
	void LinkingPoint::mouseReleaseEvent(QMouseEvent* e)
	{
		if (e->button() == Qt::LeftButton)
		{
			emit linkAttempt(e->globalPos());
		}
		BaseClass::mouseReleaseEvent(e);
	}
	void LinkingPoint::mouseMoveEvent(QMouseEvent* e)
	{
		BaseClass::mouseMoveEvent(e);
	}
}