#include "ShaderLabGUIElement.h"
#include <qpainter.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsscene.h>
#include "CodeEditor.h"
#include "ApproxGuiResManager.h"
#include "ShaderPartSettings.h"
#include "Link.h"
#include "ISyntaxHighlighter.h"
#include "MaterialVar.h"
#include "MaterialVarInfo.h"
#include <ApproxMemory.h>
#include "LinkingPoint.h"
#include "ShaderParamInfo.h"
#include "TextureInfo.h"
#include "ViewSessionInfo.h"

const int rad = 50;
const qreal slotSpacing = 5;
namespace ASL
{
	ShaderLabGUIElement::ShaderLabGUIElement(QGraphicsItem *parent)
		: BaseClass(parent), m_txtedit(nullptr), m_Settings(nullptr), 
		m_width(100), m_height(70), m_type(ST_NONE), m_dragging(false), m_hasFocus(false), m_lastBufferCodeSize(0)
	{
		m_icon = g_ResManager->GetPic(":/ShaderEditor/background.png");
		BaseClass::setZValue(9);
		ApproxCommon::Memory::ZeroMemStatic(m_buffers);
	}

	ShaderLabGUIElement::~ShaderLabGUIElement()
	{
		{auto tmp = m_bufLinks;
		for (auto link : tmp)
		{
			delete link;
		}}
		{auto tmp = m_setLinks;
		for (auto link : tmp)
		{
			delete link;
		}}


		if (m_txtedit)
			delete m_txtedit;
		if (m_Settings)
			delete m_Settings;
	}

	QRectF ShaderLabGUIElement::boundingRect() const
	{
		return QRectF(0, 0, m_width, m_height);
	}

	void ShaderLabGUIElement::setShader_Type(Shader_Type t)
	{
		m_type = t;
	}

	const QString& ShaderLabGUIElement::EntryPoint()
	{
		return m_Settings->getSettings().EntryPoint;
	}

	ICodeEditor* ShaderLabGUIElement::codeEditor()const
	{
		return m_txtedit;
	}

	int ShaderLabGUIElement::addLink(SettingsLink* lnk)
	{
		auto Info = lnk->GetLinkingObjects().first->Info();
		
		if (auto shaderParam = dynamic_cast<ShaderParamInfo*>(Info))
		{
			auto& selectedBuffer = m_buffers[2];
			if (!selectedBuffer)
			{
				selectedBuffer = new ShaderBuffer(this, PER_OBJECT);
			}
			
			selectedBuffer->AddVariable(*shaderParam);
			m_txtedit->SyntaxHilighter()->AddShaderParam(shaderParam->Name.c_str());
			UpdateGeneratedCode();
		}
		else if (dynamic_cast<TextureInfo*>(Info))
		{
			m_Textures.push_back(lnk->GetLinkingObjects().first);
			UpdateGeneratedCode();
		}
		
		m_setLinks.push_back(lnk);
		m_allLinks.push_back(lnk);
		return m_setLinks.size() - 1;
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
		if (m_type != ST_NONE)
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

	void ShaderLabGUIElement::Init(const ViewShaderPartInfo& info)
	{
		auto settings = m_Settings->getSettings();
		settings.EntryPoint = info.entryPoint;
		m_Settings->setSettings(settings);
		m_type = info.Shader_Type;
		m_txtedit->setPlainText(info.qStr_code);
		
	}

	Shader_Type ShaderLabGUIElement::getShader_Type() const
	{
		return m_type;
	}

	int ShaderLabGUIElement::addLink(BufferLink* lnk)
	{
		m_bufLinks.push_back(lnk);
		m_allLinks.push_back(lnk);
		ShaderBuffer** selectedBuffer;
		const MaterialVarInfo& var = lnk->GetLinkingObjects().first->VarInfo();
		switch (var.group)
		{
		case SCENE_CONSTANT:
		{
			selectedBuffer = &m_buffers[0];
			break;
		}
		case PER_FRAME:
		{
			selectedBuffer = &m_buffers[1];
			break;
		}
		case PER_OBJECT:
		{
			selectedBuffer = &m_buffers[2];
			break;
		}
		default:
		{
			throw std::runtime_error("Variable without group");
		}
		}
		if (!*selectedBuffer)
		{
			*selectedBuffer = new ShaderBuffer(this, var.group);
		}
		
		(*selectedBuffer)->AddVariable(var);

		m_txtedit->SyntaxHilighter()->AddApproxVar(lnk->GetLinkingObjects().first->VarInfo().name);
		UpdateGeneratedCode();
		return m_bufLinks.size() - 1;
	}

	void ShaderLabGUIElement::deleteLink(const BufferLink* lnk)
	{
		ShaderBuffer* selectedBuffer;
		const MaterialVarInfo& var = lnk->GetLinkingObjects().first->VarInfo();
		switch (var.group)
		{
		case SCENE_CONSTANT:
		{
			selectedBuffer = m_buffers[0];
			break;
		}
		case PER_FRAME:
		{
			selectedBuffer = m_buffers[1];
			break;
		}
		case PER_OBJECT:
		{
			selectedBuffer = m_buffers[2];			
			break;
		}
		default:
		{
			qDebug("Variable without group in deleteLink");
			return; //# 
		}
		}
		selectedBuffer->DeleteVariable(var);
		UpdateGeneratedCode();
		for (auto link = m_bufLinks.begin(); link != m_bufLinks.end(); ++link)
		{
			if (*link == lnk)
			{
				m_bufLinks.erase(link);
				break;
			}
		}
		for (auto link = m_allLinks.begin(); link != m_allLinks.end(); ++link)
		{
			if (*link == lnk)
			{
				m_allLinks.erase(link);
				break;
			}
		}
	}

	void ShaderLabGUIElement::deleteLink(const SettingsLink* lnk)
	{
		auto info = lnk->GetLinkingObjects().first->Info();
		if (auto shaderParamInfo = dynamic_cast<ShaderParamInfo*>(info))
		{
			if (m_buffers[2])
			{
				m_buffers[2]->DeleteVariable(*shaderParamInfo);
				UpdateGeneratedCode();
			}
		}
		else if (auto textureInfo = dynamic_cast<TextureInfo*>(info))
		{
			for (auto variable = m_Textures.begin(); variable != m_Textures.end(); ++variable)
			{
				if (auto textureInfoVariable = dynamic_cast<TextureInfo*>((*variable)->Info()))
					if (textureInfoVariable->Slot == textureInfo->Slot)
					{
						m_Textures.erase(variable);
						UpdateGeneratedCode();
						break;
					}
			}			
		}
		for (auto link = m_setLinks.begin(); link != m_setLinks.end(); ++link)
		{
			if (*link == lnk)
			{
				m_setLinks.erase(link);
				break;
			}
		}
		for (auto link = m_allLinks.begin(); link != m_allLinks.end(); ++link)
		{
			if (*link == lnk)
			{
				m_allLinks.erase(link);
				break;
			}
		}
	}

	QPointF ShaderLabGUIElement::slotPos(int slot, Side side) const
	{
		switch (side)
		{
		case LEFT:
		{
			auto tl = sceneBoundingRect().topLeft();
			return QPointF(tl.x() + 10, tl.y() + slotSpacing * slot);
		}
		case RIGHT:
		{
			auto tr = sceneBoundingRect().topRight();
			return QPointF(tr.x() - 10, tr.y() + slotSpacing * slot);
		}
		case UP:
		{
			auto tl = sceneBoundingRect().topLeft();
			return QPointF(tl.x() + slotSpacing * slot, tl.y() + 10);
		}
		case DOWN:
		{
			auto bl = sceneBoundingRect().bottomLeft();
			return QPointF(bl.x() + slotSpacing * slot, bl.y() - 10);
		}
		}
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
				for (auto link : m_bufLinks)
				{
					link->GetPathItem()->setPen(QPen(QColor(60, 150, 200), 2));
					link->GetLinkingObjects().first->Point().setInFocus();
				}
				for (auto link : m_setLinks)
				{
					link->GetPathItem()->setPen(QPen(QColor(60, 150, 200), 2));
					link->GetLinkingObjects().first->Point().setInFocus();
				}
				emit Clicked(this);
			});
			connect(m_txtedit, &CodeEditor::unFocused, [&]()
			{
				m_hasFocus = false;
				for (auto link : m_bufLinks)
				{
					link->GetPathItem()->setPen(QPen(QColor(150, 150, 150), 2));
					link->GetLinkingObjects().first->Point().setInUse();
				}
				for (auto link : m_setLinks)
				{
					link->GetPathItem()->setPen(QPen(QColor(150, 150, 150), 2));
					link->GetLinkingObjects().first->Point().setInUse();
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
		for (auto link : m_bufLinks)
		{
			link->GetPathItem()->setZValue(z - 1);
		}
	}

	bool ShaderLabGUIElement::hasFocus() const
	{
		return m_hasFocus;
	}

	const std::vector<ShaderLabGUIElement::BufferLink*>& ShaderLabGUIElement::BufferLinks() const
	{
		return m_bufLinks;
	}

	const std::vector<ShaderLabGUIElement::SettingsLink*>& ShaderLabGUIElement::SettingsLinks() const
	{
		return m_setLinks;
	}

	const std::vector<ILink*>& ShaderLabGUIElement::Links() const
	{
		return m_allLinks;
	}

	QVector<RuntimeBufferInfo> ShaderLabGUIElement::BuffersInfo() const
	{
		QVector<RuntimeBufferInfo> info;
		for (auto buf : m_buffers)
		{
			if (buf)
			{
				info.push_back(buf->Info());
			}
		}
		return info;
	}

	QVector<int> ShaderLabGUIElement::ParamIDs() const
	{
		QVector<int> res;
		if (m_buffers[2])
		{
			for (auto param : m_buffers[2]->Params())
			{
				res.push_back(param->ID);
			}
		}
		return res;
	}

	QVector<int> ShaderLabGUIElement::TextureSlots() const
	{
		QVector<int> res;
		for (auto texture : m_Textures)
		{
			res.push_back(dynamic_cast<TextureInfo*>(texture->Info())->Slot);
		}
		return res;
	}

	void ShaderLabGUIElement::UpdateGeneratedCode()
	{
		QString combinedCode;
		for (auto buf : m_buffers)
		{
			if (buf)
			{
				combinedCode += buf->ToCode();
			}
		}
		combinedCode += '\n';
		for (auto tex :m_Textures)
		{
			combinedCode += tex->ToCode();
		}
		if (m_Textures.size())
		{
			combinedCode += "SamplerState Sampler;\n";
		}
		m_txtedit->setPlainText(combinedCode + m_txtedit->toPlainText().remove(0, m_lastBufferCodeSize));
		m_txtedit->SetReadOnlyArea(0, combinedCode.size() - 1);
		m_lastBufferCodeSize = combinedCode.size();
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
			for (auto link : m_allLinks)
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