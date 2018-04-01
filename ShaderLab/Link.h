#ifndef LINK_H
#define LINK_H
#include <QPainterPath>

#include "ShaderLabGUIElement.h"
#include "ShaderSettingsElement.h"
#include "MaterialVar.h"
#include "MaterialVarInfo.h"
#include "LinkingPoint.h"
#include <qgraphicsview.h>
#include "Enums.h"

class QGraphicsView;
class QGraphicsPathItem;

namespace ASL
{
	struct MatVarSaveData
	{
		int32_t MatVarID;
		ShaderType shader_type;
		MatVarSaveData(int ID, ShaderType type) :MatVarID(ID), shader_type(type){}
	};

	interface ILink
	{
		virtual QGraphicsPathItem* GetPathItem() const = 0;
        virtual void Update() = 0;
        virtual ~ILink(){};
    };

	template <class LinkingClass1, class LinkingClass2, typename SaveData>
	class AbstractLink : public ILink
	{
	protected:
		QGraphicsView* m_view;
		LinkingClass1* m_var;
		LinkingClass2* m_item;
		QGraphicsPathItem* m_pathItem;
		int m_end_slot;
	public:
		std::pair<LinkingClass1*, LinkingClass2*> GetLinkingObjects()const
		{
			return std::pair<LinkingClass1*, LinkingClass2*>(m_var, m_item);
		}
		AbstractLink(QGraphicsView* view, LinkingClass1* var, LinkingClass2* item) : m_item(item), m_view(view), m_var(var)
		{
			QPointF p1(view->mapToScene(view->mapFromGlobal(m_var->Point().mapToGlobal(QRect(QPoint(0, 0), m_var->Point().size()).center()))));
			m_end_slot = m_item->addLink(reinterpret_cast< Link<LinkingClass1, LinkingClass2, SaveData> *>(this));
			QPointF p2;
			if(std::is_same<LinkingClass1,MaterialVar>::value)
				p2 = m_item->slotPos(m_end_slot,LEFT);
			else if (std::is_same<LinkingClass1, ShaderSettingsElement>::value)
				p2 = m_item->slotPos(m_end_slot, RIGHT);
			QPainterPath path(p1);
			path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
			if (m_item->hasFocus())
				m_pathItem = m_item->scene()->addPath(path, QPen(QColor(60, 150, 200), 2));
			else
				m_pathItem = m_item->scene()->addPath(path, QPen(QColor(150, 150, 150), 2));
			m_var->Point().addRef();
		}
		AbstractLink(QGraphicsView* view, std::pair<LinkingClass1*, LinkingClass2*>& req) :AbstractLink(view, req.first, req.second)
		{
			
		}
		QGraphicsPathItem* GetPathItem()const override
		{
			return m_pathItem;
		}

		virtual ~AbstractLink()
		{
			m_pathItem->scene()->removeItem(m_pathItem);
			delete m_pathItem;
			m_item->deleteLink(reinterpret_cast< Link<LinkingClass1, LinkingClass2, SaveData> *>(this));
			m_var->Point().deleteRef();
		}
	public slots:
		void Update()override
		{
			QPointF p1(m_view->mapToScene(m_view->mapFromGlobal(m_var->Point().mapToGlobal(QRect(QPoint(0, 0), m_var->Point().size()).center()))));
			QPointF p2;
			if (std::is_same<LinkingClass1, MaterialVar>::value)
				p2 = m_item->slotPos(m_end_slot, LEFT);
			else if (std::is_same<LinkingClass1, ShaderSettingsElement>::value)
				p2 = m_item->slotPos(m_end_slot, RIGHT);
			QPainterPath path(p1);
			path.cubicTo(QPointF(p2.x() - m_end_slot * 5, p1.y()), QPointF(p1.x() - m_end_slot * 5, p2.y()), p2);
			m_pathItem->setPath(path);
		}
	};

	template <class LinkingClass1, class LinkingClass2, typename SaveData = MatVarSaveData>
	class Link : public AbstractLink<LinkingClass1, LinkingClass2, SaveData>
	{
		typedef AbstractLink<LinkingClass1, LinkingClass2, SaveData> AbstractLinkInst;
	public:
		Link(QGraphicsView* view, LinkingClass1* obj1, LinkingClass2* obj2) :AbstractLinkInst::AbstractLink(view,obj1,obj2)
		{}
		Link(QGraphicsView* view, std::pair<LinkingClass1*, LinkingClass2*>& req) : AbstractLinkInst::AbstractLink(view,req)
		{}
		~Link(){}
	};

	template <> class Link<MaterialVar, ShaderLabGUIElement, MatVarSaveData> : public AbstractLink<MaterialVar, ShaderLabGUIElement, MatVarSaveData>
	{
	public:
		Link(QGraphicsView* view, MaterialVar* obj1, ShaderLabGUIElement* obj2) :AbstractLink(view, obj1, obj2)
		{}
		Link(QGraphicsView* view, std::pair<MaterialVar*, ShaderLabGUIElement*>& req) : AbstractLink(view, req)
		{}
		MatVarSaveData GetSaveData()const
		{
			return MatVarSaveData(m_var->VarInfo().ID, m_item->get_shader_type());
		}
		~Link(){}
	};

	template <> class Link<ShaderSettingsElement, ShaderLabGUIElement, MatVarSaveData> : public AbstractLink<ShaderSettingsElement, ShaderLabGUIElement, MatVarSaveData>
	{
	public:
		Link(QGraphicsView* view, ShaderSettingsElement* obj1, ShaderLabGUIElement* obj2) :AbstractLink(view, obj1, obj2)
		{}
		Link(QGraphicsView* view, std::pair<ShaderSettingsElement*, ShaderLabGUIElement*>& req) : AbstractLink(view, req)
		{}
		~Link(){}
		/*MatVarSaveData GetSaveData()const
		{
			return MatVarSaveData(m_var->VarInfo().ID, m_item->getShader_Type());
		}*/
	};
}
#endif // LINK_H
