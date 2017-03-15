#ifndef SHADERLABGUI_H
#define SHADERLABGUI_H

#include <QWidget>
#include "ui_shaderlabgui.h"
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
	interface IControllerSession;
	
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

		void SetMaterialInterfaceInfo(std::vector<MaterialVarInfo>&, const QString&);

	private:
		std::map<QString, ShaderEditor*> m_Views;
		std::vector<MaterialVarInfo> m_varInfo;
		QString m_mi_version;

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
