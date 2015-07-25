#include "Editor.h"
#include <QTreeWidget>
#include <ShaderDesc.h>
#include <IApproxShaderLabExternalControl.h>
#include <IApproxShaderLabControl.h>
#include "rendererwrapper.h"
#include <qmenubar.h>
static const int maxTabWidth = 200;

Editor::Editor(QWidget *parent)
	: ApproxWindow(), m_modelLoader(this), m_rendererWrp(nullptr), m_renderingWindowX(7), m_renderingWindowY(65), m_UpdateThread(nullptr)
{
	ui.setupUi(client);
	setMouseTracking(true);
	InitUI();
	m_asl = ASL::GetASLControl();
	m_StatusBarText = new QLabel(client);
	m_rendererWrp = new RendererWrapper(client);
	m_rendererWrp->hide();
	m_done = false;
	m_menuBar = new QMenuBar(client);
	QMenu* subMenu1 = m_menuBar->addMenu(QStringLiteral("Действия"));
	QMenu* subMenu2 = m_menuBar->addMenu(QStringLiteral("Инструменты"));

	actRunEngine = subMenu1->addAction(QStringLiteral("Запустить графику"));
	actLoadModel = subMenu1->addAction(QStringLiteral("Загрузить модель"));
	actShaderLab = subMenu2->addAction(QStringLiteral("Редактор шейдеров"));

	actLoadModel->setEnabled(false);

	connect(actRunEngine, SIGNAL(triggered()), SLOT(sl_RunEngine()));
	connect(actShaderLab, SIGNAL(triggered()), SLOT(sl_RunShaderLab()));
	connect(actLoadModel, SIGNAL(triggered()), &m_modelLoader, SLOT(show()));
	connect(&m_modelLoader, SIGNAL(accepted()), SLOT(sl_LoadModel()));

	/*ui.statusBar->addPermanentWidget(m_StatusBarText);*/
}

void Editor::sl_RunEngine()
{
	m_rendererWrp->show();
	m_rendererWrp->Initialize();
	m_rendererWrp->Run();
	m_UpdateThread = new std::thread(&Editor::Update, this);
	m_rendererWrp->move(0, 35);
	resize(m_rendererWrp->width() + ui.treeWidget->width(), m_rendererWrp->height() + 100);
	ui.tabWidget->move(m_rendererWrp->width(), 0);
	m_rendererWrp->NativeInterface()->GetRendererEx()->GetScene()->SetASLInterface(dynamic_cast<ASL::IApproxShaderLabExternalControl*>(m_asl));
	actRunEngine->setEnabled(false);
	actLoadModel->setEnabled(true);
	m_modelLoader.SetAvailableShaders(m_rendererWrp->NativeInterface()->GetRendererEx()->AvailableShadersDesc());
}

void Editor::sl_LoadModel()
{
	int ShaderID;
	char *objFile;
	wchar_t **ddsFiles;
	int numberOfTextures;
	m_modelLoader.GetInitData(objFile, ddsFiles, numberOfTextures, ShaderID);
	IExternalRenderObject* object = m_rendererWrp->LoadModel(objFile, ShaderID);
	for (unsigned short i = 0; i < numberOfTextures; i++)
	{
		object->GetMaterial()->LoadTexture(ddsFiles[i], i);
	}
	m_objects.push_back(object);
	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, QString("RenderObject"));
	ui.treeWidget->addTopLevelItem(item);
	return;
}

void Editor::sl_RunShaderLab()
{
	m_asl->Run();
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
	return false;
}

void Editor::Update()
{
	while (!m_done.load())
	{
		float x, y, z;
		m_rendererWrp->GetCameraPos(x, y, z);
		//m_StatusBarText->setText("Camera position X=" + QString().setNum(x) + "  Y=" + QString().setNum(y) + "  Z=" + QString().setNum(z));
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
	}
}

void Editor::moveEvent(QMoveEvent *event)
{
	if (m_rendererWrp)
		m_rendererWrp->PosUpdate();
}

void Editor::closeEvent(QCloseEvent* event)
{
	m_done.store(true);
	if (m_UpdateThread)
	{
		if (m_UpdateThread->joinable())
			m_UpdateThread->join();
	}
	if (m_rendererWrp)
		m_rendererWrp->Shutdown();
	if (m_asl)
		m_asl->Shutdown();
}

void Editor::resizeEvent(QResizeEvent* e)
{
	int tabWidth = width() - m_rendererWrp->x() - m_rendererWrp->width();
	if (tabWidth > maxTabWidth){ tabWidth = maxTabWidth; }

	ui.tabWidget->setGeometry(width() - tabWidth, ButtonSize(), tabWidth, height() - ButtonSize() - m_menuBar->height());

	ApproxWindow::resizeEvent(e);
}

Editor::~Editor()
{

}
