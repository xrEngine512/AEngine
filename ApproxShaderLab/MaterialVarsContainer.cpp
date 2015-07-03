#include "MaterialVarsContainer.h"
#include <QVBoxLayout>
#include "MaterialVar.h"

namespace ASL
{
	MaterialVarsContainer::MaterialVarsContainer(QWidget *parent, std::vector<MaterialVarInfo>const& infos)
		: QFrame(parent)
	{
		setLayout(new QVBoxLayout(this));
		layout()->setSpacing(0);
		layout()->setContentsMargins(5, 0, 0, 0);
		m_vars.reserve(infos.size());
		auto hdl = [&](MaterialVar* sender, const QPoint& mouseGlobalPos){emit linkAttempt(sender, mouseGlobalPos); };
		for (auto& info : infos)
		{
			MaterialVar* var = new MaterialVar(this, info);
			m_vars.push_back(var);
			layout()->addWidget(var);
			connect(var, &MaterialVar::linkAttempt, hdl);
		}
		resize(150, 20 * m_vars.size());
	}

	MaterialVarsContainer::~MaterialVarsContainer()
	{

	}
}
