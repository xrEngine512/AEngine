//
// Created by islam on 09.03.17.
//

#pragma once

#include <memory>
#include <cstdint>

interface IInternalMaterial
{
    virtual void render(uint32_t indexCount) = 0;
    virtual ~IInternalMaterial(){};
};

typedef std::shared_ptr<IInternalMaterial> IInternalMaterialPtr;
typedef std::weak_ptr<IInternalMaterial> IInternalMaterialRef;
