////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "System.h"

#include <ApproxSystemTools.h>
#include <Renderer.h>
#include <IInternalInput.h>
#include <IObjectFactory.h>


System::System() :renderer(nullptr), sound_system(nullptr) {
    thread_pool = new SystemThreadPool;
}


System::~System() {
    if (renderer)
        renderer->shutdown();
}


System& System::instance() {
    static System instance;
    return instance;
}

System* SystemSingleton = &System::instance();

bool System::initialize(Window ParentWindowID, bool FullScreenOn = false, int RendererWindowWidth = 800,
                        int RendererWindowHeight = 600, int RendererWindowPosX = 0, int RendererWindowPosY = 0)
{
	bool result;

    renderer = GetRenderer();
    sound_system = GetSoundSystem();

    sound_system->Initialize();
    m_State.Sound = SYS_INITIALIZED;
	// initialize the input object.
	GetInput()->Initialize();
    m_State.Input = SYS_INITIALIZED;
    thread_pool->initialize();
    m_State.ThreadPool = SYS_INITIALIZED;
	// Create the graphics object.  This object will handle rendering all the graphics for this application.
    
    if (!renderer)
	{
		return false;
	}

	// initialize the graphics object.
    RendererParameters renderer_parameters {
        .full_screen    = FullScreenOn,
        .screen_width   = RendererWindowWidth,
        .screen_height  = RendererWindowHeight,
        .viewport       = ParentWindowID
    };
    result = renderer->initialize(renderer_parameters);
	if(!result) {
        shutdown();
		return false;
	}
    m_State.Renderer = SYS_INITIALIZED;
    renderer->BindSoundSystem(sound_system);
	return true;
}

IExternalRenderer* System::GetRendererEx()
{
	return dynamic_cast<IExternalRenderer*>(renderer);
}

SystemState System::State() const
{
    return m_State;
}

IExternalSound* System::GetSoundSystemEx()
{
    return dynamic_cast<IExternalSound*>(sound_system);
}

void System::shutdown()
{
    DELETE_SYS_OBJECT(thread_pool);
	m_State.ThreadPool = SYS_NOT_INITIALIZED;
	if (m_State.Renderer >= SYS_INITIALIZED)
		renderer->shutdown();
	m_State.Renderer = SYS_NOT_INITIALIZED;
	m_State.Input = SYS_INITIALIZED;

    getObjectFactory()->getGraphicsAPI()->shutdown();
}


void System::Run()
{
    thread_pool->add_activity(renderer->get_activity(), true);
    thread_pool->add_activity(GetInput()->get_activity());
    thread_pool->run();
    m_State.Renderer = SYS_RUNNING;
    m_State.ThreadPool = SYS_RUNNING;
	m_State.Input = SYS_RUNNING;
    //dynamic_cast<IExternalSound*> (m_SoundSystem)->PlayRain();
	return;
}