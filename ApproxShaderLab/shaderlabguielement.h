#ifndef SHADERLABGUIELEMENT_H
#define SHADERLABGUIELEMENT_H

#include <QGraphicsItem>
#include "Enums.h"
#include <QtWidgets/QTextEdit>

using namespace ASL;
class CodeEditor;
class ShaderPartSettings;
class Link;
__interface ICodeEditor;

class ShaderLabGUIElement :  public QGraphicsItem
{
   // Q_OBJECT
    

public:
    ShaderLabGUIElement(QGraphicsItem *parent = nullptr);
    ~ShaderLabGUIElement();
    QRectF boundingRect() const override;
   
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)override;
    void setShader_Type(Shader_Type);
    QString getCode()const;
	void setCode(QString&);
    Shader_Type getShader_Type()const;
	void setEntryPoint(QString&);
	const QString& EntryPoint();
    void ShrinkCodeEditor();
	void InitializeComponents();
	ICodeEditor* codeEditor()const;
	int addLink(Link&);
	const QPointF slotPos(int slot)const;
	void setZValue(qreal z);
private:
    int m_width , m_height;
    Shader_Type m_type;
    CodeEditor* m_txtedit;
	ShaderPartSettings* m_Settings;
    QPixmap *m_icon;
	std::vector<Link*> m_links;

	qreal m_xPrev, m_yPrev, m_xOld, m_yOld;
    bool m_dragging;
    
    QString toStr(Shader_Type);
    void mousePressEvent(QGraphicsSceneMouseEvent*)override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*)override;
    
};

#endif // SHADERLABGUIELEMENT_H
