#include "RenderObject.h"
#include "Material.h"
#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>
#include "MaterialInterface.h"

using namespace MatInterface;

RenderObject::RenderObject(void) :m_Mesh(nullptr),m_Material(nullptr),  m_globalCoords(0, 0, 0), m_globalRotation(0,0,0)
{
    m_Mesh = new Mesh;
    //m_MeshBoxRender = new Mesh;
    m_Material = new Material;
    //m_MeshBoxMaterial = new Material;
}

RenderObject::RenderObject(ID3D11Device* device, ShaderDesc type) : RenderObject()
{
    try
    {
        m_Material->Initialize(device, type);
        //m_MeshBoxMaterial->Initialize(device, WIREFRAME_SHADER);
    }
    catch (ApproxException &reason)
    {
        throw ApproxException(L"Could not initialize the material object.", L"RenderObject").becauseOf(reason);
    }
}

bool RenderObject::Initialize(ID3D11Device *device, const char *modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)
{
    try
    {
        m_Mesh->Initialize(device, modelFileName, true);
        //m_MeshBoxRender->Initialize(device, &m_Mesh->GetMeshBox(), true);
    }
    catch (ApproxException &reason)
    {
        throw ApproxException(L"Could not initialize the model object.", L"RenderObject").becauseOf(reason);
    }    
    SetCoord(PosX, PosY, PosZ);
    SetRotation(RotX, RotY, RotZ);
    return true;
}

bool RenderObject::Initialize(ID3D11Device* device, MeshBox* box, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)
{
    try
    {
        m_Mesh->Initialize(device, box, true);
        DELETE_SYS_OBJECT(m_MeshBoxRender);
        DELETE_SYS_OBJECT(m_MeshBoxMaterial);
    }
    catch (ApproxException &reason)
    {
        throw ApproxException(L"Could not initialize the model object.", L"RenderObject").becauseOf(reason);
    }
    SetCoord(PosX, PosY, PosZ);
    SetRotation(RotX, RotY, RotZ);
    return true;
}

XMVECTOR RenderObject::GetPoint()
{
    return XMLoadFloat3(&m_globalCoords);
}

XMFLOAT3 RenderObject::GetCoord()
{
    return m_globalCoords;
}

MeshBox RenderObject::GetMeshBox()
{
    return m_Mesh->GetMeshBox();
}

unsigned RenderObject::GetID()
{
    return m_ID;
}

void RenderObject::SetID(unsigned ID)
{
    m_ID = ID;
}

IExternalMaterial* RenderObject::GetMaterial()
{
    return dynamic_cast<IExternalMaterial*>(m_Material);
}

void RenderObject::SetCoord(float x, float y, float z)
{
    m_globalCoords.x = x;
    m_globalCoords.y = y;
    m_globalCoords.z = z;
}

void RenderObject::SetRotation(float x, float y, float z)
{
    m_globalRotation.x = x;
    m_globalRotation.y = y;
    m_globalRotation.z = z;
}

bool RenderObject::Render(ID3D11DeviceContext *deviceContext, MaterialInterface& mi)
{
    mi.perObjectData.modelMatrix = XMMatrixTranspose(XMMatrixRotationX(m_globalRotation.x)*XMMatrixRotationY(m_globalRotation.y)*XMMatrixRotationZ(m_globalRotation.z)*XMMatrixTranslation(m_globalCoords.x, m_globalCoords.y, m_globalCoords.z));
	mi.CalculateMVPAfterTranspose();
	if (m_MeshBoxRender)
        m_MeshBoxRender->Render(deviceContext);
    if (m_MeshBoxMaterial)
		m_MeshBoxMaterial->Render(deviceContext, m_MeshBoxRender->GetIndexCount());

    m_Mesh->Render(deviceContext);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    
	return m_Material->Render(deviceContext, m_Mesh->GetIndexCount());
}

void RenderObject::Shutdown()
{
    DELETE_SYS_OBJECT(m_Material);
    DELETE_SYS_OBJECT(m_Mesh);
    DELETE_SYS_OBJECT(m_MeshBoxRender);
    DELETE_SYS_OBJECT(m_MeshBoxMaterial);
}



RenderObject::~RenderObject(void)
{
    RenderObject::Shutdown();
}
