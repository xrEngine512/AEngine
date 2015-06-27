#include "ApproxShaderView.h"
#include "ApproxGUIMenuButton.h"
#include "Enums.h"
#include "shaderlabguielement.h"
#include <qpropertyanimation.h>
#include "wnd.h"
#include <QPropertyAnimation>
#include "shaderlabgui.h"
#include <qtimer.h>
#include <ApproxSizeGrip.h>
#include <QMenu>
#include "ApproxGUIMenuBar.h"
#include <qstatusbar.h>
#include "ApproxSceneVarsContainer.h"
#include "ApproxGuiResManager.h"
#include "IControllerSession.h"
#include <QGraphicsDropShadowEffect>
#include <qmessagebox.h>
#include <ApproxSystemErrors.h>
#include "Literals.h"
#include <qfiledialog.h>
#include "CodeEditor.h"
#include "HLSLHighlighter.h"
#include "Link.h"

#define DEF_MENU_ITEM(menu,str,handler) connect(menu->addAction(QStringLiteral(str)), SIGNAL(triggered()), SLOT(handler()))

#define MSG_BOX_LITERAL(Message, Icon)	QMessageBox msgBox(this); \
										msgBox.setIcon(Icon);\
										msgBox.setText(QStringLiteral(Message));\
										msgBox.exec()

#define MSG_BOX_W(Message,Icon)			QMessageBox msgBox(this); \
										msgBox.setIcon(Icon);\
										msgBox.setText(QString::fromWCharArray(Message));\
										msgBox.exec()

using namespace ApproxGUI;
using namespace ASL;
ApproxShaderView::ApproxShaderView(IControllerSession* sessionInterface, QWidget *parent) : m_sessionInterface(sessionInterface),
BaseClass(parent), m_gView(this), m_gScene(this), m_wndSettings(nullptr), m_dragging(false), m_btnSave(nullptr)
, m_shrinkAnim(nullptr), m_propertyInfo(nullptr)
{
    setObjectName("ViewWnd");
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    memset(m_SP, 0, sizeof(void*) * 6);
    QPalette pal;
    pal.setColor(QPalette::ColorRole::Text, QColor(150, 150, 150));
    setPalette(pal);
    resize(800, 600);
	
	info.m_SessionID = m_sessionInterface->OpenSession();

	info.m_ShaderParts.reserve(6);
	info.m_ShaderParts.fill(ApproxViewShaderPartInfo(),6);

	m_wndSettings = new wnd(this);
	m_opacEffect = new QGraphicsOpacityEffect(this);
	m_opacEffect->setOpacity(1);
	setGraphicsEffect(m_opacEffect);
	m_fadeAnim = new QPropertyAnimation(m_opacEffect, "opacity", this);
	m_sizeGrip = new SizeGrip(this);

	
    m_btnCompile = new ApproxGUIMenuButton;
    m_btnSettings = new ApproxGUIMenuButton;
    m_btnSave = new ApproxGUIMenuButton;
	m_btnLoad = new ApproxGUIMenuButton;
    m_lblDesc = new QLabel(this);
    m_timer = new QTimer(this);
    m_Menu = new ApproxGUIMenuBar(this);
	
    m_statusBar = new QStatusBar(this);
    m_lblStatus = new QLabel(QStringLiteral("Тестовое сообщение"));
    m_lblRendererStatus = new QLabel(this);;
    m_lblRendererStatusIcon = new QLabel(this);

    
    connect(m_btnCompile,	SIGNAL(clicked()), SLOT(on_Compile()));
    connect(m_btnSettings,	SIGNAL(clicked()), SLOT(on_Settings()));
	connect(m_btnLoad,		SIGNAL(clicked()), SLOT(on_Load()));
    connect(m_timer,		SIGNAL(timeout()), SLOT(on_Frame()));
	connect(&m_gView, SIGNAL(createShaderElem(ASL::Shader_Type)), SLOT(CreateShaderElem(ASL::Shader_Type)));
	connect(&m_gView, SIGNAL(sceneMoved()), SLOT(UpdateLinks()));
	connect(&m_gView, SIGNAL(viewChanged()), SLOT(UpdateLinks()));

	m_sizeGrip->snapTo(BOTTOM_RIGHT);
	m_sizeGrip->raise();

	m_fadeAnim->setDuration(500);
	m_fadeAnim->setStartValue(windowOpacity());
	m_fadeAnim->setEndValue(0.4);

	m_wndSettings->hide();
	m_wndSettings->move(0, 25);

    m_statusBar->setGeometry(2, height() - 23, width()-4, 21);
    m_statusBar->addWidget(m_lblStatus);
    m_statusBar->setSizeGripEnabled(false);

    m_Menu->setGeometry(QRect(2, 2, 500, 25));
    m_Menu->addWidget(m_btnCompile);
    m_Menu->addWidget(m_btnSettings);
    m_Menu->addWidget(m_btnSave);
	m_Menu->addWidget(m_btnLoad);
    if (parent)
        m_Menu->setPalette(parent->palette());

    m_btnSettings->move(0, 0);
    m_btnCompile->move(m_btnSettings->width() - 9, 0);
	m_btnSave->move(m_btnCompile->x() + m_btnCompile->width() - 9, 0);
	m_btnLoad->move(m_btnCompile->x() + m_btnCompile->width() + m_btnSave->width() - 9, 0);

	m_btnCompile->setText(QStringLiteral(CTRL_BTN_COMPILE_QT));
	m_btnSettings->setText(QStringLiteral(CTRL_BTN_SETTINGS_QT));
	m_btnSave->setText(QStringLiteral(CTRL_BTN_SAVE_QT));
	m_btnLoad->setText(QStringLiteral(CTRL_BTN_LOAD_QT));

	auto menu = new QMenu(m_btnSave);
	
	DEF_MENU_ITEM(menu, SUB_CTRL_SAVE_PROJECT_QT, on_SaveProject);
	DEF_MENU_ITEM(menu, SUB_CTRL_SAVE_SHADER_QT, on_SaveShader);

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

    m_lblRendererStatusIcon->setGeometry(500,-8,50, 50);
    m_lblRendererStatusIcon->setStyleSheet("background-color:rgba(0,0,0,0)");   
    
    m_lblRendererStatus->move(550, 12);

    setStyleSheet("#ViewWnd{ border : 2px solid;}");    
	
    RendererOnline(false);
}

void ApproxShaderView::UpdateLinks()
{
	for (auto link : m_links)
	{
		link->Update();
	}
}

ApproxShaderView::~ApproxShaderView()
{
	ClearElements();
}

void ApproxShaderView::ClearElements()
{
	for (int i = 0; i < 6; i++)
	{
		delete m_SP[i];
		m_SP[i] = nullptr;
	}
}

ShaderLabGUIElement* ApproxShaderView::CreateShaderElem(Shader_Type type)
{
    switch (type)
    {
    case VS:
        return setSP(0, VS);
    case PS:
        return setSP(1, PS);
    case GS:
		return setSP(2, GS);
    case CS:
		return setSP(3, CS);
    case DS:
		return setSP(4, DS);
    case HS:
		return setSP(5, HS);
    default:
		return nullptr;
    }
}

void ApproxShaderView::SetSceneProperties(std::vector<ScenePropertyVisualInfo>& info)
{
	m_propertyInfo = &info;
	m_sceneVars = new ApproxSceneVarsContainer(&m_gView, info);

	connect(m_sceneVars, &ApproxSceneVarsContainer::linkAttempt, [&](QWidget* sender, const QPoint& mouseGlobalPos)
	{
		QPointF p2(m_gView.mapToScene(m_gView.mapFromGlobal(mouseGlobalPos)));
		for (auto elem : m_SP)
		{
			if (elem)
			{
				if (elem->sceneBoundingRect().contains(p2))
				{
					pair<QWidget*, ShaderLabGUIElement*> req(sender, elem);
					for (auto link : m_links)
					{
						if (link->GetLinkingObjects() == req)
							return;
					}
					Link* lnk = new Link(&m_gView, req);
					m_links.push_back(lnk);
					return;
				}
			}
		}
	});

	m_sceneVars->move(2, 50);
	if (!isHidden())
		m_sceneVars->show();
	SetApproxVars();
}

void ApproxShaderView::SetApproxVars()
{
	if (m_propertyInfo)
	{
		vector<QString> approxVars;
		for (auto elem : *m_propertyInfo)
		{
			approxVars.push_back(elem.name);
		}
		for (auto elem : m_SP)
		{
			if (elem)
				elem->codeEditor()->SyntaxHilighter()->SetApproxVars(approxVars);
		}
	}
}

void ApproxShaderView::SetApproxVars(ShaderLabGUIElement* elem)
{
	if (m_propertyInfo)
	{
		vector<QString> approxVars;
		for (auto elem : *m_propertyInfo)
		{
			approxVars.push_back(elem.name);
		}
		elem->codeEditor()->SyntaxHilighter()->SetApproxVars(approxVars);
	}
}

void ApproxShaderView::RendererOnline(bool online)
{
    if (online)
    {
        m_lblRendererStatusIcon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/Icon_Eye")->scaled(m_lblRendererStatusIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        m_lblRendererStatus->setText("Renderer online");
    }
    else
    {
        m_lblRendererStatusIcon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/Icon_Eye_Closed")->scaled(m_lblRendererStatusIcon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        m_lblRendererStatus->setText("Renderer offline");
    }
}

void ApproxShaderView::on_Compile()
{
	info.m_shaderName = m_wndSettings->ui.txtShaderName->text();
	info.m_SM = V5_0;

	for (int i = 0; i < 6; i++)
	{
		if (m_SP[i])
		{
			ApproxViewShaderPartInfo part_info;
			part_info.entryPoint = QString("main");
			part_info.qStr_code = m_SP[i]->getCode();
			part_info.Shader_Type = m_SP[i]->getShader_Type();
			
			info.m_ShaderParts[i] = part_info;
		}
	}

	if (m_sessionInterface->Compile(info) == ASL_NO_ERROR)
		m_lblStatus->setText(QStringLiteral(COMPILATION_SUCCESS_QT));
	else
		m_lblStatus->setText(QStringLiteral(COMPILATION_FAIL_QT));
}

void ApproxShaderView::on_Settings()
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

void ApproxShaderView::on_SaveShader()
{
	if (m_wndSettings->ui.txtWorkLib->text().size() == 0)
	{
		MSG_BOX_LITERAL(SPECIFY_WORK_DIR_QT, QMessageBox::Information);
	}
	else
	{
		info.m_shaderFilename = m_wndSettings->ui.txtWorkLib->text() + m_wndSettings->ui.txtShaderName->text();
		m_sessionInterface->SaveShader(info);
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

void ApproxShaderView::on_SaveProject()
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
				ApproxViewShaderPartInfo part_info;
				part_info.entryPoint = m_SP[i]->EntryPoint();
				part_info.qStr_code = m_SP[i]->getCode();
				part_info.Shader_Type = m_SP[i]->getShader_Type();
				
				info.m_ShaderParts.push_back(part_info);
			}
		}
		info.m_shaderName = m_wndSettings->ui.txtShaderName->text();
		info.m_SM = toSM(m_wndSettings->ui.comboBox->currentIndex());
		info.m_projectFilename = m_wndSettings->ui.txtWorkLib->text() + m_wndSettings->ui.txtShaderName->text();
		try
		{
			m_sessionInterface->SaveProject(info);
		}
		catch (ApproxException& exc)
		{
			MSG_BOX_W(exc.Message(), QMessageBox::Warning);
		}
	}
}

void ApproxShaderView::on_Load()
{
	info.m_projectFilename = QFileDialog::getOpenFileName(this,	tr("Open Shader project file"), "../", tr("Approx Shader Project (*.asp)"));
	if (info.m_projectFilename.size())
	{
		m_sessionInterface->LoadProject(info);
		m_lblDesc->setText(info.m_shaderName);
		m_wndSettings->ui.txtShaderName->setText(info.m_shaderName);
		
		ClearElements();

		for (auto part : info.m_ShaderParts)
		{
			auto element = CreateShaderElem(part.Shader_Type);
			element->setCode(part.qStr_code);
			element->setShader_Type(part.Shader_Type);
			element->setEntryPoint(part.entryPoint);
		}
	}
}

void ApproxShaderView::on_Frame()
{
    m_gScene.update();
}

void ApproxShaderView::setShaderName(QString name)
{
    m_lblDesc->setText(name);
}

void ApproxShaderView::FadeIn()
{
	m_fadeAnim->stop();
	m_fadeAnim->setStartValue(windowOpacity());
	m_fadeAnim->setDirection(QAbstractAnimation::Direction::Forward);
	m_fadeAnim->start();
}

void ApproxShaderView::FadeOut()
{
	m_fadeAnim->stop();
	m_fadeAnim->setStartValue(windowOpacity());
	m_fadeAnim->setDirection(QAbstractAnimation::Direction::Backward);
	m_fadeAnim->start();
}

ShaderLabGUIElement* ApproxShaderView::setSP(int index, ASL::Shader_Type type)
{
    if (!m_SP[index])
    {
        m_SP[index] = new ShaderLabGUIElement;
        m_SP[index]->setShader_Type(type);
        m_SP[index]->moveBy(100 * index, 0);
        m_gScene.addItem(m_SP[index]);
		m_SP[index]->InitializeComponents();
    }
	SetApproxVars(m_SP[index]);
	return m_SP[index];
}

void ApproxShaderView::resizeEvent(QResizeEvent*)
{
    m_lblDesc->resize(width()-4, m_lblDesc->height());
    QRect rc = m_gView.geometry();
    m_gView.resize(width(), height() - 55);
	m_statusBar->setGeometry(2, height() - 23, width() - 4, 21);
	m_sizeGrip->snapTo(BOTTOM_RIGHT);
}

void ApproxShaderView::moveEvent(QMoveEvent* e)
{
	BaseClass::moveEvent(e);
}

bool inRange(const int val, const int min, const int max)
{
	return val < max && val > min;
}

void ApproxShaderView::mouseMoveEvent(QMouseEvent* event)
{    
	if (m_dragging)
	{
		QPoint newPos = pos();;
		QPoint mousePos = event->pos();
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
		move(newPos);
		event->accept();
	}    
	else
	{
		event->ignore();
	}
	BaseClass::mouseMoveEvent(event);
}

void ApproxShaderView::mousePressEvent(QMouseEvent* event)
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

void ApproxShaderView::mouseReleaseEvent(QMouseEvent* event)
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