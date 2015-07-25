#include "modelloaderui.h"
#include <qfiledialog.h>
#include <ShaderDesc.h>

wchar_t* QStringToWChar(QString qstr);
char* QStringToChar(QString qstr);

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

void ModelLoaderUI::SetAvailableShaders(const std::vector<const ShaderDesc*>& availShaders)
{
	int i = 0;
	for (auto desc : availShaders)
	{
		ui.cbxShaderType->addItem(QString::fromStdString(desc->name));
		m_IDsByIndexes.insert(std::pair<int , int>( i, desc->ID));
		i++;
	}
}

ModelLoaderUI::~ModelLoaderUI()
{

}

void ModelLoaderUI::OnObjOpen()
{
    ui.lblMeshFile->setText(QFileDialog::getOpenFileName(this,
        tr("Open obj file"), "D:/BackUpBecauseofShitHappens/Users/Islam/Desktop/DX/Core/Engine/Resources/Meshes", tr("WaweFront (*.obj)")));
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
            tr("Open DDS file"), "D:/BackUpBecauseofShitHappens/Users/Islam/Desktop/DX/Core/Engine/Resources/Textures", tr("DirectDrawSurface (*.dds)")));
}

void ModelLoaderUI::OnShaderTypeChange()
{
    switch (ui.cbxShaderType->currentIndex())
    {
    case 0:
	case 1:
    case 2:
    case 3:
	case 4: 
        SetEnabledTextureElement1(false);
        SetEnabledTextureElement2(false);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
    
        SetEnabledTextureElement1(true);
        SetEnabledTextureElement2(false);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
    
        SetEnabledTextureElement1(true);
        SetEnabledTextureElement2(true);
        SetEnabledTextureElement3(false);
        SetEnabledTextureElement4(false); break;
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

void ModelLoaderUI::GetInitData(char* &objFilename, wchar_t** &ddsFilenames, int &numberOfTextures, int &ID)
{
    ddsFilenames = new wchar_t*[4];
    numberOfTextures = 0;

    if (ui.ledTex1->isEnabled())
    {
        ddsFilenames[0] = QStringToWChar(ui.ledTex1->text());
        numberOfTextures++;
    }
    if (ui.ledTex2->isEnabled())
    {
        ddsFilenames[1] = QStringToWChar(ui.ledTex2->text());
        numberOfTextures++;
    }
    if (ui.ledTex3->isEnabled())
    {
        ddsFilenames[2] = QStringToWChar(ui.ledTex3->text());
        numberOfTextures++;
    }
    if (ui.ledTex4->isEnabled())
    {
        ddsFilenames[3] = QStringToWChar(ui.ledTex4->text());
        numberOfTextures++;
    }

    objFilename = QStringToChar(ui.lblMeshFile->text());
    ID = m_IDsByIndexes.find(ui.cbxShaderType->currentIndex())->second;
}

wchar_t* QStringToWChar(QString qstr)
{
    wchar_t* buf;
    int size = qstr.size();
    buf = new wchar_t[size + 1];
    qstr.toWCharArray(buf);
    buf[size] = '\0';
    return buf;
}

char* QStringToChar(QString qstr)
{
    char* c_buf;
    wchar_t *w_buf;
    int size = qstr.size();
    c_buf = new char[size + 1]; 
    w_buf = new wchar_t[size + 1];
    qstr.toWCharArray(w_buf);
    wcstombs(c_buf, w_buf, size);
    delete[] w_buf;
    c_buf[size] = '\0';
    return c_buf;
}