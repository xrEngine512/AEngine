////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Scene.h"
#include "RenderObject.h"
#include "HUDElement.h"
#include <Keys.h>
#include <IInternalInput.h>
#include <ApproxSystemTools.h>
#include <string>
#include "ShaderPool.h"
#include <ApproxSystemErrors.h>
#include <ApproxStatistics.h>
#include "MaterialInterface.h"
#include <IApproxShaderLabExternalControl.h>
#include <EventEmitter.h>

using namespace std;
using namespace MatInterface;

Scene::Scene() :m_D3D(nullptr), m_Camera(nullptr), xpos(0.0f), zpos(0.0f), m_light(nullptr), rotation(0),
rotationX(0), m_ModelLoadingThread(nullptr), m_HUD(nullptr), m_HUD2D(nullptr), trigger(false), 
m_StatDisplay(nullptr), m_numberOfObjects(0)
{
    m_Input = dynamic_cast<IExternalInput*>(GetInput());

	m_Input->BindToKey(M_Key) += [=](void){ m_HUD2D->ShowMessage("Message", 300, 100); };
}


Scene::Scene(const Scene& other)
{
}


Scene::~Scene()
{
}


bool Scene::Initialize(WindowHandle hwnd, int screenWidth, int screenHeight, bool FullScreen)
{
	bool result;
	m_hwnd = hwnd;

	// Create the Direct3D object.
	m_D3D = new D3D11;
	if (!m_D3D)
		return false;

	m_SceneGraphRoot = new SceneGraphNode(-SCREEN_DEPTH, SCREEN_DEPTH, -SCREEN_DEPTH, SCREEN_DEPTH, -SCREEN_DEPTH, SCREEN_DEPTH);
	m_SceneGraphRoot->Subdivide(4);

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FullScreen, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	
	g_ShaderPool = new ShaderPool(m_D3D->GetDevice(),L"../",m_hwnd);
	

#ifdef RENDER_OCTREE
    m_SceneGraphRoot->PrepareToRender(m_D3D->GetDevice());
#endif
	
	
    if (!SetupEnvironment())
    {
        return false;
    }




    /*m_HUD = new HUDElement;
    if (!m_HUD)
    {
        return false;
    }*/

   /* IInternalRenderObject* testObject = new RenderObject(m_D3D->GetDevice(),UNIFIED_SHADER);
    try
    {
        testObject->Initialize(m_D3D->GetDevice(), "D:/BackUpBecauseofShitHappens/Users/Islam/Desktop/DX/Core/Engine/Resources/Meshes/test01.obj", 0, 0, 0, 0, 0, 0);
        dynamic_cast<IExternalRenderObject*>(testObject)->GetMaterial()->LoadTexture(L"D:/BackUpBecauseofShitHappens/Users/Islam/Desktop/DX/Core/Engine/Resources/Textures/35x35.dds", 0);
    }
    catch (ApproxException e)
    {
        e(m_hwnd);
    }
    m_SceneGraphRoot->AddObject(testObject);*/
    
    /*result = m_HUD->Initialize(m_D3D->GetDevice(),hwnd,const_cast<wchar_t*>((wstring(TEXTURES_DIR) + wstring(L"testHUD.dds")).c_str()),128,128);
    dynamic_cast<IExternalHUDElement*>(m_HUD)->SetCoord(-150, -100);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
        return false;
    }*/

    m_HUD2D = new HUD2D;
    if (!m_HUD2D)
    {
        return false;
    }

    m_HUD2D->Initialize(m_D3D->GetDevice());
    
    m_StatDisplay = new StatisticsDisplay;
    m_StatDisplay->Initialize(m_D3D->GetDevice());
    
   

	return true;
}

IExternalRenderObject* Scene::AddRenderObject(const char *modelFileName, const ShaderDesc& type, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)
{
    if (m_ModelLoadingThread)
    {
        if (m_ModelLoadingThread->joinable())
        {
            m_ModelLoadingThread->join();
        }
        delete m_ModelLoadingThread;
    }
    IInternalRenderObject* newRenderObject = new RenderObject(m_D3D->GetDevice(),type);
    
    m_ModelLoadingThread = new thread(&Scene::LoadThread, this, newRenderObject,modelFileName, PosX, PosY, PosZ, RotX, RotY, RotZ);
    return dynamic_cast<IExternalRenderObject*> (newRenderObject);
}

bool Scene::LoadThread(IInternalRenderObject* newRenderObject, const string& modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)
{
    try
    {
        newRenderObject->Initialize(m_D3D->GetDevice(), modelFileName, PosX, PosY, PosZ, RotX, RotY, RotZ);
    }
    catch (ApproxException &e)
    {
        ApproxException ex("Could not initialize RenderObject");
        ex+=e;
        ex();
        delete newRenderObject;
        return false;               
    }
    m_NewRenderObjects.push(newRenderObject);
    return true;
}

void Scene::UpdateObjects()
{
    while(!m_NewRenderObjects.empty())
    {
        IInternalRenderObject* object = m_NewRenderObjects.front();
        object->SetID(m_numberOfObjects++);
        m_SceneGraphRoot->AddObject(object);
        m_NewRenderObjects.pop();
    }
}

bool Scene::SetupEnvironment()
{
	AMatrix projectionMatrix;

    m_Camera = new Camera;
    if (!m_Camera)
    {
        return false;
    }

    m_light = new Light;
    if (!m_light)
    {
        return false;
    }    

	m_light->SetDiffuseColor(0.8, 0.8, 0.8, 1);
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDirection(1, 0, 1);

	// Set the initial position of the camera.
	m_Camera->SetPosition(1.0f, 0.0f, 0.0f);
	m_Camera->SetRotation(0, 0, 0);

	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_materialInterface.sceneConstants.projectionMatrix = XMMatrixTranspose(projectionMatrix);
	m_materialInterface.sceneConstants.diffuseColor = m_light->GetDiffuseColor();
	m_materialInterface.sceneConstants.ambientColor = m_light->GetAmbientColor();
	m_materialInterface.sceneConstants.lightDirection = m_light->GetDirection();

	g_ShaderPool->UpdateSceneConstantsBuffersForAll(m_D3D->GetDeviceContext());

    return true;
}

void Scene::Shutdown()
{
    m_SceneGraphRoot->Shutdown();

	DELETE_SYS_OBJECT(g_ShaderPool);

	if(m_light)
	{
		delete m_light;
		m_light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
    DELETE_SYS_OBJECT(m_D3D);
    DELETE_SYS_OBJECT(m_HUD);
	return;
}


bool Scene::Frame()
{
	bool result;
	// Update the rotation variable each frame.
	
    if (m_Input->IsKeyDown(W_Key))
    {
        m_Camera->MoveLocal(0, 0, 0.125f);
    }
    if (m_Input->IsKeyDown(S_Key))
    {
        m_Camera->MoveLocal(0, 0, -0.125f);
    }
    if (m_Input->IsKeyDown(A_Key))
    {
        m_Camera->MoveLocal(0.125f, 0, 0);
    }
    if (m_Input->IsKeyDown(D_Key))
    {
        m_Camera->MoveLocal(-0.125f, 0, 0);
    }

    /*if (m_Input->IsKeyDown(M_Key))
    {
        //if (!trigger)
            m_HUD2D->ShowMessage(L"Message", 300, 100);
        //trigger = true;
    }*/
    if (m_Input->IsKeyDown(VK_RBUTTON))
	{	
		float x, y;
        m_Input->GetMouseCoord(x, y);
		m_Camera->SetRotation(-(y-300) / 10, -(x-400 )/ 10, 0);
	}
    if (m_Input->IsIncrementalMouseControl())
    {
        int x, y;
        m_Input->GetMouseDelta(x, y);
        m_Camera->Rotate(static_cast<float>(x) / 100, static_cast<float>(y) / 100, 0);
    }
	//m_Camera->SetPosition(xpos, 0, zpos);
    
    UpdateObjects();

	result = Render();
	if(!result)
	{
		return false;
	}
	return true;
}

IExternalRenderObject* Scene::GetObjectByID(unsigned int ID)
{
    /*if (m_RenderObjects.size()>ID)
    {
        return dynamic_cast<IExternalRenderObject*>(m_RenderObjects[ID]);
    }*/
    return nullptr;
}



void Scene::GetCameraPos(float &x, float &y, float &z)
{
    AVector3 tmppos = m_Camera->GetPosition();
    x = tmppos.x;
    y = tmppos.y;
    z = tmppos.z;
}

bool Scene::Render()
{
	AMatrix viewMatrix, projectionMatrix, orthoMatrix;
    //BoundingFrustum *Frustum;

    
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    m_Camera->Render();
    
    m_D3D->GetOrthoMatrix(orthoMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    

	m_materialInterface.perFrameData.viewMatrix = XMMatrixTranspose(viewMatrix);
	m_materialInterface.perFrameData.cameraPos = m_Camera->GetPosition();

	g_ShaderPool->UpdatePerFrameBuffersForAll(m_D3D->GetDeviceContext());

	//m_D3D->SetWireframeFillMode();
    m_SceneGraphRoot->Render(m_D3D->GetDeviceContext(), m_materialInterface);
	//m_D3D->SetSolidFillMode();
	
    m_D3D->TurnZBufferOff();
    m_D3D->EnableAlphaBlending();
   // m_HUD->Render(m_D3D->GetDeviceContext(), orthoMatrix);
    m_HUD2D->Render();
    m_StatDisplay->Render();
    g_Statistics->ResDrawCalls();
    m_D3D->DisableAlphaBlending();
    m_D3D->TurnZBufferOn();
    
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void Scene::SetASLInterface(ASL::IApproxShaderLabExternalControl* ctrl)
{
    m_asl = ctrl;
	m_asl->SetMaterialInterfaceInfo(g_materialInterfaceManager->GetMaterialInterfaceInfo());
}