#include "shaderlabguielement.h"
#include <qpainter.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsscene.h>
#include "CodeEditor.h"
#include <qgraphicsview.h>
#include <QtCore/qpropertyanimation.h>
#include <QGraphicsSceneDragDropEvent>
#include "ApproxGuiResManager.h"
#include "ShaderPartSettings.h"
#include "Link.h"

const int rad = 50;
const qreal slotSpacing = 5;

ShaderLabGUIElement::ShaderLabGUIElement(QGraphicsItem *parent)
	: QGraphicsItem(parent), m_txtedit(nullptr),m_Settings(nullptr), m_width(100), m_height(70), m_type(ST_NONE), m_dragging(false)
{
    m_icon = g_ResManager->GetPic(":/ShaderEditor/background.png");
	setZValue(9);
}

ShaderLabGUIElement::~ShaderLabGUIElement()
{
	if (m_txtedit)
		delete m_txtedit;
	if (m_Settings)
		delete m_Settings;
}

QRectF ShaderLabGUIElement::boundingRect() const
{
    return QRectF(0, 0,m_width,m_height);
}

void ShaderLabGUIElement::setEntryPoint(QString& str)
{
	auto settings = m_Settings->getSettings();
	settings.EntryPoint = str;
	m_Settings->setSettings(settings);
}

const QString& ShaderLabGUIElement::EntryPoint()
{
	return m_Settings->getSettings().EntryPoint;
}

ICodeEditor* ShaderLabGUIElement::codeEditor()const
{
	return m_txtedit;
}

void ShaderLabGUIElement::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* )
{
    QPainterPath path;
    path.addRoundRect(0, 0, m_width, m_height, rad);
	QColor color;
	if (m_txtedit)
	{
		if (m_txtedit->hasFocus())
			color = QColor(60, 150, 200);
		else
			color = QColor(150, 150, 150);
	}
	QPen pen1(color, 2);
    painter->setPen(pen1);
    painter->drawPath(path);
	if (isUnderMouse())
		painter->fillPath(path, QBrush(QColor(80, 80, 80)));
	else
		painter->fillPath(path, QBrush(QColor(40, 40, 40)));
    if (m_type != SM_NONE)
    {
        painter->setFont(QFont("Verdana", 20));
        painter->drawText(m_width - 52, m_height - 7, toStr(m_type));
    }
    if (m_txtedit)
    {
        if (m_txtedit->toPlainText().size() > 0)
        {
            QRectF target(5.0, 5.0, 30.0, 30.0);
            QRectF source(0.0, 0.0, 100.0, 100.0);
            painter->drawPixmap(target, *m_icon, source);

        }
    }
}

void ShaderLabGUIElement::setShader_Type(Shader_Type type)
{
    m_type = type;
}

QString ShaderLabGUIElement::getCode() const
{
    return m_txtedit->toPlainText();
}

Shader_Type ShaderLabGUIElement::getShader_Type() const
{
    return m_type;
}

int ShaderLabGUIElement::addLink(Link& lnk)
{	
	m_links.push_back(&lnk);
	return m_links.size() - 1;
}

const QPointF ShaderLabGUIElement::slotPos(int slot) const
{
	auto tl = sceneBoundingRect().topLeft();
	return QPointF(tl.x() + 10, tl.y() + slotSpacing * slot);
}

void ShaderLabGUIElement::ShrinkCodeEditor()
{
    if(m_txtedit)
    {
        m_txtedit->Shrink();
    }
}

QString ShaderLabGUIElement::toStr(Shader_Type type)
{
    switch (type)
    {
    case VS: return ".VS";
    case PS: return ".PS";
    case GS: return ".GS";
    case CS: return ".CS";
    case DS: return ".DS";
    case HS: return ".HS";
    default: return "";
    }
}

void ShaderLabGUIElement::InitializeComponents()
{
	if (!m_txtedit)
	{
		m_txtedit = new CodeEditor(nullptr,this, scene()->views().at(0)->width() / 2, scene()->views().at(0)->height());
		m_txtedit->hide();
		m_txtedit->SetProxyWidget(scene()->addWidget(m_txtedit));
	}
	if (!m_Settings)
	{
		m_Settings = new ShaderPartSettings;
		m_Settings->hide();
		m_Settings->SetProxyWidget(scene()->addWidget(m_Settings));
	}
}

void ShaderLabGUIElement::setZValue(qreal z)
{
	QGraphicsItem::setZValue(z);
	for (auto link : m_links)
	{
		link->GetPathItem()->setZValue(z-1);
	}
}

void ShaderLabGUIElement::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	event->accept();
	
	m_txtedit->setZValue(10);
	
    if (event->buttons().testFlag(Qt::MidButton))
    {
        if (!m_dragging)
        {
			m_xPrev = event->pos().x();
			m_yPrev = event->pos().y();
            setCursor(Qt::CursorShape::OpenHandCursor);
        }
        m_dragging = true;
		
    }

	QPointF sPos = scenePos();
	QRect geom(sPos.x(), sPos.y(), m_width, m_height);

    if (event->buttons().testFlag(Qt::LeftButton))
    {
		m_txtedit->ShowAnimated(geom);  
    }
	if (event->buttons().testFlag(Qt::RightButton))
	{
		if (m_Settings->isHidden())
			m_Settings->ShowAnimated(geom);
		else
			m_Settings->HideAnimated(geom);
	}
}

void ShaderLabGUIElement::setCode(QString& code)
{
	m_txtedit->setPlainText(code);
}

void ShaderLabGUIElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e)
{  
	if (m_txtedit)
	{
		QPointF sPos = scenePos();
		QRect geom(sPos.x(), sPos.y(), m_width, m_height);

		m_txtedit->HideAnimated(geom);
	}
	QGraphicsItem::mouseDoubleClickEvent(e);
}

void ShaderLabGUIElement::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_dragging)
    {
		setPos(event->scenePos().x() - m_xPrev, event->scenePos().y() - m_yPrev);
		if (m_txtedit && !m_txtedit->isHidden())
		{
			m_txtedit->move(scenePos().x()+m_width,scenePos().y());			
		}
		m_Settings->move(scenePos().x() - m_Settings->width(), scenePos().y());
		for (auto link : m_links)
		{
			link->Update();
		}
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ShaderLabGUIElement::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::MidButton)
    {
        m_dragging = false;
        unsetCursor();
    }
   
	QGraphicsItem::mouseReleaseEvent(event);
}