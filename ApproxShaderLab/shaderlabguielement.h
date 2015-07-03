#ifndef SHADERLABGUIELEMENT_H
#define SHADERLABGUIELEMENT_H

#include <QGraphicsObject>
#include "Enums.h"
#include <QtWidgets/QTextEdit>

namespace ASL
{
	class CodeEditor;
	class ShaderBuffer;
	class ShaderPartSettings;
	class Link;
	__interface ICodeEditor;

	class ShaderLabGUIElement : public QGraphicsObject
	{
		typedef QGraphicsObject BaseClass;
		Q_OBJECT
		signals :
		void Clicked(ShaderLabGUIElement*);
	public:
		ShaderLabGUIElement(QGraphicsItem *parent = nullptr);
		~ShaderLabGUIElement();
		QRectF boundingRect() const override;

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			QWidget *widget)override;
		void setShader_Type(Shader_Type);
		
		Shader_Type getShader_Type()const;
		void setEntryPoint(QString&);
		const QString& EntryPoint();
		void ShrinkCodeEditor();
		void InitializeComponents();
		ICodeEditor* codeEditor()const;
		int addLink(Link&);
		QPointF slotPos(int slot)const;
		void setZValue(qreal z);
		bool hasFocus()const;
		const std::vector<Link*>& Links() const;
	private:
		int m_width, m_height;
		Shader_Type m_type;
		CodeEditor* m_txtedit;
		ShaderPartSettings* m_Settings;
		QPixmap *m_icon;
		std::vector<Link*> m_links;
		ShaderBuffer* m_buffers[3];

		qreal m_xPrev, m_yPrev, m_xOld, m_yOld;
		bool m_dragging, m_hasFocus;

		QString toStr(Shader_Type);
		void mousePressEvent(QGraphicsSceneMouseEvent*)override;
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent*)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent*)override;

	};
}
#endif // SHADERLABGUIELEMENT_H
