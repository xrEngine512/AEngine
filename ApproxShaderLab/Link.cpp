#include "Link.h"
#include "shaderlabguielement.h"

#include <qgraphicsview.h>

Link::Link(QGraphicsView* view, std::pair<QWidget*, ShaderLabGUIElement*>& req) : m_item(req.second), m_view(view), m_wgt(req.first)
{
	QPointF p1(view->mapToScene(view->mapFromGlobal(m_wgt->mapToGlobal(QRect(QPoint(0, 0),m_wgt->size()).center()))));
	m_end_slot = m_item->addLink(*this);
	QPointF p2 = m_item->slotPos(m_end_slot);
	m_path.moveTo(p1);
	m_path.cubicTo(QPointF(p2.x(), p1.y()), QPointF(p1.x(), p2.y()), p2);
	m_pathItem = m_item->scene()->addPath(m_path, QPen(QBrush(QColor(180, 180, 180)), 2));
	m_pathItem->setData(0, 5);
}

const QPainterPath& Link::Path()const
{
	return m_path;
}

QGraphicsPathItem* Link::GetPathItem()const
{
	return m_pathItem;
}

std::pair<QWidget*, ShaderLabGUIElement*> Link::GetLinkingObjects()const
{
	return std::pair<QWidget*, ShaderLabGUIElement*>(m_wgt, m_item);
}

void Link::Update()
{
	QPointF p1(m_view->mapToScene(m_view->mapFromGlobal(m_wgt->mapToGlobal(QRect(QPoint(0, 0), m_wgt->size()).center()))));
	QPointF p2 = m_item->slotPos(m_end_slot);
	m_path = QPainterPath(p1);
	m_path.cubicTo(QPointF(p2.x(), p1.y()), QPointF(p1.x(), p2.y()), p2);
	m_item->scene()->removeItem(m_pathItem);
	delete m_pathItem;
	m_pathItem = m_item->scene()->addPath(m_path, QPen(QBrush(QColor(180, 180, 180)), 2));
}

Link::~Link()
{

}
