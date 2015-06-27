#ifndef LINK_H
#define LINK_H

#include <QPainterPath>

class ShaderLabGUIElement;
class QGraphicsView;
class QGraphicsPathItem;

class Link
{
	QPainterPath m_path;
	QGraphicsView* m_view;
	QWidget* m_wgt; 
	ShaderLabGUIElement* m_item;
	QGraphicsPathItem* m_pathItem;
	int m_end_slot;
public:
	std::pair<QWidget*, ShaderLabGUIElement*> GetLinkingObjects()const;
	Link(QGraphicsView*, std::pair<QWidget*, ShaderLabGUIElement*>&);
	const QPainterPath& Path()const;
	QGraphicsPathItem* GetPathItem()const;
	~Link();
public slots:
	void Update();
private:
	
};

#endif // LINK_H
