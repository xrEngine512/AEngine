#include "MaterialVarPoint.h"

#include <qevent.h>

namespace ASL
{
	MaterialVarPoint::MaterialVarPoint(QWidget *parent)
		: QLabel(parent)
	{

	}

	MaterialVarPoint::~MaterialVarPoint()
	{

	}

	void MaterialVarPoint::enterEvent(QEvent* e)
	{
		setCursor(Qt::CursorShape::PointingHandCursor);
		BaseClass::enterEvent(e);
	}

	void MaterialVarPoint::leaveEvent(QEvent* e)
	{
		unsetCursor();
		BaseClass::leaveEvent(e);
	}

	void MaterialVarPoint::mousePressEvent(QMouseEvent* e)
	{
		if (e->buttons().testFlag(Qt::LeftButton))
		{
			e->accept();
		}
		BaseClass::mousePressEvent(e);
	}
	void MaterialVarPoint::mouseReleaseEvent(QMouseEvent* e)
	{
		if (e->button() == Qt::LeftButton)
		{
			emit linkAttempt(e->globalPos());
		}
		BaseClass::mouseReleaseEvent(e);
	}
	void MaterialVarPoint::mouseMoveEvent(QMouseEvent* e)
	{
		BaseClass::mouseMoveEvent(e);
	}
}