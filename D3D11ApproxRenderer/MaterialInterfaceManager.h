#pragma once
#include <vector>
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
		pair<vector <void*>, vector <string> > GetMaterialVariablesPtr()const;
		pair<vector <void*>, vector <string> >  GetMaterialVariablesPtr(const vector<int> &const IDs)const;
		static string NormalizeType(const string& type);
		static MaterialInterfaceManager* Instance();
	};

	extern MaterialInterfaceManager* g_materialInterfaceManager;
}