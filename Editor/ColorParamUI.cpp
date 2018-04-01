#include "ColorParamUI.h"
#include <qcolordialog.h>
#include <ShaderSystemTypes.h>
#include <ShaderParameterInfo.h>

using namespace ShaderSystem;


QString COLOR_QSS(const QColor& c)
{
	QString res("QLabel{ background-color : rgb(%1, %2, %3); color: rgb(%1, %2, %3); }");
	return res.arg(c.red()).arg(c.green()).arg(c.blue());
}


ColorParamUI::ColorParamUI(QWidget *parent, const ASL::ShaderParameterInfo& param)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btnPickColor, SIGNAL(clicked()), SLOT(On_PickColor()));
	connect(ui.valR, SIGNAL(valueChanged(int)), SLOT(On_ColorChange()));
	connect(ui.valG, SIGNAL(valueChanged(int)), SLOT(On_ColorChange()));
	connect(ui.valB, SIGNAL(valueChanged(int)), SLOT(On_ColorChange()));

	ui.lblParamName->setText(QString::fromStdString(param.Name));
}

ColorParamUI::~ColorParamUI()
{

}

inline floatVariant Convert(const QColor& c)
{
	floatVariant param;
	double rgba[4];
	c.getRgbF(rgba, rgba + 1, rgba + 2, rgba + 3);
	param.float4[0] = static_cast<float>(rgba[0]);
	param.float4[1] = static_cast<float>(rgba[1]);
	param.float4[2] = static_cast<float>(rgba[2]);
	param.float4[3] = static_cast<float>(rgba[3]);
	return param;
}

void ColorParamUI::SetColor(const QColor& c)
{
	ui.lblColor->setStyleSheet(COLOR_QSS(c));
	ui.lblColor->update();
	ui.valR->setValue(c.red());
	ui.valG->setValue(c.green());
	ui.valB->setValue(c.blue());
}

void ColorParamUI::On_ColorChange()
{
	QColor color(ui.valR->value(), ui.valG->value(), ui.valB->value());
	ui.lblColor->setStyleSheet(COLOR_QSS(color));
	ui.lblColor->update();
	emit valueChanged(Convert(color));
}

void ColorParamUI::On_PickColor()
{
	QColorDialog dialog(QColor(ui.valR->value(), ui.valG->value(), ui.valB->value()), this);
	if(dialog.exec())
	{
		 SetColor(dialog.selectedColor());
	}
}