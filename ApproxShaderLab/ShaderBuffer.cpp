#include "ShaderBuffer.h"
#include "MaterialVarInfo.h"
#include "ShaderParamInfo.h"

#include <algorithm>

namespace ASL
{
	ShaderBuffer::ShaderBuffer(QObject *parent, const QString& name)
		: QObject(parent),m_bufSize(0)
	{
	}

	ShaderBuffer::ShaderBuffer(QObject *parent, VarGroup group) : QObject(parent), m_bufSize(0)
	{
		m_info.group = static_cast<MatInterface::VarGroup>( group);
		switch (group)
		{
		case SCENE_CONSTANT:
		{
			m_bufferName = "Constants";
			break;
		}
		case PER_FRAME:
		{
			m_bufferName = "PerFrame";
			break;
		}
		case PER_OBJECT:
		{
			m_bufferName = "PerObject";
			break;
		}
		case SHADER_PARAMS:
		{
			m_bufferName = "Parameters";
			break;
		}
		default:
		{
			m_bufferName = "Error";
			break;
		}
		}
		
	}

	void ShaderBuffer::AddVariable(int ID, const QString& type, const QString& name)
	{
		m_info.IDs.push_back(ID);
		m_bufSize += SizeOf(type);
	}

	void ShaderBuffer::AddVariable(const MaterialVarInfo& var)
	{
		m_vars.push_back(&var);
		std::sort(m_vars.begin(), m_vars.end(), [](const MaterialVarInfo* a, const MaterialVarInfo* b)
		{
			return SizeOf(a->type) > SizeOf(b->type);
		});		
		m_bufSize += SizeOf(var.type);
	}

	void ShaderBuffer::AddVariable(const ShaderParamInfo& var)
	{
		m_params.push_back(&var);
		std::sort(m_params.begin(), m_params.end(), [](const ShaderParamInfo* a, const ShaderParamInfo* b)
		{
			return SizeOf(a->Type.c_str()) > SizeOf(b->Type.c_str());
		});
		m_bufSize += SizeOf(var.Type.c_str());		
	}

	void ShaderBuffer::DeleteVariable(const MaterialVarInfo& var)
	{
		for (auto variable = m_vars.begin(); variable != m_vars.end(); ++variable)
		{
			if ((*variable)->ID == var.ID)
			{
				m_vars.erase(variable);
				break;
			}
		}
	}

	QString ShaderBuffer::ToCode() const
	{
		if (!m_vars.size()&&!m_params.size())
			return QString();
		QString res;
		res += "cbuffer " + m_bufferName + "\n{\n";
		for (auto var : m_vars)
		{
			res += VarToCode(*var);
		}

		for (auto var : m_params)
		{
			res += VarToCode(*var);
		}

		int pNum = 0;
		auto bSize = m_bufSize;
		while (bSize % 16 != 0)
		{
			res += "\tfloat\tpadding" + QString().setNum(pNum) + ";\n";
			bSize += 4;
			++pNum;
		}
		res += "};\n\n";
		return res;
	}

	int ShaderBuffer::CodeSize() const
	{
		return ToCode().size();
	}

	const RuntimeBufferInfo& ShaderBuffer::Info() 
	{
		m_info.IDs.clear();
		for (auto var : m_vars)
		{
			m_info.IDs.push_back(var->ID);
		}
		
		return m_info;
	}

	const std::vector<const ShaderParamInfo*>& ShaderBuffer::Params() const
	{
		return m_params;
	}

	QString ShaderBuffer::NormalizeType(const QString& type)
	{
		if (type.compare("float") == 0)
		{
			return "float";
		}
		if (type.compare("struct DirectX::XMFLOAT4") == 0)
		{
			return "float4";
		}
		if (type.compare("struct DirectX::XMFLOAT3") == 0)
		{
			return "float3";
		}
		if (type.compare("struct DirectX::XMFLOAT2") == 0)
		{
			return "float2";
		}
		if (type.compare("struct DirectX::XMMATRIX") == 0)
		{
			return "matrix";
		}
		if (type.compare("struct DirectX::XMVECTOR") == 0)
		{
			return "float4";
		}
		return nullptr;
	}

	int ShaderBuffer::SizeOf(const QString& type)
	{
		if (type.compare("float") == 0)
		{
			return 4;
		}
		if (type.compare("struct DirectX::XMFLOAT4") == 0 || type.compare("XMFLOAT4") == 0 || type.compare("float4") == 0)
		{
			return 16;
		}
		if (type.compare("struct DirectX::XMFLOAT3") == 0 || type.compare("XMFLOAT3") == 0 || type.compare("float3") == 0)
		{
			return 12;
		}
		if (type.compare("struct DirectX::XMFLOAT2") == 0 || type.compare("XMFLOAT2") == 0 || type.compare("float2") == 0)
		{
			return 8;
		}
		if (type.compare("struct DirectX::XMMATRIX") == 0 || type.compare("XMMATRIX") == 0 || type.compare("matrix") == 0)
		{
			return 64;
		}
		if (type.compare("struct DirectX::XMVECTOR") == 0 || type.compare("XMVECTOR") == 0 || type.compare("float4") == 0)
		{
			return 16;
		}
		return 0;
	}

	ShaderBuffer::~ShaderBuffer()
	{

	}

	inline QString ShaderBuffer::VarToCode(const MaterialVarInfo& var)const
	{
		return '\t' + NormalizeType(var.type) + '\t' + var.name + ";\n";
	}

	inline QString ShaderBuffer::VarToCode(const ShaderParamInfo& var)const
	{
		return QString('\t') + var.Type.c_str() + '\t' + var.Name.c_str() + ";\n";
	}
}
