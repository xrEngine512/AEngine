#ifndef MATERIALSETTINGSUI_H
#define MATERIALSETTINGSUI_H

#include <QDialog>
#include <unordered_map>

struct IExternalMaterial;

namespace ShaderSystem
{
	enum GenericType;
}

class MaterialSettingsUI : public QDialog
{
	Q_OBJECT

public:
	MaterialSettingsUI(QWidget *parent, IExternalMaterial* material);
	~MaterialSettingsUI();

private:
	std::unordered_map<ShaderSystem::GenericType, QWidget*> m_Param;

	QPushButton* btnOK;
};

#endif // MATERIALSETTINGSUI_H
