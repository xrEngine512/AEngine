////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
#include <ApproxSystemTools.h>
#include <RendererSystem.h>
#include <IInternalInput.h>
#include <IExternalSound.h>

SystemClass::SystemClass() :m_Renderer(nullptr), m_SoundSystem(nullptr)
{
    
    m_ThreadPool = new SystemThreadPool;
}


SystemClass::~SystemClass()
{
}


SystemClass& SystemClass::Instance()
{
    static SystemClass instance;
    return instance;
}

SystemClass* SystemSingleton = &SystemClass::Instance();

bool SystemClass::Initialize(void* ParentWindowID, bool FullScreenOn = false, int RendererWindowWidth = 800, int RendererWindowHeight = 600, int RendererWindowPosX = 0, int RendererWindowPosY = 0)
{
	bool result;

    m_Renderer = GetRenderer();
    m_SoundSystem = GetSoundSystem();

    m_SoundSystem->Initialize();
    m_State.Sound = SYS_INITIALIZED;
	// Initialize the input object.
	GetInput()->Initialize();
    m_State.Input = SYS_INITIALIZED;
    m_ThreadPool->Initialize();
    m_State.ThreadPool = SYS_INITIALIZED;
	// Create the graphics object.  This object will handle rendering all the graphics for this application.
    
    if (!m_Renderer)
	{
		return false;
	}

	// Initialize the graphics object.
    result = m_Renderer->Initialize(static_cast<HWND>(ParentWindowID), FullScreenOn, RendererWindowWidth, RendererWindowHeight, RendererWindowPosX, RendererWindowPosY);
	if(!result)
	{
		return false;
	}
    m_State.Renderer = SYS_INITIALIZED;
    m_Renderer->BindSoundSystem(m_SoundSystem);
	return true;
}

IExternalRenderer* SystemClass::GetRendererEx()
{
	return dynamic_cast<IExternalRenderer*>(m_Renderer);
}

SystemState SystemClass::State() const
{
    return m_State;
}

IExternalSound* SystemClass::GetSoundSystemEx()
{
    return dynamic_cast<IExternalSound*>(m_SoundSystem);
}

void SystemClass::Shutdown()
{
    DELETE_SYS_OBJECT(m_ThreadPool);
	m_State.ThreadPool = SYS_NOT_INITIALIZED;
	if (m_State.Renderer >= SYS_INITIALIZED)
		m_Renderer->Shutdown();
	m_State.Renderer = SYS_NOT_INITIALIZED;
	m_State.Input = SYS_INITIALIZED;
}


void SystemClass::Run()
{
    m_ThreadPool->AddTask(m_Renderer->GetThreadFunc(),true);
    m_ThreadPool->AddTask(GetInput()->GetThreadFunc());
    m_ThreadPool->Run();
    m_State.Renderer = SYS_RUNNING;
    m_State.ThreadPool = SYS_RUNNING;
	m_State.Input = SYS_RUNNING;
    //dynamic_cast<IExternalSound*> (m_SoundSystem)->PlayRain();
	return;
}