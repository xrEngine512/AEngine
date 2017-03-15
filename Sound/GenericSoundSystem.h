#pragma once
//
// Created by islam on 17.02.17.
//

#include <IExternalSound.h>
#include <IInternalSound.h>


class GenericSoundSystem : public IInternalSound, public IExternalSound {
public:
    void PlayRain() override;

    bool Initialize() override;

    bool Update() override;

    static GenericSoundSystem& Instance();
};



