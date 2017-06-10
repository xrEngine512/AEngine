#include "ShaderEditor.h"
#include "ApproxGUIMenuButton.h"
#include "Enums.h"
#include "ShaderLabGUIElement.h"
#include <qpropertyanimation.h>
#include "wnd.h"
#include "shaderlabgui.h"
#include <qtimer.h>
#include <ApproxSizeGrip.h>
#include <QMenu>
#include "ApproxGUIMenuBar.h"
#include <qstatusbar.h>
#include "MaterialVarsContainer.h"
#include "ApproxGuiResManager.h"
#include "IControllerSession.h"
#include "MaterialVar.h"
#include <qmessagebox.h>
#include <ApproxSystemErrors.h>
#include "Literals.h"
#include <qfiledialog.h>
#include "CodeEditor.h"
#include "HLSLHighlighter.h"
#include "Link.h"
#include "ShaderSettings.h"
#include <qwindow.h>
#include <assert.h>

#define DEF_MENU_ITEM(menu,str,handler) connect(menu->addAction(str), SIGNAL(triggered()), SLOT(handler()))

#define MSG_BOX_LITERAL(Message, Icon)	QMessageBox msgBox(this); \
										msgBox.setIcon(Icon);\
										msgBox.setText(Message);\
										msgBox.exec()

#define MSG_BOX(Message,Icon)			QMessageBox msgBox(this); \
										msgBox.setIcon(Icon);\
										msgBox.setText(QString::fromStdString(Message));\
										msgBox.exec()

using namespace ApproxGUI;
namespace ASL
{
	ShaderEditor::ShaderEditor(IControllerSession* sessionInterface, QWidget *parent) : m_sessionInterface(sessionInterface),
		BaseClass(parent), m_gView(this), m_gScene(this), m_wndSettings(nullptr), m_dragging(false), m_btnSave(nullptr)
		, m_shrinkAnim(nullptr), m_sceneVars(nullptr)
	{
		setObjectName("ViewWnd");
		memset(m_SP, 0, sizeof(void*) * 6);
		
		resize(800, 600);
		
		m_gView.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		m_gView.update();
		info.m_SessionID = m_sessionInterface->OpenSession();
		
		info.m_ShaderParts.reserve(6);

		m_wndSettings = new wnd(this);
		m_opacEffect = new QGraphicsOpacityEffect(this);
		m_opacEffect->setOpacity(1);
		setGraphicsEffect(m_opacEffect);
		m_fadeAnim = new QPropertyAnimation(m_opacEffect, "opacity", this);
		m_sizeGrip = new SizeGrip(this);

        for (auto processor : sessionInterface->get_available_shader_processors()) {
			m_wndSettings->ui.shader_subsystem->addItem(QString::fromStdString(processor->describe().subsystem_name));
        }

		m_btnCompile = new ApproxGUIMenuButton;
		m_btnSettings = new ApproxGUIMenuButton;
		m_btnSave = new ApproxGUIMenuButton;
		m_btnLoad = new ApproxGUIMenuButton;
		m_lblDesc = new QLabel(this);
		m_timer = new QTimer(this);
		m_Menu = new ApproxGUIMenuBar(this);
		m_shaderSettings = new ShaderSettings(this);

		m_statusBar = new QStatusBar(this);
		m_lblStatus = new QLabel(QStringLiteral("Тестовое сообщение"));
		m_lblRendererStatus = new QLabel(this);
		m_lblRendererStatusIcon = new QLabel(this);

		connect(m_btnCompile, SIGNAL(clicked()), SLOT(on_Compile()));
		connect(m_btnSettings, SIGNAL(clicked()), SLOT(on_Settings()));
		connect(m_btnLoad, SIGNAL(clicked()), SLOT(on_Load()));
		connect(m_timer, SIGNAL(timeout()), SLOT(on_Frame()));
		connect(&m_gView, SIGNAL(createShaderElem(Shader_Type)), SLOT(GetOrCreateShaderElem(Shader_Type)));
		connect(&m_gView, SIGNAL(sceneMoved()), SLOT(UpdateLinks()));
		connect(&m_gView, SIGNAL(viewChanged()), SLOT(UpdateLinks()));
		connect(m_shaderSettings, &ShaderSettings::linkAttempt, this, &ShaderEditor::On_LinkAttemptSettings);
		connect(m_wndSettings->ui.shader_subsystem, SIGNAL(currentIndexChanged(const QString&)), SLOT(On_SubsystemChange(const QString&)));

		m_sizeGrip->snapTo(BOTTOM_RIGHT);
		m_sizeGrip->raise();

		m_fadeAnim->setDuration(500);
		m_fadeAnim->setStartValue(windowOpacity());
		m_fadeAnim->setEndValue(0.4);

		m_wndSettings->hide();
		m_wndSettings->move(0, 25);

		m_statusBar->setGeometry(2, height() - 23, width() - 4, 21);
		m_statusBar->addWidget(m_lblStatus);
		m_statusBar->setSizeGripEnabled(false);

		m_Menu->setGeometry(QRect(2, 2, 500, 25));
		m_Menu->addWidget(m_btnCompile);
		m_Menu->addWidget(m_btnSettings);
		m_Menu->addWidget(m_btnSave);
		m_Menu->addWidget(m_btnLoad);
		if (parent)
			m_Menu->setPalette(parent->palette());

		m_btnCompile->setText(CTRL_BTN_COMPILE_QT);
		m_btnSettings->setText(CTRL_BTN_SETTINGS_QT);
		m_btnSave->setText(CTRL_BTN_SAVE_QT);
		m_btnLoad->setText(CTRL_BTN_LOAD_QT);

		m_btnSettings->move(0, 0);
		m_btnCompile->move(m_btnSettings->width() - 9, 0);
		m_btnSave->move(m_btnCompile->x() + m_btnCompile->width() - 9, 0);
		m_btnLoad->move(m_btnCompile->x() + m_btnCompile->width() + m_btnSave->width() - 9, 0);

		auto menu = new QMenu(m_btnSave);

		DEF_MENU_ITEM(menu, SUB_CTRL_SAVE_PROJECT_QT, on_SaveProject);
		DEF_MENU_ITEM(menu, SUB_CTRL_SAVE_SHADER_QT, on_SaveShader);

		menu->actions()[1]->setEnabled(false);

		m_btnSave->setMenu(menu);

		m_lblDesc->move(2, 2);
		m_lblDesc->setFont(QFont("Verdana", 15));
		m_lblDesc->setText("Test");
		m_lblDesc->setAlignment(Qt::AlignRight);

		m_timer->start(33);

		m_gScene.setSceneRect(0, 0, 100, 100);

		m_gView.setGeometry(0, 32, width(), height() - 53);
		m_gView.setScene(&m_gScene);
		m_gView.setStyleSheet("QGraphicsView{border : 2px solid;}");

		m_shaderSettings->move(m_gView.width() - m_shaderSettings->width(), 100);

		m_lblRendererStatusIcon->setGeometry(500, -8, 50, 50);
		m_lblRendererStatusIcon->setStyleSheet("background-color:rgba(0,0,0,0)");

		m_lblRendererStatus->move(550, 12);

		setStyleSheet("#ViewWnd{ border : 2px solid;}");

		RendererOnline(false);
		On_SubsystemChange(m_wndSettings->ui.shader_subsystem->currentText());
	}

	void ShaderEditor::UpdateLinks()
	{
		for (auto part : m_SP)
		{
			if (part)
			for (auto link : part->Links())
			{
				link->Update();
			}
		}
	}
	void ShaderEditor::handleLinkAttempts(MaterialVar* sender, const QPoint& mouseGlobalPos)
	{
		QPointF p2(m_gView.mapToScene(m_gView.mapFromGlobal(mouseGlobalPos)));
		for (auto elem : m_SP)
		{
			if (elem)
			{
				if (elem->sceneBoundingRect().contains(p2))
				{
					std::pair<MaterialVar*, ShaderLabGUIElement*> req(sender, elem);

					for (auto link : elem->BufferLinks())
					{
						if (link->GetLinkingObjects() == req)
						{
							delete link;
							return;
						}
					}
					new Link<MaterialVar, ShaderLabGUIElement>(&m_gView, req);
					return;
				}
			}
		}
	}
	void ShaderEditor::handleLinkAttempts(ShaderSettingsElement* sender, const QPoint& mouseGlobalPos)
	{
		QPointF p2(m_gView.mapToScene(m_gView.mapFromGlobal(mouseGlobalPos)));
		for (auto elem : m_SP)
		{
			if (elem)
			{
				if (elem->sceneBoundingRect().contains(p2))
				{
					std::pair<ShaderSettingsElement*, ShaderLabGUIElement*> req(sender, elem);
					for (auto link : elem->SettingsLinks())
					{
						if (link->GetLinkingObjects() == req)
						{
							delete link;
							return;
						}
					}

					new Link<ShaderSettingsElement, ShaderLabGUIElement>(&m_gView, req);
					return;
				}
			}
		}
	}
	void ShaderEditor::On_LinkAttemptSettings(ShaderSettingsElement* sender, const QPoint& mouseGlobalPos)
	{
		handleLinkAttempts(sender, mouseGlobalPos);
	}

	void ShaderEditor::On_LinkAttemptMatVars(MaterialVar* sender, const QPoint& mouseGlobalPos)
	{
		handleLinkAttempts(sender, mouseGlobalPos);
	}

	void ShaderEditor::Raise(ShaderLabGUIElement* elem)
	{
		elem->setZValue(10);
		for (auto element : m_SP)
		{
			if (element)
			{
				if (element != elem)
				{
					element->setZValue(8);
				}
			}
		}
	}

	ShaderEditor::~ShaderEditor()
	{
		ClearElements();
	}

	void ShaderEditor::ClearElements()
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_SP[i];
			m_SP[i] = nullptr;
		}
		m_shaderSettings->ClearElements();
	}

	ShaderLabGUIElement* ShaderEditor::GetOrCreateShaderElem(Shader_Type type)
	{
		switch (type)
		{
		case VS:
			return setSP(0, type);
		case PS:
			return setSP(1, type);
		case GS:
			return setSP(2, type);
		case CS:
			return setSP(3, type);
		case DS:
			return setSP(4, type);
		case HS:
			return setSP(5, type);
		default:
			return nullptr;
		}
	}

	void ShaderEditor::SetMaterialVariables(std::vector<MaterialVarInfo>& info, const QString& ver)
	{
		m_sceneVars = new MaterialVarsContainer(&m_gView, info, ver);

		connect(m_sceneVars, &MaterialVarsContainer::linkAttempt, this, &ShaderEditor::On_LinkAttemptMatVars);

		m_sceneVars->move(2, 50);
		if (!isHidden())
			m_sceneVars->show();
	}

	void ShaderEditor::SetApproxVars()
	{
		if (m_sceneVars)
		{
			std::vector<QString> approxVars;
			for (auto elem : m_sceneVars->Vars())
			{
				approxVars.push_back(elem->VarInfo().name);
			}
			for (auto elem : m_SP)
			{
				if (elem)
					elem->codeEditor()->SyntaxHilighter()->SetApproxVars(approxVars);
			}
		}
	}

	void ShaderEditor::SetApproxVars(ShaderLabGUIElement* elem)
	{
		if (m_sceneVars)
		{
			std::vector<QString> approxVars;
			for (auto element : m_sceneVars->Vars())
			{
				approxVars.push_back(element->VarInfo().name);
			}
			elem->codeEditor()->SyntaxHilighter()->SetApproxVars(approxVars);
		}
	}

	void ShaderEditor::RendererOnline(bool online)
	{
		if (online)
		{
			m_lblRendererStatusIcon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/Icon_Eye")->scaled(m_lblRendererStatusIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			m_lblRendererStatus->setText("RendererVulkan online");
		}
		else
		{
			m_lblRendererStatusIcon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/Icon_Eye_Closed")->scaled(m_lblRendererStatusIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
			m_lblRendererStatus->setText("RendererVulkan offline");
		}
	}

	void ShaderEditor::on_Compile()
	{
		info.m_shaderName = m_wndSettings->ui.txtShaderName->text();
		info.shader_subsystem = m_wndSettings->ui.shader_subsystem->currentText();
		info.shader_language = m_wndSettings->ui.shader_language->currentText();
		info.m_ShaderParts.clear();
		for (int i = 0; i < 6; i++)
		{
			if (m_SP[i])
			{
				ViewShaderPartInfo part_info;
				if (m_SP[i]->EntryPoint().size())
					part_info.entryPoint = m_SP[i]->EntryPoint();
				else
					part_info.entryPoint = QString("main");
				part_info.qStr_code = m_SP[i]->codeEditor()->getCode();
				part_info.Shader_Type = m_SP[i]->getShader_Type();
				part_info.buffersInfo = m_SP[i]->BuffersInfo();
				part_info.paramIDs = m_SP[i]->ParamIDs();
				part_info.textureSlots = m_SP[i]->TextureSlots();
				info.m_ShaderParts.push_back(part_info);
			}
		}

        try {
            m_sessionInterface->compile(info);
            m_lblStatus->setText(COMPILATION_SUCCESS_QT);
            m_lblStatus->setStyleSheet("QLabel{color : rgb(10,240,10);}");
            m_btnSave->menu()->actions()[1]->setEnabled(true); // TODO: rely on session's state instead
        }
        catch (approx_exception& ex) {
            m_lblStatus->setText(ex.what());
            m_lblStatus->setStyleSheet("QLabel{color : rgb(240,10,10);}");
            m_btnSave->menu()->actions()[1]->setEnabled(false); // TODO: rely on session's state instead
        }
	}

	void ShaderEditor::on_Settings()
	{
		if (m_shrinkAnim && m_shrinkAnim->state() != QAbstractAnimation::Stopped)
		{
			return;
		}
		if (m_shrinkAnim)
		{
			delete m_shrinkAnim;
		}
		if (m_wndSettings->isHidden())
		{
			m_shrinkAnim = new QPropertyAnimation(&m_gView, "geometry", this);
			connect(m_shrinkAnim, SIGNAL(finished()), m_wndSettings, SLOT(show()));
			m_shrinkAnim->setStartValue(m_gView.geometry());
			m_shrinkAnim->setEndValue(QRect(m_wndSettings->width(), m_gView.y(), m_gView.width() - m_wndSettings->width(), m_gView.height()));
			m_shrinkAnim->setDuration(300);
			m_shrinkAnim->setEasingCurve(QEasingCurve::OutCubic);
			m_shrinkAnim->start();
		}
		else
		{
			m_lblDesc->setText(m_wndSettings->ui.txtShaderName->text());
			m_wndSettings->hide();
			m_shrinkAnim = new QPropertyAnimation(&m_gView, "geometry", this);
			m_shrinkAnim->setStartValue(m_gView.geometry());
			m_shrinkAnim->setEndValue(QRect(0, m_gView.y(), m_gView.width() + m_wndSettings->width(), m_gView.height()));
			m_shrinkAnim->setDuration(300);
			m_shrinkAnim->setEasingCurve(QEasingCurve::OutCubic);
			m_shrinkAnim->start();
		}
	}

	void ShaderEditor::on_SaveShader()
	{
		if (m_wndSettings->ui.txtWorkLib->text().size() == 0)
		{
			MSG_BOX_LITERAL(SPECIFY_WORK_DIR_QT, QMessageBox::Information);
		}
		else
		{
			info.m_shaderFilename = m_wndSettings->ui.txtWorkLib->text() + m_wndSettings->ui.txtShaderName->text();
			try {
                m_sessionInterface->SaveShader(info);
            }
            catch (approx_exception& e) {
                e();
            }
		}
	}

	inline Shader_Model toSM(int index)
	{
		switch (index)
		{
		case 0: return V5_0;
		case 1: return V4_1;
		case 2: return V4_0;
		default:return SM_NONE;
		}
	}

	inline int fromSM(Shader_Model model)
	{
		switch (model)
		{
		case V5_0: return 0;
		case V4_1: return 1;
		case V4_0: return 2;
		default:return 0;
		}
	}

	void ShaderEditor::on_SaveProject()
	{
		if (m_wndSettings->ui.txtWorkLib->text().size() == 0)
		{
			MSG_BOX_LITERAL(SPECIFY_WORK_DIR_QT, QMessageBox::Information);
		}
		else
		{
			info.m_ShaderParts.clear();
			for (int i = 0; i < 6; i++)
			{
				if (m_SP[i])
				{
					ViewShaderPartInfo part_info;
					part_info.entryPoint = m_SP[i]->EntryPoint();
					part_info.qStr_code = m_SP[i]->codeEditor()->getCode();
					part_info.Shader_Type = m_SP[i]->getShader_Type();
					part_info.buffersInfo = m_SP[i]->BuffersInfo();
					part_info.paramIDs = m_SP[i]->ParamIDs();
					part_info.textureSlots = m_SP[i]->TextureSlots();
					info.m_ShaderParts.push_back(part_info);
				}
			}
			info.m_shaderName = m_wndSettings->ui.txtShaderName->text();
			info.shader_language = m_wndSettings->ui.shader_language->currentText();
			info.m_projectFilename = m_wndSettings->ui.txtWorkLib->text() + m_wndSettings->ui.txtShaderName->text();
			info.m_Params = m_shaderSettings->Parameters();
			info.m_Textures = m_shaderSettings->Textures();
			try {
				m_sessionInterface->SaveProject(info);
			}
			catch (approx_exception& exc)
			{
				MSG_BOX(exc.Message(), QMessageBox::Warning);
			}
		}
	}

	void ShaderEditor::on_Load()
	{
		info.m_projectFilename = QFileDialog::getOpenFileName(this, tr("Open AbstractShader project file"), "../", tr("Approx AbstractShader Project (*.asp)"));
		if (info.m_projectFilename.size())
		{
			try
			{
				m_sessionInterface->LoadProject(info);
			}
			catch (approx_exception& exc)
			{
				approx_exception e("Не удалось открыть файл.");
				e += exc;
				e();
				return;
			}
			m_lblDesc->setText(info.m_shaderName);
			m_wndSettings->ui.txtShaderName->setText(info.m_shaderName);

			ClearElements();		

			m_shaderSettings->SetParameters(info.m_Params);
			m_shaderSettings->SetTextures(info.m_Textures);

			if (m_sceneVars)
			{
				for (auto part : info.m_ShaderParts)
				{
					for(auto info : part.buffersInfo)
					{
						for (auto var : m_sceneVars->Vars())
						{
							for (auto ID : info.IDs)
							{
								if (ID == var->VarInfo().ID)
									new Link<MaterialVar, ShaderLabGUIElement>(&m_gView, var, GetOrCreateShaderElem(part.Shader_Type));
							}
						}
					}
				}
			}
			for (auto part : info.m_ShaderParts)
			{
				for (auto set : m_shaderSettings->AllSettings())
				{
					ShaderParamInfo* info = dynamic_cast<ShaderParamInfo*>(set->Info());
					if (info)
						for (auto ID : part.paramIDs)
						{
							if (info->ID == ID)
							{
								new Link<ShaderSettingsElement, ShaderLabGUIElement>(&m_gView, set, GetOrCreateShaderElem(part.Shader_Type));
							}

						}
					else
					{
						TextureInfo* texInfo = dynamic_cast<TextureInfo*>(set->Info());
						if (texInfo)
							for (auto Slot : part.textureSlots)
							{
								if (texInfo->Slot == Slot)
								{
									new Link<ShaderSettingsElement, ShaderLabGUIElement>(&m_gView, set, GetOrCreateShaderElem(part.Shader_Type));
								}
							}
					}
				}
			}
			for (auto part : info.m_ShaderParts)
			{
				auto element = GetOrCreateShaderElem(part.Shader_Type);
				element->Init(part);				
			}

		}
	}

	void ShaderEditor::on_Frame()
	{
		//m_gScene.update();
//		m_glWidget->qglClearColor(QColor(150, 150, 150));
//		m_glWidget->updateGL();
	}

	void ShaderEditor::setShaderName(QString name)
	{
		m_lblDesc->setText(name);
	}

	void ShaderEditor::FadeIn()
	{
		m_fadeAnim->stop();
		m_fadeAnim->setStartValue(windowOpacity());
		m_fadeAnim->setDirection(QAbstractAnimation::Direction::Forward);
		m_fadeAnim->start();
	}

	void ShaderEditor::FadeOut()
	{
		m_fadeAnim->stop();
		m_fadeAnim->setStartValue(windowOpacity());
		m_fadeAnim->setDirection(QAbstractAnimation::Direction::Backward);
		m_fadeAnim->start();
	}

	ShaderLabGUIElement* ShaderEditor::setSP(int index, ASL::Shader_Type type)
	{
		if (!m_SP[index])
		{
			m_SP[index] = new ShaderLabGUIElement;
			m_SP[index]->moveBy(100 * index, 0);
			m_SP[index]->setShader_Type(type);
			m_gScene.addItem(m_SP[index]);
			m_SP[index]->InitializeComponents();
		}
		connect(m_SP[index], SIGNAL(Clicked(ShaderLabGUIElement*)), SLOT(Raise(ShaderLabGUIElement*)));
		return m_SP[index];
	}

	void ShaderEditor::resizeEvent(QResizeEvent*)
	{
		m_lblDesc->resize(width() - 4, m_lblDesc->height());
		m_gView.resize(width(), height() - 55);
		m_statusBar->setGeometry(2, height() - 23, width() - 4, 21);
		m_sizeGrip->snapTo(BOTTOM_RIGHT);
		m_shaderSettings->move(m_gView.width() - m_shaderSettings->width(), 100);
	}

	void ShaderEditor::moveEvent(QMoveEvent* e)
	{
		BaseClass::moveEvent(e);
	}

	bool inRange(const int val, const int min, const int max)
	{
		return val < max && val > min;
	}

	void ShaderEditor::mouseMoveEvent(QMouseEvent* event)
	{
		if (m_dragging)
		{
			QPoint newPos = pos();;
			QPoint mousePos = event->pos();
			
			if (parentWidget())
			{
				QPoint mappedPos = mapToParent(mousePos);
				QSize parentSize = parentWidget()->size();

				if (inRange(mappedPos.x(), 0, parentSize.width()))
				{
					newPos.setX(mousePos.x() - m_Prev.x() + x());
				}
				if (inRange(mappedPos.y(), 0, parentSize.height()))
				{
					newPos.setY(mousePos.y() - m_Prev.y() + y());
				}
			}
			else
			{
				newPos.setX(mousePos.x() - m_Prev.x() + x());
				newPos.setY(mousePos.y() - m_Prev.y() + y());
			}
			move(newPos);
			event->accept();
		}
		else
		{
			event->ignore();
		}
		BaseClass::mouseMoveEvent(event);
	}

	void ShaderEditor::mousePressEvent(QMouseEvent* event)
	{
		raise();
		if (event->buttons().testFlag(Qt::MidButton))
		{
			if (!m_dragging)
			{
				m_Prev = event->pos();
				setCursor(Qt::CursorShape::ClosedHandCursor);
			}
			m_dragging = true;
			event->accept();
		}
		else
		{
			BaseClass::mousePressEvent(event);
		}
	}

	void ShaderEditor::mouseReleaseEvent(QMouseEvent* event)
	{
		if (event->button() == Qt::MidButton)
		{
			m_dragging = false;
			unsetCursor();
		}
		else
		{
			BaseClass::mouseReleaseEvent(event);
		}
	}

	void ShaderEditor::On_SubsystemChange(const QString &name) {
		m_wndSettings->ui.shader_language->clear();
		for (auto processor : m_sessionInterface->get_available_shader_processors()) {
			auto description = processor->describe();
			if (QString::fromStdString(description.subsystem_name) == name) {
				for (auto shader_language : description.available_languages) {
					m_wndSettings->ui.shader_language->addItem(QString::fromStdString(shader_language));
				}
			}
		}
	}
}