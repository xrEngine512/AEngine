#ifndef MODELLOADERUI_H
#define MODELLOADERUI_H

#include <QDialog>
#include "ui_modelloaderui.h"
#include <unordered_map>

struct ShaderDesc;

class ModelLoaderUI : public QDialog
{
	Q_OBJECT

	std::unordered_map<int, ShaderDesc> m_DescsByIndexes;
public:  
    ModelLoaderUI(QWidget *parent = 0);
    ~ModelLoaderUI();
    void GetInitData(std::string &objFilename, std::vector<std::wstring> &ddsFilenames, int &type);
	void SetAvailableShaders(const std::vector<const ShaderDesc*>& availShaders);
private slots:
    void OnObjOpen();
    void OnDDSOpen(int index);
    void OnShaderTypeChange();
private:
    Ui::ModelLoaderUI ui;
    void SetEnabledTextureElement1(bool set);
    void SetEnabledTextureElement2(bool set);
    void SetEnabledTextureElement3(bool set);
    void SetEnabledTextureElement4(bool set);
};

#endif // MODELLOADERUI_H
