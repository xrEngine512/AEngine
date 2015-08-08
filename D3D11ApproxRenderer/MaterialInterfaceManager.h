#pragma once
#include <vector>
#include "MemoryMultiplexer.h"
using namespace std;

namespace MatInterface
{
	__interface IMaterialVariable;
	struct MaterialInterfaceInfo
	{
		MaterialInterfaceInfo(const vector <IMaterialVariable*>& vars, const string& ver) :Vars(vars), Ver(ver)
		{}
		const vector <IMaterialVariable*>& Vars;
		const string& Ver;
	};
	class MaterialInterfaceManager
	{
		vector <IMaterialVariable*> m_materialVariables;
		string m_ver;

		MaterialInterfaceManager();
		~MaterialInterfaceManager();
	public:
		
		void RegisterVariable(IMaterialVariable*);
		MaterialInterfaceInfo GetMaterialInterfaceInfo()const;
		vector<pair<void*, ShaderSystem::GenericType>> GetMaterialVariablesPtr()const;
		vector<pair<void*, ShaderSystem::GenericType>> GetMaterialVariablesPtr(const vector<int> &const IDs)const;
		static ShaderSystem::GenericType NormalizeType(const string& type);
		static MaterialInterfaceManager* Instance();
	};

	extern MaterialInterfaceManager* g_materialInterfaceManager;
}