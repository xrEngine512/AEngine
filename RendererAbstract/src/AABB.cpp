//
// Created by islam on 24.02.17.
//

#include <include/IObjectFactory.h>
#include "AABB.h"



void AABB::GetCenter(float &CenterX, float &CenterY, float &CenterZ) const {
    CenterX = (left + right) / 2;
    CenterY = (top + bottom) / 2;
    CenterZ = (back + front) / 2;
}

inline bool Inters(float min1, float max1, float min2, float max2) {
    return min1 <= max2 && max1 >= min2;
}

bool AABB::Intersects(const AABB& box) const
{
    return Inters(left, right, box.left, box.right) &&
           Inters(bottom, top, box.bottom, box.top) &&
           Inters(front, back, box.front, box.back);
}

Math::AVector3 AABB::GetSize() const {
    return Math::AVector3(abs(left - right), abs(top - bottom), abs(back - front));
}

Math::AVector3 AABB::GetCenter() const {
    return Math::AVector3((left + right) / 2, (top + bottom) / 2, (back + front) / 2);
}

void AABB::GetSize(float &SizeX, float &SizeY, float &SizeZ) const {
    SizeX = abs(left - right);
    SizeY = abs(top - bottom);
    SizeZ = abs(back - front);
}

IGeometryBufferPtr AABB::generate_geometry_buffer() const {
    return getObjectFactory()->createGeometryBuffer(generate_vertices(), get_indices(), *this);
}

std::vector<DefaultVertex> AABB::generate_vertices() const {
    return {
        {left, bottom, front},
        {left, bottom, back},
        {left, top, front},
        {left, top, back},
        {right, bottom, front},
        {right, bottom, back},
        {right, top, front},
        {right, top, back}
    };
}

std::vector<index_t> AABB::get_indices() const {
    return {
        0, 1, 2,
        0, 2, 3,
        0, 4, 5,
        0, 5, 1,
        1, 5, 6,
        1, 6, 2,
        2, 6, 7,
        2, 7, 3,
        3, 7, 4,
        3, 4, 0,
        4, 7, 6,
        4, 6, 5
    };
}
