#include "ShaderSettings.h"
#include <QVBoxLayout>
#include <qpushbutton.h>
#include "Literals.h"
#include <PopupDialog.h>
#include "ShaderSettingsElement.h"

namespace ASL
{
	ShaderSettings::ShaderSettings(QWidget *parent)
		: QWidget(parent)
	{
		new QVBoxLayout(this);
		m_btnAddParameter = new QPushButton;
		layout()->addWidget(m_btnAddParameter);
		m_btnAddTexture = new QPushButton;
		layout()->addWidget(m_btnAddTexture);

		resize(200, 100);

		m_btnAddParameter->setText(CTRL_BTN_ADD_PARAMETER_QT);
		m_btnAddTexture->setText(CTRL_BTN_ADD_TEXTURE_QT);

		connect(m_btnAddParameter,	SIGNAL(clicked()), SLOT(AddParameter()));
		connect(m_btnAddTexture,	SIGNAL(clicked()), SLOT(AddTexture()));
	}

	void ShaderSettings::SetParameters(const QVector<ShaderParamInfo>& params)
	{
		for (auto param : params)
		{
			AddParameter(param);
		}
	}

	void ShaderSettings::SetTextures(const QVector<TextureInfo>& textures)
	{
		for (auto tex : textures)
		{
			AddTexture(tex);
		}
	}

	void ShaderSettings::AddTexture(const TextureInfo& texture)
	{
		m_textures.push_back(texture);
		auto elem = new ShaderSettingsElement(this, texture);
		m_Settings.push_back(elem);
		connect(elem, &ShaderSettingsElement::linkAttempt, [=](ShaderSettingsElement *sender, const QPoint& mouseGlobalPos)
		{
			emit linkAttempt(sender, mouseGlobalPos);
		});
		dynamic_cast<QVBoxLayout*>(layout())->insertWidget(0, elem);
		resize(size() + QSize(0, 20));
	}

	void ShaderSettings::AddParameter(const ShaderParamInfo& param)
	{
		m_params.push_back(param);
		auto elem = new ShaderSettingsElement(this, param);
		m_Settings.push_back(elem);
		connect(elem, &ShaderSettingsElement::linkAttempt, [=](ShaderSettingsElement *sender, const QPoint& mouseGlobalPos)
		{
			emit linkAttempt(sender, mouseGlobalPos);
		});
		dynamic_cast<QVBoxLayout*>(layout())->insertWidget(0, elem);
		resize(size() + QSize(0, 20));
	}

	const QVector<TextureInfo>& ShaderSettings::Textures()const
	{
		return m_textures;
	}

	const QVector<ShaderParamInfo>& ShaderSettings::Parameters() const
	{
		return m_params;
	}

	const QVector<ShaderSettingsElement*>& ShaderSettings::AllSettings() const
	{
		return m_Settings;
	}

	void ShaderSettings::ClearElements()
	{
		for (auto s : m_Settings)
		{
			delete s;
		}
		m_Settings.clear();
	
		m_textures.clear();
		m_params.clear();
		resize(200, 100);
	}

	void ShaderSettings::AddTexture()
	{
		ApproxGUI::PopupDialog* dialog = new ApproxGUI::PopupDialog(parentWidget());
		dialog->move(mapToGlobal(m_btnAddTexture->pos()));
		dialog->Exec(LBL_TEXTURE_NAME_QT);
		
		connect(dialog, &ApproxGUI::PopupDialog::OK, [=]()
		{
			TextureInfo info;
			auto data = dialog->Data();
			info.Name = data[0].toStdString();
			info.Slot = m_textures.size();
			AddTexture(info);
		});		
	}

	void ShaderSettings::AddParameter()
	{
		ApproxGUI::PopupDialog* dialog = new ApproxGUI::PopupDialog(parentWidget());
		QVector<QString> Types;
		Types.push_back("float");
		Types.push_back("float2");
		Types.push_back("float3");
		Types.push_back("float4");
		Types.push_back("int");
		Types.push_back("uint");
		dialog->move(mapToGlobal(m_btnAddParameter->pos()));
		dialog->Exec(LBL_PARAMETER_NAME_QT,QPair<QString,QVector<QString>>(LBL_PARAMETER_TYPE_QT,Types));
		
		connect(dialog, &ApproxGUI::PopupDialog::OK, [=]()
		{
			ShaderParamInfo info;
			auto data = dialog->Data();
			info.Name = data[0].toStdString();
			info.Type = data[1].toStdString();
			info.ID = m_params.size();
			AddParameter(info);
		});		
	}

	ShaderSettings::~ShaderSettings()
	{

	}
}
