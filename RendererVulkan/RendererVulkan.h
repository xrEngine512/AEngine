#pragma once
//
// Created by islam on 17.02.17.
//

#include <Renderer.h>
#include "../Sound/include/IInternalSound.h"


class RendererVulkan : public Renderer {


public:

    IExternalScene *get_scene() const override;

    ~RendererVulkan();
};



