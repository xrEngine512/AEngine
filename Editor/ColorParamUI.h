#ifndef COLORPARAMUI_H
#define COLORPARAMUI_H

#include <QWidget>
#include "ui_ColorParamUI.h"

namespace ASL{
	struct ShaderParameterInfo;
}

namespace ShaderSystem{
	union floatVariant;
}

class ColorParamUI : public QWidget
{
	Q_OBJECT

public:
	ColorParamUI(QWidget *parent, const ASL::ShaderParameterInfo& param);
	~ColorParamUI();
signals:
	void valueChanged(const ShaderSystem::floatVariant&);
private:
	Ui::ColorParamUI ui;
	void SetColor(const QColor& c);
private slots:
	void On_ColorChange();
	void On_PickColor();
};

#endif // COLORPARAMUI_H
