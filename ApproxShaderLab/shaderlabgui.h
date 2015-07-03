#ifndef SHADERLABGUI_H
#define SHADERLABGUI_H

#include <QWidget>
#include "GeneratedFiles\ui_shaderlabgui.h"
#include <map>
#include <vector>
#include "MaterialVarInfo.h"
#include <ApproxWindow.h>

namespace ApproxGUI{
    class ApproxGUIMenuButton;
	class ApproxGUIMenuBar;
}

namespace ASL
{
	__interface IControllerSession;
	
	class ShaderEditor;
	class GlobalSettingsForm;

	class ShaderLabGUI : public ApproxGUI::ApproxWindow < QWidget >
	{
		Q_OBJECT
			private slots:
		void on_Settings();
		void on_NewView();
	public:
		ShaderLabGUI(IControllerSession*);
		~ShaderLabGUI();

		void SetMaterialVariables(std::vector<MaterialVarInfo>&);

	private:
		std::map<QString, ShaderEditor*> m_Views;
		std::vector<MaterialVarInfo> m_varInfo;

		ApproxGUI::ApproxGUIMenuBar* m_Menu;
		ApproxGUI::ApproxGUIMenuButton *m_btnSettings, *m_btnAddView;

		Ui::ShaderLabGUI ui;
		GlobalSettingsForm *m_settings;
		bool m_rendererOnline;

		IControllerSession* m_ctrl;
		void resizeEvent(QResizeEvent*)override;
		QString generateViewName();
		void hideEvent(QHideEvent*)override;
	};
}
#endif // SHADERLABGUI_H
