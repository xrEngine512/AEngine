#include "Link.h"
#include "shaderlabguielement.h"
#include "MaterialVar.h"
#include "MaterialVarPoint.h"
#include <qgraphicsview.h>

namespace ASL
{
	Link::Link(QGraphicsView* view, std::pair<MaterialVar*, ShaderLabGUIElement*>& req) : m_item(req.second), m_view(view), m_wgt(req.first)
	{
		QPointF p1(view->mapToScene(view->mapFromGlobal(m_wgt->Point()->mapToGlobal(QRect(QPoint(0, 0), m_wgt->Point()->size()).center()))));
		m_end_slot = m_item->addLink(*this);
		QPointF p2 = m_item->slotPos(m_end_slot);
		QPainterPath path(p1);
		path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
		if (m_item->hasFocus())
			m_pathItem = m_item->scene()->addPath(path, QPen(QColor(60, 150, 200), 2));
		else
			m_pathItem = m_item->scene()->addPath(path, QPen(QColor(150, 150, 150), 2));
	}

	QGraphicsPathItem* Link::GetPathItem()const
	{
		return m_pathItem;
	}

	Link::SaveData Link::GetSaveData() const
	{
		return SaveData(m_wgt->VarInfo().ID, m_item->getShader_Type());
	}

	std::pair<MaterialVar*, ShaderLabGUIElement*> Link::GetLinkingObjects()const
	{
		return std::pair<MaterialVar*, ShaderLabGUIElement*>(m_wgt, m_item);
	}

	void Link::Update()
	{
		QPointF p1(m_view->mapToScene(m_view->mapFromGlobal(m_wgt->Point()->mapToGlobal(QRect(QPoint(0, 0), m_wgt->Point()->size()).center()))));
		QPointF p2 = m_item->slotPos(m_end_slot);
		QPainterPath path(p1);
		path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
		m_pathItem->setPath(path);
	}

	Link::~Link()
	{
		m_pathItem->scene()->removeItem(m_pathItem);
		delete m_pathItem;
	}
}