#include "MaterialSettingsUI.h"
#include <ShaderSystemTypes.h>
#include <ShaderParamInfo.h>
#include <qslider.h>
#include <QVBoxLayout>
#include <qpushbutton.h>
#include <IExternalMaterial.h>
#include <QtWidgets/QLabel>

using namespace ShaderSystem;

MaterialSettingsUI::MaterialSettingsUI(QWidget* parent, IExternalMaterial* material): QDialog(parent)
{
	setLayout(new QVBoxLayout(this));
	resize(200, 500);
	for (auto info : material->Settings().ParamsInfo)
	{
		auto type = ReadType(info.Type);
		switch (type)
		{
		case FLOAT:
		{
			auto slider = new QSlider(Qt::Horizontal);
			auto label = new QLabel;
			layout()->addWidget(label);
			label->setText(QString::fromStdString(info.Name));
			slider->setMinimum(1);
			slider->setMaximum(1000);
			layout()->addWidget(slider);
			connect(slider, &QSlider::valueChanged, [=](int value)
			{
				floatVariant var;
				var.float1 = static_cast<float>(value) / 100;
				material->SetParameter(info.Name, var);
			});
			break;
		}
		case FLOAT2:
		{
			break;
		}
		case FLOAT3:
		{
			break;
		}
		case FLOAT4:
		{
			break;
		}
		}
	}
	btnOK = new QPushButton(this);
	connect(btnOK, &QPushButton::clicked, [&]()
	{

		accept();
	});
}

MaterialSettingsUI::~MaterialSettingsUI()
{

}
