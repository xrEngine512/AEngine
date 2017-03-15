#pragma once
#include "IShaderParams.h"
#include "Light.h"
#include "AABB.h"
#include "AlignedObject.h"
#include "Material.h"
#include "IGeometryBuffer.h"

#include <ApproxMath.h>
#include <AABB.h>

namespace MatInterface
{
	class MaterialInterface;
}

class Octree;

interface IInternalRenderObject : AlignedObject
{
	virtual bool render(MatInterface::MaterialInterface &) = 0;
	virtual std::vector<std::pair<IInternalMaterialRef, IGeometryBufferRef>> get_render_info() const = 0;
    virtual const AABB& aabb() const = 0;
    virtual Math::AVector GetPoint() = 0;
    virtual Math::AVector3 GetCoord() = 0;
    virtual unsigned int id() = 0;
    virtual void set_id(unsigned int) = 0;
    virtual ~IInternalRenderObject(){};
protected:
    friend class Octree;
    virtual void set_is_visible() = 0;
};