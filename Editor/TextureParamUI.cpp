#include "TextureParamUI.h"
#include <TextureInfo.h>
#include <qfiledialog.h>

using namespace ASL;

TextureParamUI::TextureParamUI(QWidget *parent, const TextureInfo& info)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.grbTexName->setTitle(QString::fromStdString(info.Name));

	connect(ui.btnOpen, SIGNAL(clicked()), SLOT(On_OpenFile()));
	connect(ui.txtPath, SIGNAL(textChanged(const QString&)), SLOT(On_FilePathChange(const QString&)));
}

bool FileExists(const QString& filePath)
{
	QFile file(filePath);

	if(file.open(QIODevice::ReadOnly))
	{
		file.close();
		return true;
	}
	return false;
}

TextureParamUI::~TextureParamUI()
{

}

void TextureParamUI::On_OpenFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open dds file"), "../Engine/Resources/Textures", tr("DirectDrawSurface (*.dds)"));
	ui.txtPath->setText(path);
}

void TextureParamUI::On_FilePathChange(const QString& path)
{
	if (FileExists(path)) emit pathChange(path);
}