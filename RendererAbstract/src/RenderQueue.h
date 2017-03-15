#pragma once
//
// Created by islam on 26.02.17.
//

#include "Material.h"
#include "IGeometryBuffer.h"
#include "IInternalRenderObject.h"

#include <unordered_map>
#include <vector>

namespace MatInterface {
    struct MaterialInterface;
}


class RenderQueue {
    std::vector<IInternalRenderObject*> by_object;
public:
    void add_object(IInternalRenderObject* object);
    void remove_object(const IInternalRenderObject* object);
    void render(MatInterface::MaterialInterface&);
};



