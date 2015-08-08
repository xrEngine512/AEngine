#ifndef SHADERLABGUIELEMENT_H
#define SHADERLABGUIELEMENT_H

#include <QGraphicsObject>
#include "Enums.h"
#include "ShaderBuffer.h"
#include <QtWidgets/QTextEdit>

namespace ASL
{
	struct ViewShaderPartInfo;
	__interface ILink;
	class ShaderPartSettings;
	class ShaderSettingsElement;
	class MaterialVar;
	class CodeEditor;
	struct MatVarSaveData;
	template<class t, class t2, class t3> class Link;
	__interface ICodeEditor;
	enum Side{LEFT,RIGHT,UP,DOWN};
	class ShaderLabGUIElement : public QGraphicsObject
	{
		typedef QGraphicsObject BaseClass;
		Q_OBJECT
		signals :
		void Clicked(ShaderLabGUIElement*);
	public:
		typedef Link<MaterialVar, ShaderLabGUIElement, MatVarSaveData> BufferLink;
		typedef Link<ShaderSettingsElement, ShaderLabGUIElement, MatVarSaveData> SettingsLink;
		ShaderLabGUIElement(QGraphicsItem *parent = nullptr);
		~ShaderLabGUIElement();
		QRectF boundingRect() const override;

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
			QWidget *widget)override;
		void setShader_Type(Shader_Type);
		
		void Init(const ViewShaderPartInfo& info);
		Shader_Type getShader_Type()const;
		
		const QString& EntryPoint();
		void ShrinkCodeEditor();
		void InitializeComponents();
		ICodeEditor* codeEditor()const;
		int addLink(BufferLink*);
		int addLink(SettingsLink*);
		void deleteLink(const BufferLink*);
		void deleteLink(const SettingsLink*);
		QPointF slotPos(int slot, Side)const;
		void setZValue(qreal z);
		bool hasFocus()const;
		const std::vector<BufferLink*>& BufferLinks() const;
		const std::vector<SettingsLink*>& SettingsLinks() const;
		const std::vector<ILink*>& Links() const;
		QVector<RuntimeBufferInfo> BuffersInfo() const;
		QVector<int> ParamIDs()const;
		QVector<int> TextureSlots()const;
	public slots:
		void UpdateGeneratedCode();
	private:
		int m_width, m_height;
		Shader_Type m_type;
		CodeEditor* m_txtedit;
		ShaderPartSettings* m_Settings;
		QPixmap *m_icon;
		std::vector<BufferLink*> m_bufLinks;
		std::vector<SettingsLink*> m_setLinks;
		std::vector<ILink*> m_allLinks;
		std::vector<ShaderSettingsElement*> m_Textures;
		ShaderBuffer* m_buffers[3];

		qreal m_xPrev, m_yPrev, m_xOld, m_yOld;
		bool m_dragging, m_hasFocus;
		int m_lastBufferCodeSize;

		QString toStr(Shader_Type);
		void mousePressEvent(QGraphicsSceneMouseEvent*)override;
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent*)override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent*)override;

	};
}
#endif // SHADERLABGUIELEMENT_H
