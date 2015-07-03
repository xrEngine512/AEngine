#pragma once
#include <QString>
#include "Enums.h"
#include <qvector.h>

namespace ASL
{
	struct ViewShaderPartInfo
	{
		QString qStr_code;
		QString entryPoint;
		ASL::Shader_Type Shader_Type = ASL::ST_NONE;
	};

	struct ViewSessionInfo
	{
		int m_SessionID = 0;
		ASL::Shader_Model m_SM = ASL::SM_NONE;
		QString m_shaderName;
		QString m_shaderFilename;
		QString m_projectFilename;
		QVector<ViewShaderPartInfo> m_ShaderParts;
	};
}