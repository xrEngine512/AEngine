////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "ISystem.h"
#include "SystemThreadPool.h"
#include <IInternalRenderer.h>
#include <IInternalSound.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////


class SystemClass: public ISystem
{
public:
	SystemClass();
    virtual ~SystemClass();

    bool Initialize(void* , bool, int , int , int , int )override;
    void Shutdown()override;
    void Run()override;
    IExternalRenderer* GetRendererEx()override;
    IExternalSound* GetSoundSystemEx()override;
    SystemState State()const override;
    static SystemClass& Instance();
private:
    SystemThreadPool *m_ThreadPool;
    IInternalRenderer *m_Renderer;
    IInternalSound* m_SoundSystem;

    SystemState m_State;
};

extern SystemClass *SystemSingleton;

ISystem* GetSystem()
{
    return SystemSingleton;
}
#endif