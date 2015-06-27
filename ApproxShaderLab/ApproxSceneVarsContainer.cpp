#include "ApproxSceneVarsContainer.h"
#include <QVBoxLayout>
#include "ApproxSceneVar.h"
#include "MaterialVarPoint.h"

ApproxSceneVarsContainer::ApproxSceneVarsContainer(QWidget *parent, std::vector<ScenePropertyVisualInfo>const& infos)
    : QFrame(parent)
{
    setLayout(new QVBoxLayout(this));
    layout()->setSpacing(0);
    layout()->setContentsMargins(5, 0, 0, 0);
    m_vars.reserve(infos.size());
	auto hdl = [&](QWidget* sender, const QPoint& mouseGlobalPos){emit linkAttempt(sender, mouseGlobalPos); };
    for (auto info : infos)
    {
        ApproxSceneVar* var = new ApproxSceneVar(this, info);
        m_vars.push_back(var);
        layout()->addWidget(var);
		connect(var->Point(), &MaterialVarPoint::linkAttempt, hdl);
    }
    resize(150, 20 * m_vars.size());
}

ApproxSceneVarsContainer::~ApproxSceneVarsContainer()
{

}
