#ifndef SHADERPARTSETTINGS_H
#define SHADERPARTSETTINGS_H

#include <QWidget>
#include <AnimatedGraphicsWidget.h>

class QLabel;
class QLineEdit;

namespace ASL
{
	class ShaderPartSettings : public ApproxGUI::AnimatedGraphicsWidget < QWidget >
	{
		Q_OBJECT

	public:
		struct Settings
		{
			QString EntryPoint = "main";
		};

		ShaderPartSettings(QWidget *parent = nullptr);

		const Settings& getSettings();
		const Settings& getSettings()const;

		void setSettings(const Settings&);
		~ShaderPartSettings();

	private:
		enum UpdatePolicy{ FROM_UI, FROM_STRUCT };

		void UpdateSettings(UpdatePolicy updPolicy);

		QLabel *m_lblDescEntryPoint, *m_lblTitle;
		QLineEdit *m_txtEntryPoint;
		Settings m_Settings;
	};
}
#endif // SHADERPARTSETTINGS_H
