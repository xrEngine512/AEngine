#ifndef APPROXSHADERVIEW_H
#define APPROXSHADERVIEW_H

#include "ApproxGraphicsView.h"
#include "ScenePropertyVisualInfo.h"
#include "ApproxViewSessionInfo.h"

class QStatusBar;
class QLabel;
class wnd;
class ShaderLabGUIElement;
class ApproxSceneVarsContainer;
class QPropertyAnimation;
class QGraphicsOpacityEffect;
class Link;

__interface IControllerSession;

namespace ApproxGUI{
    class ApproxGUIMenuButton;
    class ApproxGUIMenuBar;
	class SizeGrip;
}

class ApproxShaderView : public QFrame
{
	typedef QFrame BaseClass;
    Q_OBJECT

public:
	ApproxShaderView(IControllerSession* sessionInterface, QWidget *parent = nullptr);
    ~ApproxShaderView();
    void SetSceneProperties(std::vector<ScenePropertyVisualInfo>&);
    void RendererOnline(bool online);
private slots: 
	ShaderLabGUIElement* CreateShaderElem(ASL::Shader_Type);   
    void on_Compile();
    void on_Settings();
    void on_SaveShader();
	void on_SaveProject();
	void on_Load();
    void on_Frame();
	void UpdateLinks();
public slots:
    void setShaderName(QString);
	void FadeIn();
	void FadeOut();
private:
	ShaderLabGUIElement* setSP(int index, ASL::Shader_Type type);
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
    ApproxGUI::ApproxGraphicsView m_gView;
    ApproxGUI::ApproxGUIMenuBar* m_Menu;
	ApproxGUI::SizeGrip* m_sizeGrip;
	IControllerSession* m_sessionInterface;
	ApproxViewSessionInfo info;
    QGraphicsScene m_gScene;
	QGraphicsOpacityEffect* m_opacEffect;
	QPropertyAnimation* m_fadeAnim;
	QPropertyAnimation* m_shrinkAnim;

    std::vector<ScenePropertyVisualInfo>* m_propertyInfo;
	std::vector<Link*> m_links;

    ShaderLabGUIElement* m_SP[6];
    wnd *m_wndSettings;
    QLabel* m_lblDesc, *m_lblStatus, *m_lblRendererStatus, *m_lblRendererStatusIcon;
    QStatusBar* m_statusBar;
    ApproxSceneVarsContainer* m_sceneVars;

	QPoint m_Prev;
    bool m_dragging;
};
#endif // APPROXSHADERVIEW_H
