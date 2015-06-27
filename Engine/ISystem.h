#pragma once
#include <IExternalRenderer.h>
#include <IExternalSound.h>

enum SubSystemState
{
    SYS_NOT_INITIALIZED, SYS_INITIALIZED, SYS_RUNNING
};

struct SystemState
{
    SubSystemState Renderer = SYS_NOT_INITIALIZED;
    SubSystemState Sound = SYS_NOT_INITIALIZED;
    SubSystemState ThreadPool = SYS_NOT_INITIALIZED;
    SubSystemState Input = SYS_NOT_INITIALIZED;
};

__interface ISystem
{
    bool Initialize(void* ParentWindowID,bool FullScreenOn = false,  int RendererWindowWidth = 800, int RendererWindowHeight = 600, int RendererWindowPosX = 0, int RendererWindowPosY = 0);
	void Shutdown();
	void Run();
    IExternalRenderer* GetRendererEx();
    IExternalSound* GetSoundSystemEx();
    SystemState State()const;
};

extern _declspec(dllexport) ISystem* GetSystem();

