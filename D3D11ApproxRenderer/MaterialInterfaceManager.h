#pragma once
#include <vector>
class SceneObject;
__interface IMaterialVariable;

using namespace std;

class MaterialInterfaceManager
{
	vector <IMaterialVariable*> m_materialVariables;
    string m_ver;

	MaterialInterfaceManager();
	~MaterialInterfaceManager();
public:
    
    //void RegisterObject(SceneObject*);
	void RegisterVariable(IMaterialVariable*);
	const vector <IMaterialVariable*>& GetMaterialVariables()const;
	const vector <void*>  GetMaterialVariablesPtr()const;
    const vector <void*>  GetMaterialVariablesPtr(vector<int> &const IDs)const;
    
	static MaterialInterfaceManager* Instance();
};

extern MaterialInterfaceManager* g_materialInterfaceManager;