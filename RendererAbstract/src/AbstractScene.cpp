////////////////////////////////////////////////////////////////////////////////
// Filename: AbstractScene.cpp
////////////////////////////////////////////////////////////////////////////////
#include "AbstractScene.h"
#include "RenderObject.h"
#include "ShaderPool.h"
#include "HUDElement.h"
#include "MaterialInterface.h"

#include <Promise.h>
#include <Keys.h>
#include <IInternalInput.h>
#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>
#include <ApproxStatistics.h>
#include <IApproxShaderLabExternalControl.h>
#include <EventEmitter.h>
#include <ApproxMath.h>
#include <IObjectFactory.h>

using namespace std;
using namespace MatInterface;
using namespace Math;

AbstractScene::AbstractScene(): m_Camera(nullptr), xpos(0.0f), zpos(0.0f), m_light(nullptr), rotation(0),
rotationX(0), m_ModelLoadingThread(nullptr), trigger(false),
m_StatDisplay(nullptr), m_numberOfObjects(0), projection_is_valid(false), orthographic_is_valid(false)
{
}


AbstractScene::AbstractScene(const AbstractScene& other)
{
}


AbstractScene::~AbstractScene()
{
}


bool AbstractScene::Initialize(float screen_width, float screen_height)
{
	octree = new Octree(-SCREEN_DEPTH, SCREEN_DEPTH, -SCREEN_DEPTH, SCREEN_DEPTH, -SCREEN_DEPTH, SCREEN_DEPTH);
	octree->Subdivide(4);

    fov = pi / 4.0f;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
	projectionMatrix = APerspective(fov.load(), screen_width / screen_height, SCREEN_NEAR, SCREEN_DEPTH);
    projection_is_valid = true;

    orthographicMatrix = AOrthogonal(.0f, screen_width, screen_height, .0f, SCREEN_NEAR, SCREEN_DEPTH);
    orthographic_is_valid = true;

    if (!SetupEnvironment())
    {
        return false;
    }
    
    m_StatDisplay = new StatisticsDisplay;
    m_StatDisplay->Initialize();

	return true;
}

void AbstractScene::SetFov(float fov) {
    this->fov = fov;
    projection_is_valid = false;
}

void AbstractScene::SetScreenWidth(float width) {
    screen_width = width;
    projection_is_valid = false;
    orthographic_is_valid = false;
}

void AbstractScene::SetScreenHeight(float height) {
    screen_height = height;
    projection_is_valid = false;
    orthographic_is_valid = false;
}

RenderObjectPromise AbstractScene::AddRenderObject(const std::string &modelFileName, const ShaderDesc &type,
                                                   const Math::AVector3 &position, const Math::AVector3 &orientation)
{
    if (m_ModelLoadingThread)
    {
        if (m_ModelLoadingThread->joinable())
        {
            m_ModelLoadingThread->join();
        }
        delete m_ModelLoadingThread;
    }

    auto promise_pair = make_promise<IExternalRenderObject*>();
    m_ModelLoadingThread = new thread(&AbstractScene::LoadThread, this, promise_pair.second, modelFileName, position, orientation);
    return move(promise_pair.first);
}

bool AbstractScene::LoadThread(Promise<IExternalRenderObject *>::Fulfill_ptr_t fulfill, const string &modelFileName,
                               const Math::AVector3 &position, const Math::AVector3 &orientation)
{
    try {
        auto newRenderObject = new RenderObject(modelFileName, "", position, orientation);
        (*fulfill)(newRenderObject);
        m_NewRenderObjects.push(move(newRenderObject));
    }
    catch (approx_exception &e) {
        approx_exception ex("Could not initialize RenderObject");
        ex+=e;
        ex();
        return false;               
    }
    return true;
}

void AbstractScene::UpdateObjects()
{
    while(!m_NewRenderObjects.empty())
    {
        IInternalRenderObject* object = m_NewRenderObjects.front();
        object->set_id(m_numberOfObjects++);
        octree->add_object(object);
        render_queue_3d.add_object(object);
        m_NewRenderObjects.pop();
    }
}

bool AbstractScene::SetupEnvironment()
{
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

    m_light->set_diffuse_color({0.8, 0.8, 0.8, 1});
    m_light->set_ambient_color({0.15f, 0.15f, 0.15f, 1.0f});
    m_light->set_direction({1, 0, 1});

	// Set the initial position of the camera.
    m_Camera->set_position(1.0f, 0.0f, 0.0f);
    m_Camera->set_rotation(0, 0, 0);

	if(!projection_is_valid){
        projectionMatrix = ATranspose(APerspective(fov.load(), screen_width / screen_height, SCREEN_NEAR, SCREEN_DEPTH));
        projection_is_valid = true;
    }

	m_materialInterface.sceneConstants.projectionMatrix = projectionMatrix;
	m_materialInterface.sceneConstants.diffuseColor = m_light->GetDiffuseColor();
	m_materialInterface.sceneConstants.ambientColor = m_light->GetAmbientColor();
	m_materialInterface.sceneConstants.lightDirection = m_light->GetDirection();

    ShaderPool::instance().UpdateSceneConstantsBuffersForAll();

    return true;
}

void AbstractScene::Shutdown()
{
    octree->Shutdown();

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
	return;
}


bool AbstractScene::Frame()
{
	// Update the rotation variable each frame.
	
//    if (m_Input->IsKeyDown(W_Key))
//    {
//        m_Camera->MoveLocal(0, 0, 0.125f);
//    }
//    if (m_Input->IsKeyDown(S_Key))
//    {
//        m_Camera->MoveLocal(0, 0, -0.125f);
//    }
//    if (m_Input->IsKeyDown(A_Key))
//    {
//        m_Camera->MoveLocal(0.125f, 0, 0);
//    }
//    if (m_Input->IsKeyDown(D_Key))
//    {
//        m_Camera->MoveLocal(-0.125f, 0, 0);
//    }
//
//    if (m_Input->IsKeyDown(RM_Key))
//	{
//		float x, y;
//        m_Input->GetMouseCoord(x, y);
//        m_Camera->set_rotation(-(y - 300) / 10, -(x - 400) / 10, 0);
//	}
//    if (m_Input->IsIncrementalMouseControl())
//    {
//        int x, y;
//        m_Input->GetMouseDelta(x, y);
//        m_Camera->Rotate(static_cast<float>(x) / 100, static_cast<float>(y) / 100, 0);
//    }
    
    UpdateObjects();

	return Render();
}

IExternalRenderObject* AbstractScene::GetObjectByID(unsigned int ID)
{
    /*if (m_RenderObjects.size()>ID)
    {
        return dynamic_cast<IExternalRenderObject*>(m_RenderObjects[ID]);
    }*/
    return nullptr;
}



void AbstractScene::GetCameraPos(float &x, float &y, float &z)
{
    AVector3 tmppos = m_Camera->GetPosition();
    x = tmppos.x;
    y = tmppos.y;
    z = tmppos.z;
}

bool AbstractScene::Render()
{
    auto graphics_api = getObjectFactory()->getGraphicsAPI();

	// Clear the buffers to begin the scene.
	graphics_api->begin_scene({0.0f, 0.0f, 0.0f, 1.0f});

    m_Camera->Render();
    
    if(!orthographic_is_valid){
        orthographicMatrix = AOrthogonal(.0f, screen_width.load(), screen_height.load(), .0f, SCREEN_NEAR, SCREEN_DEPTH);
        orthographic_is_valid = true;
    }

    auto viewMatrix = m_Camera->GetViewMatrix();
    

	m_materialInterface.perFrameData.viewMatrix = ATranspose(viewMatrix);   //FIXME: deal with all unconditional transposes
	m_materialInterface.perFrameData.cameraPos = m_Camera->GetPosition();

    ShaderPool::instance().UpdatePerFrameBuffersForAll();

    octree->show_visible(m_materialInterface);

    render_queue_3d.render(m_materialInterface);

    graphics_api->set_zbuffer_enabled(false);
    graphics_api->set_alpha_blending_enabled(true);
    m_materialInterface.perFrameData.viewMatrix = ATranspose(orthographicMatrix);

    render_queue_2d.render(m_materialInterface);

    //m_StatDisplay->render();
    g_Statistics->ResDrawCalls();
    graphics_api->set_alpha_blending_enabled(false);
    graphics_api->set_zbuffer_enabled(true);
    
	// Present the rendered scene to the screen.
	graphics_api->end_scene();

	return true;
}

void AbstractScene::SetASLInterface(ASL::IApproxShaderLabExternalControl* ctrl)
{
    m_asl = ctrl;
	m_asl->SetMaterialInterfaceInfo(g_materialInterfaceManager->GetMaterialInterfaceInfo());
}