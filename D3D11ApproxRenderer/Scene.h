////////////////////////////////////////////////////////////////////////////////
// Filename: Scene.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "IExternalScene.h"
#include <windows.h>
#include "D3D11System.h"
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

using namespace std;
/////////////
// GLOBALS //
/////////////
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: Scene
////////////////////////////////////////////////////////////////////////////////
class Scene : public IExternalScene
{
	D3D11* m_D3D;
	Camera* m_Camera;
	Light *m_light;
    SceneGraphNode* m_SceneGraphRoot;
    queue<IInternalRenderObject*> m_NewRenderObjects;
	MatInterface::MaterialInterface m_materialInterface;

	HWND m_hwnd;
    unsigned int m_numberOfObjects;
	float xpos, zpos ;
	float rotation, rotationX;
	bool trigger;
    IExternalInput* m_Input;
    thread *m_ModelLoadingThread;
    IInternalHUDElement *m_HUD;
    HUD2D *m_HUD2D;
    StatisticsDisplay *m_StatDisplay;
    IApproxShaderLabExternalControl *m_asl;

    bool LoadThread(IInternalRenderObject* newRenderObject, char *modelFileName, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ);
    bool Render();
    void UpdateObjects();
    bool SetupEnvironment();
public:
	Scene();
	Scene(const Scene&);
	virtual ~Scene();

    void GetCameraPos(float &x, float &y, float &z) override;
	bool Initialize(HWND, int, int, bool);
	void Shutdown();
	bool Frame();
    IExternalRenderObject* AddRenderObject(char *modelFileName, ShaderType type, float PosX, float PosY, float PosZ, float RotX, float RotY, float RotZ)override;
    IExternalRenderObject* GetObjectByID(unsigned int ID)override;
    void SetASLInterface(IApproxShaderLabExternalControl*)override;
};