#include "ShaderBuffer.h"
#include "MaterialVarInfo.h"

namespace ASL
{
	ShaderBuffer::ShaderBuffer(QObject *parent, const QString& name)
		: QObject(parent), m_code(
		"cbuffer "+ name + "\n{\n")
	{
	}

	ShaderBuffer::ShaderBuffer(QObject *parent, VarGroup group)
	{
		
	}

	void ShaderBuffer::AddVariable(int ID, const QString& type, const QString& name)
	{
		m_IDs.push_back(ID);
		m_code += '\t' + type + '\t' + name + '\n';
	}

	void ShaderBuffer::AddVariable(const MaterialVarInfo& var)
	{
		m_IDs.push_back(var.ID);
		m_code += '\t' + NormalizeType(var.type) + '\t' + var.name + ";\n";
	}

	QString ShaderBuffer::ToCode()
	{
		QString res(m_code);
		res += "};\n\n";
		return res;
	}

	int ShaderBuffer::CodeSize() const
	{
		return m_code.size() + 4;
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

	ShaderBuffer::~ShaderBuffer()
	{

	}
}
