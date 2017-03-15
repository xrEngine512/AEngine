#pragma once
#include "AlignedObject.h"
#include <ApproxMath.h>

class AABB;

class Frustum : public AlignedObject
{
    Math::AVector m_planes[6];
public:
    Frustum();
    ~Frustum();

    void ConstructFrustum(float screenDepth, const Math::AMatrix &projectionMatrix, const Math::AMatrix &viewMatrix);

    bool CheckPoint(float, float, float);
    bool CheckPoint(const Math::APoint&);
    bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
    bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
    bool CheckBox(const AABB &);
    bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
};

