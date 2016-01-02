#pragma once
#include "AlignedObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class MeshBox;

class Frustum : public AlignedObject
{
    XMVECTOR m_planes[6];
public:
    Frustum();
    ~Frustum();

    void ConstructFrustum(float screenDepth, const XMMATRIX &projectionMatrix, const XMMATRIX &viewMatrix);

    bool CheckPoint(float, float, float);
    bool CheckPoint(XMFLOAT3);
    bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
    bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
    bool CheckBox(MeshBox*);
    bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);
};

