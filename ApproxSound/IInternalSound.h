#pragma once

__interface IInternalSound
{
    bool Initialize();
    bool Update();
};

extern "C" _declspec(dllexport) IInternalSound*  GetSoundSystem();