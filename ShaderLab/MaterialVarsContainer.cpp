#include "MaterialVarsContainer.h"
#include <QVBoxLayout>
#include <qlabel.h>
#include "MaterialVar.h"

namespace ASL
{
	MaterialVarsContainer::MaterialVarsContainer(QWidget *parent, std::vector<MaterialVarInfo>const& infos, const QString& version)
		: QFrame(parent)
	{
		setLayout(new QVBoxLayout(this));
		layout()->setSpacing(0);
		layout()->setContentsMargins(5, 0, 0, 0);
		m_vars.reserve(infos.size());
		auto hdl = [&](MaterialVar* sender, const QPoint& mouseGlobalPos){emit linkAttempt(sender, mouseGlobalPos); };
		m_title = new QLabel(this);
		layout()->addWidget(m_title);
		m_title->setAlignment(Qt::AlignLeft);
		m_title->setText("MI " + version);
		m_title->setFont(QFont("Consolas",10));
		m_title->setToolTip("Material Interface " + version);
		for (auto& info : infos)
		{
			MaterialVar* var = new MaterialVar(this, info);
			m_vars.push_back(var);
			layout()->addWidget(var);
			connect(var, &MaterialVar::linkAttempt, hdl);
		}
		resize(150, 20 * m_vars.size() + 20);
	}

	const std::vector<MaterialVar*>& MaterialVarsContainer::Vars() const
	{
		return m_vars;
	}

	MaterialVarsContainer::~MaterialVarsContainer()
	{
	}
}
