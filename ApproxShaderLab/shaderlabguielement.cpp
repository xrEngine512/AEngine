#include "shaderlabguielement.h"
#include <qpainter.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsscene.h>
#include "CodeEditor.h"
#include "ApproxGuiResManager.h"
#include "ShaderPartSettings.h"
#include "Link.h"
#include "ISyntaxHilighter.h"
#include "MaterialVar.h"
#include "MaterialVarInfo.h"
#include "ShaderBuffer.h"
#include <ApproxMemory.h>

const int rad = 50;
const qreal slotSpacing = 5;
namespace ASL
{
	ShaderLabGUIElement::ShaderLabGUIElement(QGraphicsItem *parent)
		: BaseClass(parent), m_txtedit(nullptr), m_Settings(nullptr), 
		m_width(100), m_height(70), m_type(ST_NONE), m_dragging(false), m_hasFocus(false)
	{
		m_icon = g_ResManager->GetPic(":/ShaderEditor/background.png");
		BaseClass::setZValue(9);
		ApproxCommon::Memory::ZeroMemStatic(m_buffers);
	}

	ShaderLabGUIElement::~ShaderLabGUIElement()
	{
		if (m_txtedit)
			delete m_txtedit;
		if (m_Settings)
			delete m_Settings;
		for (auto link : m_links)
		{
			delete link;
		}
	}

	QRectF ShaderLabGUIElement::boundingRect() const
	{
		return QRectF(0, 0, m_width, m_height);
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

	void ShaderLabGUIElement::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
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

	Shader_Type ShaderLabGUIElement::getShader_Type() const
	{
		return m_type;
	}

	int ShaderLabGUIElement::addLink(Link& lnk)
	{
		int prevSize = 0, changingSize;
		m_links.push_back(&lnk);
		ShaderBuffer** selectedBuffer;
		const MaterialVarInfo& var = lnk.GetLinkingObjects().first->VarInfo();
		QString bufferName;
		switch (var.group)
		{
		case SCENE_CONSTANT:
		{
			selectedBuffer = &m_buffers[0];
			bufferName = "SceneConstants";
			if (m_buffers[1])
				prevSize += m_buffers[1]->CodeSize();
			if (m_buffers[2])
				prevSize += m_buffers[2]->CodeSize();
			break;
		}
		case PER_FRAME:
		{
			selectedBuffer = &m_buffers[1];
			bufferName = "PerFrame";
			if (m_buffers[0])
				prevSize += m_buffers[0]->CodeSize();
			if (m_buffers[2])
				prevSize += m_buffers[2]->CodeSize();
			break;
		}
		case PER_OBJECT:
		{
			selectedBuffer = &m_buffers[2];
			bufferName = "PerObject";
			if (m_buffers[0])
				prevSize += m_buffers[0]->CodeSize();
			if (m_buffers[1])
				prevSize += m_buffers[1]->CodeSize();
			break;
		}
		default:
		{
			throw std::exception("Variable without group");
		}
		}
		if (!*selectedBuffer)
		{
			*selectedBuffer = new ShaderBuffer(this, bufferName);
		}
		else
		{
			changingSize = (*selectedBuffer)->CodeSize();
			prevSize += changingSize;
		}
		(*selectedBuffer)->AddVariable(var);

		m_txtedit->SyntaxHilighter()->AddApproxVar(lnk.GetLinkingObjects().first->VarInfo().name);
		int codeLen = 0;
		QString combinedCode;
		for (auto buf : m_buffers)
		{
			if (buf)
			{
				combinedCode += buf->ToCode();
			}
		}
		m_txtedit->setPlainText(combinedCode + m_txtedit->toPlainText().remove(0, prevSize));
		m_txtedit->SetReadOnlyArea(0, prevSize - changingSize + (*selectedBuffer)->CodeSize());
		return m_links.size() - 1;
	}

	QPointF ShaderLabGUIElement::slotPos(int slot) const
	{
		auto tl = sceneBoundingRect().topLeft();
		return QPointF(tl.x() + 10, tl.y() + slotSpacing * slot);
	}

	void ShaderLabGUIElement::ShrinkCodeEditor()
	{
		if (m_txtedit)
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
			m_txtedit = new CodeEditor(nullptr, this, scene()->views().at(0)->width() / 2, scene()->views().at(0)->height());
			m_txtedit->hide();
			m_txtedit->SetProxyWidget(scene()->addWidget(m_txtedit));
			connect(m_txtedit, &CodeEditor::focused, [&]()
			{
				m_hasFocus = true;
				for (auto link : m_links)
				{
					link->GetPathItem()->setPen(QPen(QColor(60, 150, 200), 2));
				}
				emit Clicked(this);
			});
			connect(m_txtedit, &CodeEditor::unFocused, [&]()
			{
				m_hasFocus = false;
				for (auto link : m_links)
				{
					link->GetPathItem()->setPen(QPen(QColor(150, 150, 150), 2));
				}
			});
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
		m_txtedit->setZValue(z);
		BaseClass::setZValue(z);
		for (auto link : m_links)
		{
			link->GetPathItem()->setZValue(z - 1);
		}
	}

	bool ShaderLabGUIElement::hasFocus() const
	{
		return m_hasFocus;
	}

	const std::vector<Link*>& ShaderLabGUIElement::Links() const
	{
		return m_links;
	}

	void ShaderLabGUIElement::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		event->accept();


		emit Clicked(this);

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

	void ShaderLabGUIElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e)
	{
		if (m_txtedit)
		{
			QPointF sPos = scenePos();
			QRect geom(sPos.x(), sPos.y(), m_width, m_height);

			m_txtedit->HideAnimated(geom);
		}
		BaseClass::mouseDoubleClickEvent(e);
	}

	void ShaderLabGUIElement::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
	{
		if (m_dragging)
		{
			setPos(event->scenePos().x() - m_xPrev, event->scenePos().y() - m_yPrev);
			if (m_txtedit && !m_txtedit->isHidden())
			{
				m_txtedit->move(scenePos().x() + m_width, scenePos().y());
			}
			m_Settings->move(scenePos().x() - (m_Settings->width() - m_width), scenePos().y()+m_height);
			for (auto link : m_links)
			{
				link->Update();
			}
		}
		BaseClass::mouseMoveEvent(event);
	}

	void ShaderLabGUIElement::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{
		if (event->button() == Qt::MidButton)
		{
			m_dragging = false;
			unsetCursor();
		}

		BaseClass::mouseReleaseEvent(event);
	}
}