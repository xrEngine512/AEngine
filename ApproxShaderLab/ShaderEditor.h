#ifndef APPROXSHADERVIEW_H
#define APPROXSHADERVIEW_H

#include "GraphicsView.h"
#include "MaterialVarInfo.h"
#include "ViewSessionInfo.h"

class QStatusBar;
class QLabel;
class wnd;
class QPropertyAnimation;
class QGraphicsOpacityEffect;

namespace ApproxGUI{
    class ApproxGUIMenuButton;
    class ApproxGUIMenuBar;
	class SizeGrip;
}

namespace ASL
{
	__interface IControllerSession;

	class Link;
	class MaterialVarsContainer;
	class ShaderLabGUIElement;
	
	class ShaderEditor : public QFrame
	{
		typedef QFrame BaseClass;
		Q_OBJECT

	public:
		ShaderEditor(IControllerSession* sessionInterface, QWidget *parent = nullptr);
		~ShaderEditor();
		void SetMaterialVariables(std::vector<MaterialVarInfo>&);
		void RendererOnline(bool online);
	private slots:
		ShaderLabGUIElement* CreateShaderElem(Shader_Type);
		void on_Compile();
		void on_Settings();
		void on_SaveShader();
		void on_SaveProject();
		void on_Load();
		void on_Frame();
		void UpdateLinks();
		void Raise(ShaderLabGUIElement*);
	public slots:
		void setShaderName(QString);
		void FadeIn();
		void FadeOut();
	private:
		ShaderLabGUIElement* setSP(int index, Shader_Type type);
		void ClearElements();
		void SetApproxVars();
		void SetApproxVars(ShaderLabGUIElement*);

		void resizeEvent(QResizeEvent *)override;
		void moveEvent(QMoveEvent*)override;
		void mouseMoveEvent(QMouseEvent *)override;
		void mousePressEvent(QMouseEvent *)override;
		void mouseReleaseEvent(QMouseEvent *)override;

		QTimer* m_timer;
		ApproxGUI::ApproxGUIMenuButton * m_btnSettings, *m_btnCompile, *m_btnSave, *m_btnLoad;
		GraphicsView m_gView;
		ApproxGUI::ApproxGUIMenuBar* m_Menu;
		ApproxGUI::SizeGrip* m_sizeGrip;
		IControllerSession* m_sessionInterface;
		ViewSessionInfo info;
		QGraphicsScene m_gScene;
		QGraphicsOpacityEffect* m_opacEffect;
		QPropertyAnimation* m_fadeAnim;
		QPropertyAnimation* m_shrinkAnim;

		std::vector<MaterialVarInfo>* m_propertyInfo;

		ShaderLabGUIElement* m_SP[6];
		wnd *m_wndSettings;
		QLabel* m_lblDesc, *m_lblStatus, *m_lblRendererStatus, *m_lblRendererStatusIcon;
		QStatusBar* m_statusBar;
		MaterialVarsContainer* m_sceneVars;

		QPoint m_Prev;
		bool m_dragging;
	};
}
#endif // APPROXSHADERVIEW_H
