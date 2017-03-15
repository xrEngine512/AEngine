//
// Created by islam on 26.02.17.
//

#pragma once

#include <ApproxMath.h>

#include <string>
#include <memory>

interface IGraphicsAPI {
    virtual void initialize(int screen_width, int screen_height, bool vSync, WindowHandle viewport, bool full_screen,
                            const char *application_name = "ApproxEngine") = 0;
    virtual bool begin_scene(const Math::AVector &color) = 0;
    virtual bool end_scene() = 0;
    virtual bool set_zbuffer_enabled(bool enabled) = 0;
    virtual bool set_alpha_blending_enabled(bool enabled) = 0;
    virtual void shutdown() = 0;
    virtual ~IGraphicsAPI(){};
};

typedef std::unique_ptr<IGraphicsAPI> IGraphicsAPIPtr;
