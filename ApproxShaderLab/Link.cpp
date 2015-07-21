#include "Link.h"



namespace ASL
{
#define LocalClass AbstractLink<LinkingClass1, LinkingClass2>

	/*template <class LinkingClass1, class LinkingClass2>
	inline LocalClass::AbstractLink(QGraphicsView* view, std::pair<LinkingClass1*, LinkingClass2*>& req) 
	{
		
	}
	template <class LinkingClass1, class LinkingClass2>
	inline QGraphicsPathItem* LocalClass::GetPathItem()const
	{
		
	}
	
	
	template <class LinkingClass1, class LinkingClass2>
	inline LocalClass::AbstractLink(QGraphicsView* view, LinkingClass1* var, LinkingClass2* item) : m_item(item), m_view(view), m_var(var)
	{
		QPointF p1(view->mapToScene(view->mapFromGlobal(m_var->Point().mapToGlobal(QRect(QPoint(0, 0), m_var->Point().size()).center()))));
		m_end_slot = m_item->addLink(*static_cast< Link<MaterialVar, ShaderLabGUIElement, MatVarSaveData> *>(this));
		QPointF p2 = m_item->slotPos(m_end_slot);
		QPainterPath path(p1);
		path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
		if (m_item->hasFocus())
			m_pathItem = m_item->scene()->addPath(path, QPen(QColor(60, 150, 200), 2));
		else
			m_pathItem = m_item->scene()->addPath(path, QPen(QColor(150, 150, 150), 2));
		m_var->addRef();
	}
	template <class LinkingClass1, class LinkingClass2>
	inline void LocalClass::Update()
	{
		QPointF p1(m_view->mapToScene(m_view->mapFromGlobal(m_var->Point().mapToGlobal(QRect(QPoint(0, 0), m_var->Point().size()).center()))));
		QPointF p2 = m_item->slotPos(m_end_slot);
		QPainterPath path(p1);
		path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
		m_pathItem->setPath(path);
	}
	template <class LinkingClass1, class LinkingClass2>
	inline LocalClass::~AbstractLink()
	{
		m_pathItem->scene()->removeItem(m_pathItem);
		delete m_pathItem;
		m_item->deleteLink(*static_cast< Link<MaterialVar, ShaderLabGUIElement, MatVarSaveData> *>(this));
		m_var->deleteRef();
	}*/
}