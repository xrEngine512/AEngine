#pragma once
#include <vector>

__interface ISceneProperty;

using namespace std;

class SceneObject
{
protected:
    vector<ISceneProperty*> m_properties;

    void AddProperty(ISceneProperty*);
public:
    SceneObject();
    vector<ISceneProperty*> Properties();
    ~SceneObject();
};

