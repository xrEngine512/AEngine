#include "GlobalSettingsForm.h"
#include <qgraphicseffect.h>

GlobalSettingsForm::GlobalSettingsForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_background = new QFrame(this);
	m_background->lower();
	m_background->resize(size());
	m_background->setStyleSheet("border : 1px solid rgb(0,200,255);");
	m_background->setGraphicsEffect(new QGraphicsBlurEffect);
}

GlobalSettingsForm::~GlobalSettingsForm()
{

}

void GlobalSettingsForm::resizeEvent(QResizeEvent*)
{
	m_background->resize(size());
}