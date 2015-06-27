#include "ApproxGraphicsView.h"
#include <QMouseEvent>
#include "ApproxShaderView.h"
#include "CodeEditor.h"
#include <qgraphicsitem.h>
#include <qmenu.h>
#include "Enums.h"

namespace ApproxGUI
{
    ApproxGraphicsView::ApproxGraphicsView(QWidget *parent)
        : QGraphicsView(parent), m_xPrev(0), m_yPrev(0), m_dragging(false)
    {
        setCursor(Qt::CrossCursor);
        
		setInteractive(true);
		setRenderHint(QPainter::Antialiasing);
    }

    ApproxGraphicsView::~ApproxGraphicsView()
    {

    }

    void ApproxGraphicsView::onContextMenu(QAction* act)
    {
        if (act->text().compare(QStringLiteral("��������� ������")) == 0)
        {
			emit createShaderElem(ASL::VS);
        }
        else if (act->text().compare(QStringLiteral("���������� ������")) == 0)
        {
			emit createShaderElem(ASL::PS);
        }
        else if (act->text().compare(QStringLiteral("�������������� ������")) == 0)
        {
			emit createShaderElem(ASL::GS);
        }
        else if (act->text().compare(QStringLiteral("������ �����������")) == 0)
        {
			emit createShaderElem(ASL::HS);
        }
        else if (act->text().compare(QStringLiteral("�������������� ������")) == 0)
        {
			emit createShaderElem(ASL::CS);
        }
        else if (act->text().compare(QStringLiteral("�������� ������")) == 0)
        {
			emit createShaderElem(ASL::DS);
        }
    }

    void ApproxGraphicsView::mouseMoveEvent(QMouseEvent* event)
    {        
        if (m_dragging)
        {
            QRectF rect = sceneRect();
            rect.setTopLeft(QPointF(m_xPrev - event->localPos().x()  + rect.x(), m_yPrev - event->localPos().y() + rect.y()));
            m_xPrev = event->localPos().x();
            m_yPrev = event->localPos().y();
            setSceneRect(rect);
			emit sceneMoved();
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void ApproxGraphicsView::mousePressEvent(QMouseEvent *event)
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

	void ApproxGraphicsView::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MidButton)
		{
			m_dragging = false;
			setCursor(Qt::CrossCursor);
		}
		QGraphicsView::mouseReleaseEvent(event);
	}

    void ApproxGraphicsView::contextMenuEvent(QContextMenuEvent* event)
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
}

void ApproxGUI::ApproxGraphicsView::resizeEvent(QResizeEvent* e)
{
	emit viewChanged();
	QGraphicsView::resizeEvent(e);
}