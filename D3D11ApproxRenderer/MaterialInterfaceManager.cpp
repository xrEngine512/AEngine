#include "MaterialInterfaceManager.h"
#include "SceneObject.h"
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

	pair<vector <void*>, vector <string> > MaterialInterfaceManager::GetMaterialVariablesPtr()const
	{
		vector<void*> resPtrs;
		vector<string> resTypes;
		resPtrs.reserve(m_materialVariables.size());
		resTypes.reserve(m_materialVariables.size());
		for (auto prop : m_materialVariables)
		{
			resPtrs.push_back(prop->Ptr());
			resTypes.push_back(NormalizeType(prop->Type()));
		}
		return pair<vector <void*>, vector <string> >(resPtrs,resTypes);
	}

	pair<vector <void*>, vector <string> > MaterialInterfaceManager::GetMaterialVariablesPtr(const vector<int>& IDs) const
	{
		vector<void*> resPtrs;
		vector<string> resTypes;
		resPtrs.reserve(IDs.size());
		resTypes.reserve(IDs.size());
		for (auto id : IDs)
		{
			for (auto prop : m_materialVariables)
			{
				if (prop->ID() == id)
				{
					resPtrs.push_back(prop->Ptr());
					resTypes.push_back(NormalizeType(prop->Type()));
				}
			}
		}
		return pair<vector <void*>, vector <string> >(resPtrs, resTypes);
	}

	string MaterialInterfaceManager::NormalizeType(const string& type)
	{
		if (type.compare("float") == 0)
		{
			return "float";
		}
		if (type.compare("struct DirectX::XMFLOAT4") == 0)
		{
			return "XMFLOAT4";
		}
		if (type.compare("struct DirectX::XMFLOAT3") == 0)
		{
			return "XMFLOAT3";
		}
		if (type.compare("struct DirectX::XMFLOAT2") == 0)
		{
			return "XMFLOAT2";
		}
		if (type.compare("struct DirectX::XMMATRIX") == 0)
		{
			return "XMMATRIX";
		}
		if (type.compare("struct DirectX::XMVECTOR") == 0)
		{
			return "XMVECTOR";
		}
		return nullptr;
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