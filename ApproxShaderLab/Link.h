#ifndef LINK_H
#define LINK_H
#include <QPainterPath>
#include "ShaderEditor.h"

class QGraphicsView;
class QGraphicsPathItem;

namespace ASL
{
	class ShaderLabGUIElement;
	class MaterialVar;


	class Link
	{
		QGraphicsView* m_view;
		MaterialVar* m_wgt;
		ShaderLabGUIElement* m_item;
		QGraphicsPathItem* m_pathItem;
		int m_end_slot;
	public:

		struct SaveData
		{
			__int32 MatVarID;
			Shader_Type ShaderType;
			SaveData(int ID, Shader_Type type) :MatVarID(ID), ShaderType(type){}
		};

		std::pair<MaterialVar*, ShaderLabGUIElement*> GetLinkingObjects()const;
		Link(QGraphicsView*, std::pair<MaterialVar*, ShaderLabGUIElement*>&);
		QGraphicsPathItem* GetPathItem()const;
		SaveData GetSaveData()const;
		~Link();
	public slots:
		void Update();
	private:

	};
}
#endif // LINK_H
