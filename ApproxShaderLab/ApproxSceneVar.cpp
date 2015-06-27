#include "ApproxSceneVar.h"
#include <ApproxGuiResManager.h>
#include <qlabel.h>
#include "MaterialVarPoint.h"

QPixmap* GetPicFromType(const QString& type)
{
	if (type.compare("float") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_float.png");
	}
	if (type.compare("struct DirectX::XMFLOAT4") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_xmfloat4.png");
	}
	if (type.compare("struct DirectX::XMFLOAT3") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_xmfloat3.png");
	}
	if (type.compare("struct DirectX::XMFLOAT2") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_xmfloat2.png");
	}
	if (type.compare("struct DirectX::XMMATRIX") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_xmmatrix.png");
	}
	if (type.compare("struct DirectX::XMVECTOR") == 0)
	{
		return g_ResManager->GetPic(":/ShaderEditor/flag_xmvector.png");
	}
	return nullptr;
}

ApproxSceneVar::ApproxSceneVar(QWidget *parent, const ScenePropertyVisualInfo& info)
    : QWidget(parent), m_flag(nullptr), m_info(info)
{
    resize(140, 20);
    m_flag = new QLabel(this);
    m_name = new QLabel(this);
	m_point = new MaterialVarPoint(this);

    m_flag->setGeometry(100, 0, 20, 20);
    m_flag->setPixmap(GetPicFromType(info.type)->scaled(m_flag->size(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

	m_point->setGeometry(125, 0, 20, 20);
	m_point->setPixmap(g_ResManager->GetPic(":/ShaderEditor/flag_point.png")->scaled(m_point->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    m_name->setGeometry(0,0,100,20);
    m_name->setText(info.name);

	m_name->setToolTip(info.description);
	m_flag->setToolTip(info.type);
}

const MaterialVarPoint* ApproxSceneVar::Point() const
{
	return m_point;
}

ApproxSceneVar::~ApproxSceneVar()
{

}
