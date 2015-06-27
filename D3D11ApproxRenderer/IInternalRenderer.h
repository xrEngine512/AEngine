#pragma once
#include <IFnPtr.h>
#include <IInternalSound.h>

__interface IInternalRenderer
{
    bool Initialize(void* parentWindowID, bool FullScreenOn, int screenWidth, int screenHeight, int xPos, int yPos);
    IFnPtr* GetThreadFunc();
    void BindSoundSystem(IInternalSound* s_sys);
    void Shutdown();
};

extern "C" _declspec(dllexport) IInternalRenderer* GetRenderer();