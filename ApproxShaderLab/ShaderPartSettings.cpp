#include "ShaderPartSettings.h"
#include "Literals.h"

#include <qlabel.h>
#include <qlineedit.h>

ShaderPartSettings::ShaderPartSettings(QWidget *parent)
	: BaseClass(parent), m_Settings()
{
	SetAnimationVariation(GO_LEFT_AND_EXPAND);

	m_lblDescEntryPoint = new QLabel(this);
	m_lblTitle = new QLabel(this);
	m_txtEntryPoint = new QLineEdit(this);

	m_lblDescEntryPoint->setStyleSheet("QLabel { color : rgb(150,150,150); }");
	m_lblTitle->setStyleSheet(m_lblDescEntryPoint->styleSheet());
	m_lblTitle->setFont(QFont("Consolas", 15));
	m_txtEntryPoint->setStyleSheet("QLineEdit{ color:white; border:none; background-color:rgb(35, 35, 35); }");

	resize(200, 100);
	m_memoredSize = size();

	m_lblDescEntryPoint->move(5, 30);
	m_txtEntryPoint->move(5, 50);

	m_txtEntryPoint->resize(175, 20);

	m_lblTitle->setText(QStringLiteral(LBL_SETTINGS_QT));
	m_lblDescEntryPoint->setText(QStringLiteral(LBL_ENTRY_POINT_QT));

	UpdateSettings(FROM_STRUCT);
}

ShaderPartSettings::Settings const& ShaderPartSettings::getSettings()
{
	UpdateSettings(FROM_UI);

	return m_Settings;
}

ShaderPartSettings::Settings const& ShaderPartSettings::getSettings() const
{
	return m_Settings;
}

void ShaderPartSettings::setSettings(const ShaderPartSettings::Settings& set)
{
	m_Settings = set;

	UpdateSettings(FROM_STRUCT);
}

void ShaderPartSettings::UpdateSettings(UpdatePolicy updPolicy)
{
	switch (updPolicy)
	{
	case FROM_UI:
	{
		m_Settings.EntryPoint = m_txtEntryPoint->text();
	}
	case FROM_STRUCT:
	{
		m_txtEntryPoint->setText(m_Settings.EntryPoint);
	}
	}
}

ShaderPartSettings::~ShaderPartSettings()
{

}
