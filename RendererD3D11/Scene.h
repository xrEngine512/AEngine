////////////////////////////////////////////////////////////////////////////////
// Filename: AbstractScene.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "IExternalScene.h"
#include <windows.h>
#include "../../RendererD3D11/D3D11System.h"
#include "Camera.h"
#include "Light.h"
#include "SceneGraphNode.h"
#include "IInternalHUDElement.h"
#include <IExternalInput.h>
#include <queue>
#include <thread>
#include "HUD2D.h"
#include "StatisticsDisplay.h"
#include "MaterialInterface.h"

struct IInternalRenderObject;


/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: AbstractScene
////////////////////////////////////////////////////////////////////////////////
class Scene : public IExternalScene
{
	D3D11* m_D3D;
	Camera* m_Camera;
	Light *m_light;
    SceneGraphNode* m_SceneGraphRoot;
    std::queue<IInternalRenderObject*> m_NewRenderObjects;
	MatInterface::MaterialInterface m_materialInterface;

	WindowHandle m_hwnd;
    unsigned int m_numberOfObjects;
	float xpos, zpos ;
	float rotation, rotationX;
	bool trigger;
    IExternalInput* m_Input;
    std::thread *m_ModelLoadingThread;
    IInternalHUDElement *m_HUD;
    HUD2D *m_HUD2D;
    StatisticsDisplay *m_StatDisplay;
    ASL::IApproxShaderLabExternalControl *m_asl;

    bool LoadThread(IInternalRenderObject* newRenderObject, const std::string& modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ);
    bool Render();
    void UpdateObjects();
    bool SetupEnvironment();
public:
	AbstractScene();
	AbstractScene(const AbstractScene&);
	virtual ~AbstractScene();

    void GetCameraPos(float &x, float &y, float &z) override;
	bool Initialize(WindowHandle, int, int, bool);
	void Shutdown();
	bool Frame();
    IExternalRenderObject* AddRenderObject(const char *modelFileName, const ShaderDesc &type, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)override;
    IExternalRenderObject* GetObjectByID(unsigned int ID)override;
    void SetASLInterface(ASL::IApproxShaderLabExternalControl*)override;
};