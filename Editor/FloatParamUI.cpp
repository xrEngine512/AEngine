#include "FloatParamUI.h"
#include <ShaderParamInfo.h>
#include <ShaderSystemTypes.h>
using namespace ASL;

FloatParamUI::FloatParamUI(QWidget *parent, const ShaderParamInfo& param)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.slider->setMinimum(1);
	ui.slider->setMaximum(10000);
	ui.lblParamName->setText(QString::fromStdString(param.Name));
	connect(ui.slider, &QSlider::valueChanged, [=](int value)
	{
		ShaderSystem::floatVariant var;
		var.float1 = static_cast<float>(value) / 100;
		ui.valParamValue->setValue(var.float1);
		emit valueChanged(var);
	});
	connect(ui.valParamValue, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [=](double value)
	{
		ShaderSystem::floatVariant var;
		var.float1 = static_cast<float>(value);
		ui.slider->setValue(var.float1 * 100);
		emit valueChanged(var);
	});
}

FloatParamUI::~FloatParamUI()
{

}
