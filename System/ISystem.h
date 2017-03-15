#pragma once
#include <IExternalRenderer.h>
#include <IExternalSound.h>
#include <X11/X.h>

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

interface ISystem
{
    virtual bool initialize(XID ParentWindowID, bool FullScreenOn = false, int RendererWindowWidth = 800,
                            int RendererWindowHeight = 600, int RendererWindowPosX = 0, int RendererWindowPosY = 0) = 0;
    virtual void shutdown() = 0;
    virtual void Run() = 0;
    virtual IExternalRenderer* GetRendererEx() = 0;
    virtual IExternalSound* GetSoundSystemEx() = 0;
    virtual SystemState State() const = 0;
    virtual ~ISystem(){};
};

APPROX_API ISystem* GetSystem();

