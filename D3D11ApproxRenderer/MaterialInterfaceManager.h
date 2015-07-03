#pragma once
#include <vector>
using namespace std;

namespace MatInterface
{
	__interface IMaterialVariable;

	class MaterialInterfaceManager
	{
		vector <IMaterialVariable*> m_materialVariables;
		string m_ver;

		MaterialInterfaceManager();
		~MaterialInterfaceManager();
	public:
		void RegisterVariable(IMaterialVariable*);
		const vector <IMaterialVariable*>& GetMaterialVariables()const;
		const vector <void*>  GetMaterialVariablesPtr()const;
		const vector <void*>  GetMaterialVariablesPtr(vector<int> &const IDs)const;

		static MaterialInterfaceManager* Instance();
	};

	extern MaterialInterfaceManager* g_materialInterfaceManager;
}