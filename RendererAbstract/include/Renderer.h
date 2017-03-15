#pragma once
#include "IExternalRenderer.h"

#include <IInternalSound.h>
#include <IActivity.h>
#include <MState.h>

#include <atomic>
#include <vector>


class AbstractScene;

struct RendererParameters {
    WindowHandle viewport;
    bool full_screen = false;
    int screen_width = 0;
    int screen_height = 0;
    bool vSync = true;
};

class Renderer : public IActivity, public IExternalRenderer, public MActivityState
{
    AbstractScene* m_Scene;
    std::atomic_bool m_done;
    IInternalSound* m_SoundSys;

    ExitCode activity();
    bool Frame();

protected:
    Renderer();
    ~Renderer();
public:
    std::vector<ShaderDesc> AvailableShadersDesc()const override;
    Activity get_activity()override;
    IExternalScene* get_scene()const override;

    void BindSoundSystem(IInternalSound* s_sys);
    bool initialize(const RendererParameters &parameters);
    void shutdown();
};

APPROX_API Renderer* GetRenderer();
