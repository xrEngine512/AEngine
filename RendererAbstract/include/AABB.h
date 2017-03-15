#pragma once
#include "IGeometryBuffer.h"
#include "VertexTypes.h"

#include <ApproxMath.h>

#include <vector>
#include <limits>

class AABB
{
public:
    float left      =   std::numeric_limits<float>::max();
    float right     =   std::numeric_limits<float>::min();
    float bottom    =   std::numeric_limits<float>::max();
    float top       =   std::numeric_limits<float>::min();
    float front     =   std::numeric_limits<float>::max();
    float back      =   std::numeric_limits<float>::min();

    AABB() = default;
    AABB(AABB&&) = default;
    AABB(const AABB&) = default;
    void GetSize(float &SizeX, float &SizeY, float &SizeZ) const;
    void GetCenter(float &CenterX, float &CenterY, float &CenterZ) const;
    bool Intersects(const AABB& box) const;
    IGeometryBufferPtr generate_geometry_buffer() const;

    Math::AVector3 GetSize() const;
    Math::AVector3 GetCenter() const;

private:
    std::vector<DefaultVertex> generate_vertices() const;
    std::vector<index_t> get_indices() const;
};

