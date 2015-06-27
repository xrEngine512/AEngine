#include "DirectSoundSystem.h"
#include <FnPtr.h>

DirectSoundSystem::DirectSoundSystem() :m_audioEngine(nullptr), m_soundBank(nullptr)
{
    threadFunc = nullptr;
}

bool DirectSoundSystem::Initialize()
{
    if (m_audioEngine)
        return false;
    m_audioEngine = new AudioEngine;
    if (!m_audioEngine)
        return false;
    if (threadFunc)
        return false;
    if (m_soundBank)
        return false;
    
    threadFunc = new FnPtr<DirectSoundSystem>(&DirectSoundSystem::Thread, this);
}

void DirectSoundSystem::PlayRain()
{
    tmp = new SoundEffect(m_audioEngine, L"../../Core/Engine/Resources/Sounds/rain-01.wav");
    tmp->Play();
}

DirectSoundSystem& DirectSoundSystem::Instance()
{
    static DirectSoundSystem instance;
    return instance;
}

IFnPtr* DirectSoundSystem::GetThreadFunc()
{
    return threadFunc;
}

bool DirectSoundSystem::Update()
{
    return m_audioEngine->Update();
}

ExitCode DirectSoundSystem::Thread()
{
    return DONE;
}

DirectSoundSystem::~DirectSoundSystem()
{
    delete tmp;
}
