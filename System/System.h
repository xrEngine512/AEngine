////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include <X11/X.h>
#include "ISystem.h"
#include "SystemThreadPool.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: System
////////////////////////////////////////////////////////////////////////////////

class Renderer;

interface IInternalSound;
interface IExternalRenderer;
interface IExternalSound;


class System: public ISystem
{
public:
	System();
    virtual ~System();

    bool initialize(Window, bool, int, int, int, int)override;
    void shutdown()override;
    void Run()override;
    IExternalRenderer* GetRendererEx()override;
    IExternalSound* GetSoundSystemEx()override;
    SystemState State()const override;
    static System& instance();
private:
    SystemThreadPool *thread_pool;
    Renderer *renderer;
    IInternalSound   *sound_system;

    SystemState m_State;
};

extern System *SystemSingleton;

ISystem* GetSystem()
{
    return SystemSingleton;
}
