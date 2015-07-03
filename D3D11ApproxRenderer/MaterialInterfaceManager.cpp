#include "MaterialInterfaceManager.h"
#include "SceneObject.h"
#include "MaterialVariable.h"

namespace MatInterface
{
	MaterialInterfaceManager::MaterialInterfaceManager()
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

	const vector<IMaterialVariable*>& MaterialInterfaceManager::GetMaterialVariables()const
	{
		return m_materialVariables;
	}

	const vector<void*> MaterialInterfaceManager::GetMaterialVariablesPtr()const
	{
		vector<void*> res;
		res.reserve(m_materialVariables.size());
		for (auto prop : m_materialVariables)
		{
			res.push_back(prop->Ptr());
		}
		return res;
	}

	const vector<void*> MaterialInterfaceManager::GetMaterialVariablesPtr(vector<int>& IDs) const
	{
		vector<void*> res;
		res.reserve(IDs.size());
		for (auto prop : m_materialVariables)
		{
			for (auto id : IDs)
			{
				if (prop->ID() == id)
				{
					res.push_back(prop->Ptr());
				}
			}
		}
		return res;
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