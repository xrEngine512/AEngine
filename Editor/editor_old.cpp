#include "editor.h"
#include <QTreeWidget>
#include <qpropertyanimation.h>
#include <QMouseEvent>
#include <IApproxShaderLabExternalControl.h>

static const int maxTabWidth = 200;

Editor::Editor(QWidget *parent)
	: ApproxWindow(parent), m_modelLoader(this), m_rendererWrp(nullptr), m_renderingWindowX(7), m_renderingWindowY(65), m_UpdateThread(nullptr)
{
    ui.setupUi(this);
	InitUI(this);
    m_asl = GetASLControl();
    m_StatusBarText = new QLabel(this);
	m_rendererWrp = new RendererWrapper(this);
	m_rendererWrp->hide();
	
    m_done = false;

    connect(ui.actRunEngine, SIGNAL(triggered()), SLOT(s_RunEngine()));
    connect(ui.actionShaderLab, SIGNAL(triggered()), SLOT(s_RunShaderLab()));
    connect(ui.actLoadModel, SIGNAL(triggered()),&m_modelLoader ,SLOT(show()));
    connect(&m_modelLoader, SIGNAL(accepted()), SLOT(s_LoadModel()));
	
    ui.statusBar->addPermanentWidget(m_StatusBarText);	

    
}

void Editor::closeEvent(QCloseEvent* event)
{
    m_done.store(true);
    if (m_UpdateThread)
    {
        if (m_UpdateThread->joinable())
            m_UpdateThread->join();
    }
    if(m_rendererWrp)
        m_rendererWrp->Shutdown();
	if (m_asl)
		m_asl->Shutdown();
}

void Editor::moveEvent(QMoveEvent *event)
{
    if (m_rendererWrp)
        m_rendererWrp->PosUpdate();
}

void Editor::s_RunEngine()
{      
    m_rendererWrp->show();
    m_rendererWrp->Initialize();
    m_rendererWrp->Run();
    m_UpdateThread = new std::thread(&Editor::Update, this);
    m_rendererWrp->move(0,35);
    resize(m_rendererWrp->width()+ui.treeWidget->width(), m_rendererWrp->height()+100);
    ui.tabWidget->move(m_rendererWrp->width(),0);
    m_rendererWrp->NativeInterface()->GetRendererEx()->GetScene()->SetASLInterface(dynamic_cast<IApproxShaderLabExternalControl*>(m_asl));
    ui.actRunEngine->setEnabled(false);
}

void Editor::s_RunShaderLab()
{
    m_asl->Run();
}

void Editor::Update()
{
    while (!m_done.load())
    {
        float x, y, z;
        m_rendererWrp->GetCameraPos(x, y, z);
        m_StatusBarText->setText("Camera position X=" + QString().setNum(x) + "  Y=" + QString().setNum(y) + "  Z=" + QString().setNum(z));
        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }
}

bool Editor::nativeEvent(const QByteArray& event_type, void* message, long* result)
{
    Q_UNUSED(result);

    if (event_type == "windows_generic_MSG")
    {
        if (m_rendererWrp)
        {
            return m_rendererWrp->ExternalNativeEvent(event_type, message, result);
        }
        return false;
    }
#ifdef WIN32
	/*MSG *m = static_cast<MSG*>(message);
	HWND hwnd = m->hwnd;
	WPARAM wParam = m->wParam;
	if (m->message == WM_NCPAINT)
	{
		HDC hdc;
		hdc = GetDCEx(hwnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
		// Paint into this DC 
		
		ReleaseDC(hwnd, hdc);
	}*/
#endif
    return false;
}

void Editor::s_LoadModel()
{
    char *objFile;
    wchar_t **ddsFiles;
    int numberOfTextures;
    m_modelLoader.GetInitData(objFile, ddsFiles,numberOfTextures, type);
    IExternalRenderObject* object = m_rendererWrp->LoadModel(objFile, type);
    for (unsigned short i = 0; i < numberOfTextures; i++)
    {
        object->GetMaterial()->LoadTexture(ddsFiles[i], i);
    }
    object->GetMaterial()->SetSpecularity(1, 1, 1, 10);
    m_objects.push_back(object);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString("RenderObject"));
    ui.treeWidget->addTopLevelItem(item);
    return;
}

void Editor::resizeEvent(QResizeEvent* e)
{
	int tabWidth = width() - m_rendererWrp->x() - m_rendererWrp->width();
	if (tabWidth > maxTabWidth){ tabWidth = maxTabWidth; }

	ui.tabWidget->setGeometry(width() - tabWidth, ButtonSize(), tabWidth, height() - ButtonSize() - ui.statusBar->height() - ui.menuBar->height());

	ApproxWindow::resizeEvent(e);
}

Editor::~Editor()
{
}
