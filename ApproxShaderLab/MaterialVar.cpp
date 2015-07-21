#include "MaterialVar.h"
#include <ApproxGuiResManager.h>
#include <qlabel.h>
#include "LinkingPoint.h"
#include "MaterialVarInfo.h"

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

namespace ASL
{

	MaterialVar::MaterialVar(QWidget *parent, const MaterialVarInfo& info)
		: QWidget(parent), m_flag(nullptr), m_info(info)
	{
		resize(140, 20);
		m_flag = new QLabel(this);
		m_name = new QLabel(this);
		m_point = new LinkingPoint(this);

		connect(m_point, &LinkingPoint::linkAttempt, [&](const QPoint& mouseGlobalPos){emit linkAttempt(this, mouseGlobalPos); });

		m_flag->setGeometry(100, 0, 20, 20);
		m_flag->setPixmap(GetPicFromType(info.type)->scaled(m_flag->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

		m_point->setGeometry(125, 0, 20, 20);
		m_point->setNotInUse();

		m_name->setGeometry(0, 0, 100, 20);
		m_name->setText(info.name);

		m_name->setToolTip(info.description);
		m_flag->setToolTip(info.type);
	}

	LinkingPoint& MaterialVar::Point() const
	{
		return *m_point;
	}

	const MaterialVarInfo& MaterialVar::VarInfo()const
	{
		return m_info;
	}

	MaterialVar::~MaterialVar()
	{

	}
}