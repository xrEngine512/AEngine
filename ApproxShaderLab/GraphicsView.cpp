#include "GraphicsView.h"
#include <QMouseEvent>
#include <qgraphicsitem.h>
#include <qmenu.h>
#include "Enums.h"

namespace ASL
{
	GraphicsView::GraphicsView(QWidget *parent)
		: QGraphicsView(parent), m_xPrev(0), m_yPrev(0), m_dragging(false)
	{
		setCursor(Qt::CrossCursor);

		setInteractive(true);
		setRenderHint(QPainter::Antialiasing);
	}

	GraphicsView::~GraphicsView()
	{

	}

	void GraphicsView::onContextMenu(QAction* act)
	{
		if (act->text().compare(QStringLiteral("��������� ������")) == 0)
		{
			emit createShaderElem(VS);
		}
		else if (act->text().compare(QStringLiteral("���������� ������")) == 0)
		{
			emit createShaderElem(PS);
		}
		else if (act->text().compare(QStringLiteral("�������������� ������")) == 0)
		{
			emit createShaderElem(GS);
		}
		else if (act->text().compare(QStringLiteral("������ �����������")) == 0)
		{
			emit createShaderElem(HS);
		}
		else if (act->text().compare(QStringLiteral("�������������� ������")) == 0)
		{
			emit createShaderElem(CS);
		}
		else if (act->text().compare(QStringLiteral("�������� ������")) == 0)
		{
			emit createShaderElem(DS);
		}
	}

	void GraphicsView::mouseMoveEvent(QMouseEvent* event)
	{
		if (m_dragging)
		{
			QRectF rect = sceneRect();
			rect.setTopLeft(QPointF(m_xPrev - event->localPos().x() + rect.x(), m_yPrev - event->localPos().y() + rect.y()));
			m_xPrev = event->localPos().x();
			m_yPrev = event->localPos().y();
			setSceneRect(rect);
			emit sceneMoved();
		}
		QGraphicsView::mouseMoveEvent(event);
	}

	void GraphicsView::mousePressEvent(QMouseEvent *event)
	{
		if (event->buttons().testFlag(Qt::MidButton))
		{
			bool someItemIsUnderCursor = false;
			for (auto item : scene()->items())
			{
				if (item->isUnderMouse())
				{
					someItemIsUnderCursor = true;
					break;
				}
			}
			if (!someItemIsUnderCursor)
			{
				event->accept();
				if (!m_dragging)
				{
					m_xPrev = event->localPos().x();
					m_yPrev = event->localPos().y();
					setCursor(Qt::CursorShape::ClosedHandCursor);
				}
				m_dragging = true;
			}
			else
			{
				QGraphicsView::mousePressEvent(event);
			}
		}
		else
		{
			QGraphicsView::mousePressEvent(event);
		}
	}

	void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MidButton)
		{
			m_dragging = false;
			setCursor(Qt::CrossCursor);
		}
		QGraphicsView::mouseReleaseEvent(event);
	}

	void GraphicsView::contextMenuEvent(QContextMenuEvent* event)
	{
		bool someItemIsUnderCursor = false;
		for (auto item : scene()->items())
		{
			if (item->isUnderMouse())
			{
				someItemIsUnderCursor = true;
				break;
			}
		}
		if (!someItemIsUnderCursor)
		{
			QMenu menu(this), submenu(QStringLiteral("�������� �������..."), &menu);
			submenu.addAction(QStringLiteral("��������� ������"));
			submenu.addAction(QStringLiteral("���������� ������"));
			submenu.addAction(QStringLiteral("�������������� ������"));
			submenu.addAction(QStringLiteral("������ �����������"));
			submenu.addAction(QStringLiteral("�������������� ������"));
			submenu.addAction(QStringLiteral("�������� ������"));
			menu.addMenu(&submenu);
			connect(&menu, SIGNAL(triggered(QAction*)), SLOT(onContextMenu(QAction*)));
			menu.exec(event->globalPos());
			event->accept();
		}
		else
		{
			QGraphicsView::contextMenuEvent(event);
		}
	}


	void GraphicsView::resizeEvent(QResizeEvent* e)
	{
		emit viewChanged();
		QGraphicsView::resizeEvent(e);
	}
}