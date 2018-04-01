#include "ShaderSettingsElement.h"
#include "LinkingPoint.h"
#include "TextureInfo.h"
#include "ShaderParameterInfo.h"
#include <ApproxGuiResManager.h>
#include <qlabel.h>
#include <qpixmap.h>

namespace ASL
{
	ShaderSettingsElement::ShaderSettingsElement(QWidget *parent):QWidget(parent)
	{
		m_Name = new QLabel(this);
		
		m_Icon = new QLabel(this);
		m_Icon->resize(20, 20);
		m_Point = new LinkingPoint(this);

		resize(200, 20);
		m_Name->setAlignment(Qt::AlignCenter);
		m_Point->setGeometry(0, 0, 20, 20);
		m_Point->setNotInUse();
		m_Icon->move(20, 0);
		m_Name->setGeometry(40, 0, 100, 20);

		connect(m_Point, &LinkingPoint::linkAttempt, [=](const QPoint& pos)
		{
			emit linkAttempt(this, pos);
		});
	}

	ShaderSettingsElement::ShaderSettingsElement(QWidget *parent, const ShaderParameterInfo& info) : ShaderSettingsElement(parent)
	{
		m_Type = PARAMETER;
		m_Name->setText(info.Name.c_str());
		m_Icon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/parameter.png")->scaled(m_Icon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		m_Data = new ShaderParameterInfo;
		*dynamic_cast<ShaderParameterInfo*>(m_Data) = info;
	}

	ShaderSettingsElement::ShaderSettingsElement(QWidget* parent, const TextureInfo& info) : ShaderSettingsElement(parent)
	{
		m_Type = TEXTURE;
		m_Name->setText(info.Name.c_str());
		m_Icon->setPixmap(g_ResManager->GetPic(":/ShaderEditor/texture.png")->scaled(m_Icon->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		m_Data = new TextureInfo;
		*dynamic_cast<TextureInfo*>(m_Data) = info;
	}

	LinkingPoint& ShaderSettingsElement::Point() const
	{
		return *m_Point;
	}

	ShaderSettingsElement::~ShaderSettingsElement()
	{

	}

	QString ShaderSettingsElement::ToCode() const
	{
		QString res;
		switch (m_Type)
		{
		case TEXTURE:
		{
			res += QString("ITexture2D ") + dynamic_cast<TextureInfo*>(m_Data)->Name.c_str() + ";\n";
			break;
		}
		default:
		{
			break;
		}
		}
		return res;
	}

	int ShaderSettingsElement::CodeSize() const
	{
		return ToCode().size();
	}

	AbstractSaveData* ShaderSettingsElement::Info() const
	{
		return m_Data;
	}
}