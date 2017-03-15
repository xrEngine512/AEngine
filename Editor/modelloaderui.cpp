#include "modelloaderui.h"
#include <qfiledialog.h>
#include <ShaderDesc.h>

ModelLoaderUI::ModelLoaderUI(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btnOpenObjFile, SIGNAL(clicked()), SLOT(OnObjOpen()));

    connect(ui.cbxShaderType, SIGNAL(currentIndexChanged(int)), SLOT(OnShaderTypeChange()));
    OnShaderTypeChange();
 
	connect(ui.btnOpenDDSFile,   &QPushButton::clicked, [=](){OnDDSOpen(0); });
	connect(ui.btnOpenDDSFile_2, &QPushButton::clicked, [=](){OnDDSOpen(1); });
	connect(ui.btnOpenDDSFile_3, &QPushButton::clicked, [=](){OnDDSOpen(2); });
	connect(ui.btnOpenDDSFile_4, &QPushButton::clicked, [=](){OnDDSOpen(4); });
}

void ModelLoaderUI::SetAvailableShaders(const std::vector<ShaderDesc>& availShaders)
{
	ui.cbxShaderType->clear();
	m_DescsByIndexes.clear();
	int i = 0;
	for (auto desc : availShaders)
	{
		ui.cbxShaderType->addItem(QString::fromStdString(desc.name));
		m_DescsByIndexes.insert({i, desc});
		i++;
	}
	OnShaderTypeChange();
}

ModelLoaderUI::~ModelLoaderUI()
{

}

void ModelLoaderUI::OnObjOpen()
{
    ui.lblMeshFile->setText(QFileDialog::getOpenFileName(this,
        tr("Open obj file"), "../Engine/Resources/Meshes", tr("WaweFront (*.obj)")));
}

void ModelLoaderUI::OnDDSOpen(int index)
{
    QLineEdit* led = nullptr;
    switch (index)
    {
    case 0: led = ui.ledTex1; break;
    case 1: led = ui.ledTex2; break;
    case 2: led = ui.ledTex3; break;
    case 3: led = ui.ledTex4; break;
    }
    if (led)
        led->setText(QFileDialog::getOpenFileName(this,
            tr("Open DDS file"), "../Engine/Resources/Textures", tr("DirectDrawSurface (*.dds)")));
}

void ModelLoaderUI::OnShaderTypeChange()
{
	auto res = m_DescsByIndexes.find(ui.cbxShaderType->currentIndex());
	int numOfTextures;
	if (res != m_DescsByIndexes.end())
		numOfTextures = res->second.TextureSlots;
	else
		numOfTextures = 0;

	switch (numOfTextures)
    {
    case 0:	 
        SetEnabledTextureElement1(false);
        SetEnabledTextureElement2(false);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
    case 1:    
        SetEnabledTextureElement1(true);
        SetEnabledTextureElement2(false);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
    case 2:    
        SetEnabledTextureElement1(true);
        SetEnabledTextureElement2(true);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
	case 3:
		SetEnabledTextureElement1(true);
		SetEnabledTextureElement2(true);
		SetEnabledTextureElement3(true);
		SetEnabledTextureElement4(false); break;
	case 4:
		SetEnabledTextureElement1(true);
		SetEnabledTextureElement2(true);
		SetEnabledTextureElement3(true);
		SetEnabledTextureElement4(true); break;
    }
}

void ModelLoaderUI::SetEnabledTextureElement1(bool set)
{
    ui.btnOpenDDSFile->setEnabled(set);
    if (!set)
        ui.lblTex1->setText("Not available");
    else
        ui.lblTex1->setText("Texture 1:");
    ui.ledTex1->setEnabled(set);
}

void ModelLoaderUI::SetEnabledTextureElement2(bool set)
{
    ui.btnOpenDDSFile_2->setEnabled(set);
    if (!set)
        ui.lblTex2->setText("Not available");
    else
        ui.lblTex2->setText("Texture 2:");
    ui.ledTex2->setEnabled(set);
}

void ModelLoaderUI::SetEnabledTextureElement3(bool set)
{
    ui.btnOpenDDSFile_3->setEnabled(set);
    if (!set)
        ui.lblTex3->setText("Not available");
    else
        ui.lblTex3->setText("Texture 3:");
    ui.ledTex3->setEnabled(set);
}

void ModelLoaderUI::SetEnabledTextureElement4(bool set)
{
    ui.btnOpenDDSFile_4->setEnabled(set);
    if (!set)
        ui.lblTex4->setText("Not available");
    else
        ui.lblTex4->setText("Texture 4:");
    ui.ledTex4->setEnabled(set);
}

void ModelLoaderUI::GetInitData(std::string &objFilename, std::vector<std::string> &ddsFilenames, int &ID)
{
    if (ui.ledTex1->isEnabled())
    {
		ddsFilenames.push_back(ui.ledTex1->text().toStdString());
    }
    if (ui.ledTex2->isEnabled())
    {
		ddsFilenames.push_back(ui.ledTex2->text().toStdString());
    }
    if (ui.ledTex3->isEnabled())
    {
		ddsFilenames.push_back(ui.ledTex3->text().toStdString());
    }
    if (ui.ledTex4->isEnabled())
    {
		ddsFilenames.push_back(ui.ledTex4->text().toStdString());
    }

    objFilename = ui.lblMeshFile->text().toStdString();
    ID = m_DescsByIndexes.find(ui.cbxShaderType->currentIndex())->second.ID;
}