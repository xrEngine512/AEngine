////////////////////////////////////////////////////////////////////////////////
// Filename: AbstractScene.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

//////////////
// INCLUDES //
//////////////
#include "IExternalScene.h"
#include "Camera.h"
#include "Light.h"
#include "Octree.h"
#include <IExternalInput.h>
#include <queue>
#include <thread>
#include "HUD2D.h"
#include "StatisticsDisplay.h"
#include "MaterialInterface.h"
#include "RenderQueue.h"

struct IInternalRenderObject;


const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



class AbstractScene : public IExternalScene
{
	Camera* m_Camera;
	Light *m_light;
    Octree* octree;
    RenderQueue render_queue_3d, render_queue_2d;
    std::queue<IInternalRenderObject*> m_NewRenderObjects;
	MatInterface::MaterialInterface m_materialInterface;

    unsigned int m_numberOfObjects;
	float xpos, zpos ;
	float rotation, rotationX;
    std::atomic<float> fov, screen_width, screen_height;
	bool trigger;
    std::thread *m_ModelLoadingThread;
    StatisticsDisplay *m_StatDisplay;
    ASL::IApproxShaderLabExternalControl *m_asl;

    Math::AMatrix projectionMatrix, orthographicMatrix;
    std::atomic_bool projection_is_valid, orthographic_is_valid;

    bool LoadThread(Promise<IExternalRenderObject *>::Fulfill_ptr_t fulfill, const std::string &, const Math::AVector3 &,
                    const Math::AVector3 &);
    bool Render();
    void UpdateObjects();
    bool SetupEnvironment();
public:
	AbstractScene();
	AbstractScene(const AbstractScene&);
	virtual ~AbstractScene();

    void GetCameraPos(float &x, float &y, float &z) override;
    void SetFov(float) override;
    void SetScreenWidth(float) override;
    void SetScreenHeight(float) override;
	RenderObjectPromise AddRenderObject(const std::string &modelFileName, const ShaderDesc &type,
										const Math::AVector3 &position, const Math::AVector3 &orientation)override;
	IExternalRenderObject* GetObjectByID(unsigned int ID)override;
	void SetASLInterface(ASL::IApproxShaderLabExternalControl*)override;

	bool Initialize(float screen_width, float screen_height);
	void Shutdown();
	bool Frame();
};