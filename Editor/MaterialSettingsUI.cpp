#include "MaterialSettingsUI.h"
#include <ShaderSystemTypes.h>
#include <ShaderParamInfo.h>
#include <TextureInfo.h>
#include <QVBoxLayout>
#include <qpushbutton.h>
#include <IExternalMaterial.h>

#include "FloatParamUI.h"
#include "ColorParamUI.h"
#include "TextureParamUI.h"

using namespace ShaderSystem;

MaterialSettingsUI::MaterialSettingsUI(QWidget* parent, IExternalMaterial* material): QWidget(parent)
{
	setLayout(new QVBoxLayout(this));
	layout()->setAlignment(Qt::AlignTop);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::Window);
	resize(200, 500);
	for (auto info : material->Settings().ParamsInfo)
	{
		auto type = ReadType(info.Type);
		switch (type)
		{
		case FLOAT:
		{
			auto paramUI = new FloatParamUI(this, info);
			layout()->addWidget(paramUI);
			connect(paramUI, &FloatParamUI::valueChanged, [=](const floatVariant& value)
			{				
				material->SetParameter(info.Name, value);
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
			auto paramUI = new ColorParamUI(this, info);
			layout()->addWidget(paramUI);
			connect(paramUI, &ColorParamUI::valueChanged, [=](const floatVariant& value)
			{
				material->SetParameter(info.Name, value);
			});
			break;
		}
		default: break;
		}
	}
	for (auto info : material->Settings().TexturesInfo)
	{
		auto paramUI = new TextureParamUI(this, info);
		layout()->addWidget(paramUI);
		connect(paramUI, &TextureParamUI::pathChange, [=](const QString& path)
		{
			material->LoadTexture(path.toStdString(), info.Slot);
		});
	}
	btnOK = new QPushButton(this);
	connect(btnOK, &QPushButton::clicked, [&]()
	{
		close();
	});
}

MaterialSettingsUI::~MaterialSettingsUI()
{

}

void MaterialSettingsUI::closeEvent(QCloseEvent* e)
{
	QWidget::closeEvent(e);
	emit closed();
}