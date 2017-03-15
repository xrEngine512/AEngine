//
// Created by islam on 17.02.17.
//

#include "RendererVulkan.h"
#include "../Sound/include/IInternalSound.h"


IExternalScene *RendererVulkan::get_scene() const {
    return Renderer::get_scene();
}

RendererVulkan::~RendererVulkan() {

}

APPROX_API Renderer* GetRenderer() {
    static RendererVulkan instance;
    return &instance;
}
