#include "ApproxSizeGrip.h"
#include <QMouseEvent>
namespace ApproxGUI
{
	SizeGrip::SizeGrip(QWidget *parent)
		: QWidget(parent), m_moved(true), m_dragging(false)
	{
		resize(25, 25);
	}

	void SizeGrip::snapTo(Position p)
	{
		switch (p)
		{
		case TOP_LEFT:
		{
			move(0, 0);
			break;
		}
		case TOP_RIGHT:
		{
			move(parentWidget()->width() - width(), 0);
			break;
		}
		case BOTTOM_LEFT:
		{
			move(0, parentWidget()->height() - height());
			break;
		}
		case BOTTOM_RIGHT:
		{
			move(parentWidget()->width() - width(), parentWidget()->height() - height());
			break;
		}
		}
	}

	SizeGrip::~SizeGrip()
	{

	}

	void SizeGrip::snap()
	{
		return snapTo(GetPosition());
	}

	void SizeGrip::moveEvent(QMoveEvent* e)
	{
		m_moved = true;
		BaseClass::moveEvent(e);
	}

	void SizeGrip::mouseMoveEvent(QMouseEvent* event)
	{
		if (m_dragging)
		{
			move(event->globalPos());
			QWidget* Parent = parentWidget();
			QPoint mousePos = event->globalPos();
			QRect geom = Parent->geometry();
			switch (GetPosition())
			{
			case TOP_LEFT:
			{
				geom.setTopLeft(Parent->mapToParent(mousePos));
				break;
			}
			case TOP_RIGHT:
			{
				move(parentWidget()->width() - width(), 0);
				break;
			}
			case BOTTOM_LEFT:
			{
				move(0, parentWidget()->height() - height());
				break;
			}
			case BOTTOM_RIGHT:
			{
				geom.setBottomRight(geom.bottomRight() + mousePos - m_Prev);
				break;
			}
			}
			Parent->setGeometry(geom);
			m_Prev = event->globalPos();
		}
		BaseClass::mouseMoveEvent(event);
	}

	void SizeGrip::mousePressEvent(QMouseEvent* event)
	{
		if (event->buttons().testFlag(Qt::LeftButton))
		{
			if (!m_dragging)
			{
				m_Prev = event->globalPos();
			}
			m_dragging = true;
			event->accept();
		}
		else
		{
			event->ignore();
		}
		BaseClass::mousePressEvent(event);
	}

	void SizeGrip::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::LeftButton)
		{
			m_dragging = false;
			event->accept();
		}
		else
		{
			event->ignore();
		}
		BaseClass::mouseReleaseEvent(event);
	}

	void SizeGrip::enterEvent(QEvent* e)
	{
		GetPosition();
		if (m_lastPosition == BOTTOM_RIGHT || m_lastPosition == TOP_LEFT)
			setCursor(Qt::CursorShape::SizeFDiagCursor);
		else
			setCursor(Qt::CursorShape::SizeBDiagCursor);
		e->accept();
		BaseClass::enterEvent(e);
	}

	void SizeGrip::leaveEvent(QEvent* e)
	{
		e->accept();
		unsetCursor();
		BaseClass::leaveEvent(e);
	}

	Position SizeGrip::GetPosition()
	{
		if (m_moved)
		{
			QPoint pos = QWidget::pos();
			QRect parentFrame = QRect(0, 0, parentWidget()->size().width(), parentWidget()->size().height());
			bool left = pos.x() < parentFrame.center().x();
			bool top = pos.y() < parentFrame.center().y();
			if (top)
			{
				if (left)
					m_lastPosition = TOP_LEFT;
				else
					m_lastPosition = TOP_RIGHT;
			}
			else
			{
				if (left)
					m_lastPosition = BOTTOM_LEFT;
				else
					m_lastPosition = BOTTOM_RIGHT;
			}
			m_moved = false;
		}
		return m_lastPosition;
	}
}