#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <ApproxWindow.h>
#include "ui_Editor.h"
#include "modelloaderui.h"
#include <thread>
#include <atomic>
#include <vector>
#include <memory>
#include <QtWidgets/QMenuBar>

using namespace ApproxGUI;
namespace ASL{
	interface IApproxShaderLabControl;
}
interface IExternalRenderObject;
class RendererWrapper;
class QStatusBar;
class Editor : public ApproxWindow<QWidget>
{
	Q_OBJECT
	
private slots:
	void sl_RunEngine();
	void sl_LoadModel();
	void sl_RunShaderLab();
	void on_SceneItemClick(QTreeWidgetItem*, int);

	void start_update_thread();
	void stop_update_thread();
public:
	Editor(QWidget *parent = 0);
	~Editor();
private:
	bool nativeEvent(const QByteArray&, void*, long*)override;
	
	void Update();
	void closeEvent(QCloseEvent *event)override;
	void moveEvent(QMoveEvent *event)override;
	void resizeEvent(QResizeEvent*)override;

	QLabel* m_StatusBarText;
	RendererWrapper* m_rendererWrp;
	ModelLoaderUI m_modelLoader;
	QMenuBar* m_menuBar;
	QStatusBar* m_statusBar;
	QAction* actRunEngine, *actShaderLab, *actLoadModel;

	ASL::IApproxShaderLabControl* m_asl;
	std::vector<IExternalRenderObject*> m_objects;
	int m_renderingWindowX, m_renderingWindowY;
	std::atomic_bool done;
	std::unique_ptr<std::thread> update_thread;
	Ui::Editor ui;
};

#endif // EDITOR_H
