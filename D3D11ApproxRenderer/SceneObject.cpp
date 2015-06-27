#include "SceneObject.h"


void SceneObject::AddProperty(ISceneProperty* prop)
{
    m_properties.push_back(prop);
}

SceneObject::SceneObject()
{
}

vector<ISceneProperty*> SceneObject::Properties()
{
    return m_properties;
}

SceneObject::~SceneObject()
{
}
