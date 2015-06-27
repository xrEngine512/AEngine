#ifndef SHADERLABGUI_H
#define SHADERLABGUI_H

#include <QWidget>
#include "GeneratedFiles\ui_shaderlabgui.h"
#include <map>
#include <vector>
#include "ScenePropertyVisualInfo.h"
#include <ApproxWindow.h>

class ApproxShaderView;
class GlobalSettingsForm;

namespace ApproxGUI{
    class ApproxGUIMenuButton;
	class ApproxGUIMenuBar;
}

__interface IControllerSession;

class ShaderLabGUI : public ApproxGUI::ApproxWindow<QWidget>
{
    Q_OBJECT
private slots:
	void on_Settings();
	void on_NewView();
public:
	ShaderLabGUI(IControllerSession*);
    ~ShaderLabGUI();

    void SetSceneProperties(std::vector<ScenePropertyVisualInfo>&);
    
private:
    std::map<QString, ApproxShaderView*> m_Views;
    std::vector<ScenePropertyVisualInfo>* m_propertyInfo;

	ApproxGUI::ApproxGUIMenuBar* m_Menu;
	ApproxGUI::ApproxGUIMenuButton *m_btnSettings , *m_btnAddView;

    Ui::ShaderLabGUI ui;
	GlobalSettingsForm *m_settings;
	bool m_rendererOnline;
	
	IControllerSession* m_ctrl;
    void resizeEvent(QResizeEvent*)override;
    QString generateViewName();
    void hideEvent(QHideEvent*)override;
};

#endif // SHADERLABGUI_H
