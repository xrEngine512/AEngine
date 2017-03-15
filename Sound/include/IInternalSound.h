#pragma once

interface IInternalSound
{
    virtual bool Initialize() = 0;
    virtual bool Update() = 0;
    virtual ~IInternalSound(){};
};

APPROX_API IInternalSound*  GetSoundSystem();