#pragma once
#include <IExternalRenderObject.h>
class ApproxGameObject
{
    IExternalRenderObject* m_rObject;
public:
    ApproxGameObject();
    void Initialize(IExternalRenderObject*);
    virtual ~ApproxGameObject();
};

