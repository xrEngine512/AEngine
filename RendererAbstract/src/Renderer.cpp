//
// Created by islam on 17.02.17.
//

#include "AbstractScene.h"
#include "ShaderPool.h"
#include "IObjectFactory.h"
#include "Renderer.h"

#include <IInternalInput.h>

using namespace std;

using ExitCode::DONE;
using ExitCode::RUNNING;

Renderer::Renderer(): m_Scene(nullptr), m_SoundSys(nullptr), m_done(false) {

}

Renderer::~Renderer()
{
    m_done.store(true);

    if (m_Scene)
    {
        m_Scene->Shutdown();
        delete m_Scene;
        m_Scene = nullptr;
    }
}

vector<ShaderDesc> Renderer::AvailableShadersDesc() const
{
    return ShaderPool::instance().LoadedShadersDescs();
}

Activity Renderer::get_activity() {
    return bind(&Renderer::activity, this);
}

bool Renderer::initialize(const RendererParameters &p)
{
    getObjectFactory()->getGraphicsAPI()->initialize(p.screen_width, p.screen_height, p.vSync, p.viewport, p.full_screen);

    ShaderPool::instance().Load("../Shaders");
    m_Scene = new AbstractScene();

    auto error_code = m_Scene->Initialize(p.screen_width, p.screen_height);

    if (error_code)
        add_state(ActivityState::INITIALIZED);
    else
        shutdown();

    return error_code;
}

ExitCode Renderer::activity()
{
    if (!m_done.load())
    {
        if (!Frame())
            m_done.store(true);

        return RUNNING;
    }

    return DONE;
}

bool Renderer::Frame()
{
    bool result;

    result =  m_Scene->Frame();
    if (m_SoundSys)
        m_SoundSys->Update();

    return result;
}

void Renderer::BindSoundSystem(IInternalSound* s_sys)
{
    m_SoundSys = s_sys;
}

IExternalScene* Renderer::get_scene()const
{
    return m_Scene;
}

void Renderer::shutdown() {
    m_done = true;
}

