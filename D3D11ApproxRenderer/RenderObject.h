#pragma once


#include "Mesh.h"
#include "IInternalMaterial.h"
#include "IInternalRenderObject.h"
#include "IExternalRenderObject.h"
#include "AlignedObject.h"

class RenderObject : public IInternalRenderObject, public IExternalRenderObject
{	
	Mesh *m_Mesh, *m_MeshBoxRender;
    XMMATRIX m_worldMatrix;
    IInternalMaterial *m_Material, *m_MeshBoxMaterial;
    unsigned int m_ID;    
    XMFLOAT3    m_globalCoords,m_globalRotation;
    RenderObject(void);
public:
    bool Initialize(ID3D11Device *device, const char *modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ) override;
    bool Initialize(ID3D11Device *device, MeshBox*, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ) override;
	bool Render(ID3D11DeviceContext *deviceContext, MatInterface::MaterialInterface&)override;
    void SetCoord(float x, float y, float z)override;
    void SetRotation(float x, float y, float z)override;
    XMVECTOR GetPoint()override;
    XMFLOAT3 GetCoord()override;
    MeshBox GetMeshBox()override;
    unsigned int GetID()override;
    void SetID(unsigned int)override;
    IExternalMaterial* GetMaterial()override;
    void Shutdown()override;
    RenderObject(ID3D11Device* device, ShaderDesc type);
    virtual ~RenderObject(void);
};

