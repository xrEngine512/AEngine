//
// Created by islam on 17.02.17.
//

#include "GenericSoundSystem.h"

void GenericSoundSystem::PlayRain() {

}

bool GenericSoundSystem::Initialize() {
    return true;
}

bool GenericSoundSystem::Update() {
    return true;
}

GenericSoundSystem& GenericSoundSystem::Instance()
{
    static GenericSoundSystem instance;
    return instance;
}

IInternalSound* GetSoundSystem()
{
    return &GenericSoundSystem::Instance();
}