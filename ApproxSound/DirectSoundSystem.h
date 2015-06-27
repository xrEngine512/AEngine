#pragma once
#include "IInternalSound.h"
#include "IExternalSound.h"
#include <Audio.h>
#include <IFnPtr.h>
using namespace DirectX;

class DirectSoundSystem : public IInternalSound, public IExternalSound
{
    AudioEngine* m_audioEngine;
    ExitCode Thread();
    WaveBank *m_soundBank;
    IFnPtr *threadFunc;
    SoundEffect* tmp;
public:
    bool Initialize();
    IFnPtr* GetThreadFunc();
    void PlayRain();
    bool Update();
    static _declspec(dllexport) DirectSoundSystem& Instance();
    DirectSoundSystem();
    ~DirectSoundSystem();
};

IInternalSound*  GetSoundSystem()
{
    return  &DirectSoundSystem::Instance();
}