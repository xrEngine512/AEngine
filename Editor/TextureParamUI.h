#ifndef TEXTUREPARAMUI_H
#define TEXTUREPARAMUI_H

#include <QWidget>
#include "ui_TextureParamUI.h"

namespace ASL{
	struct TextureInfo;
}

class TextureParamUI : public QWidget
{
	Q_OBJECT

public:
	TextureParamUI(QWidget *parent, const ASL::TextureInfo& info);
	~TextureParamUI();
signals:
	void pathChange(const QString&);
private slots:
	void On_OpenFile();
	void On_FilePathChange(const QString& path);
private:
	Ui::TextureParamUI ui;
};

#endif // TEXTUREPARAMUI_H
