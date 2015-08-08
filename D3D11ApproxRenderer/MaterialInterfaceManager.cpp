#include "MaterialInterfaceManager.h"
#include "MaterialVariable.h"

namespace MatInterface
{
	MaterialInterfaceManager::MaterialInterfaceManager():m_ver("ver. 0.0.1")
	{
	}

	MaterialInterfaceManager* g_materialInterfaceManager = MaterialInterfaceManager::Instance();

	void MaterialInterfaceManager::RegisterVariable(IMaterialVariable* var)
	{
		for (auto variable : m_materialVariables)
		{
			if (var->ID() == variable->ID())
				return;
		}
		m_materialVariables.push_back(var);
	}

	MaterialInterfaceInfo MaterialInterfaceManager::GetMaterialInterfaceInfo()const
	{
		MaterialInterfaceInfo info(m_materialVariables,m_ver);
		return info;
	}

	vector<pair<void*, ShaderSystem::GenericType>> MaterialInterfaceManager::GetMaterialVariablesPtr()const
	{
		vector<pair<void*, ShaderSystem::GenericType>> res;
		res.reserve(m_materialVariables.size());
		for (auto prop : m_materialVariables)
		{
			res.push_back(pair<void*, ShaderSystem::GenericType>(prop->Ptr(), NormalizeType(prop->Type())));
		}
		return res;
	}

	vector<pair<void*, ShaderSystem::GenericType>> MaterialInterfaceManager::GetMaterialVariablesPtr(const vector<int>& IDs) const
	{
		vector<pair<void*, ShaderSystem::GenericType>> res;
		res.reserve(IDs.size());
		for (auto id : IDs)
		{
			for (auto prop : m_materialVariables)
			{
				if (prop->ID() == id)
				{
					res.push_back(pair<void*, ShaderSystem::GenericType>(prop->Ptr(), NormalizeType(prop->Type())));
				}
			}
		}
		return res;
	}

	ShaderSystem::GenericType MaterialInterfaceManager::NormalizeType(const string& type)
	{
		if (type.compare("float") == 0)
		{
			return ShaderSystem::FLOAT;
		}
		if (type.compare("struct DirectX::XMFLOAT4") == 0 || type.compare("float4") == 0)
		{
			return ShaderSystem::FLOAT4;
		}
		if (type.compare("struct DirectX::XMFLOAT3") == 0 || type.compare("float3") == 0)
		{
			return ShaderSystem::FLOAT3;
		}
		if (type.compare("struct DirectX::XMFLOAT2") == 0 || type.compare("float2") == 0)
		{
			return ShaderSystem::FLOAT2;
		}
		if (type.compare("struct DirectX::XMMATRIX") == 0 || type.compare("matrix") == 0)
		{
			return ShaderSystem::MATRIX;
		}
		throw exception("Unsupported type during NormalizeType");
	}

	MaterialInterfaceManager* MaterialInterfaceManager::Instance()
	{
		static MaterialInterfaceManager singleton;
		return &singleton;
	}

	MaterialInterfaceManager::~MaterialInterfaceManager()
	{
	}
}