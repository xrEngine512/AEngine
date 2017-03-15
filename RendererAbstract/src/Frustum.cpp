#include "Frustum.h"
#include "AABB.h"

using namespace Math;

Frustum::Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, const AMatrix &projectionMatrix, const AMatrix &viewMatrix)
{
    AMatrix matrix;
    // Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = viewMatrix * projectionMatrix;

    // Calculate near plane of frustum.
    m_planes[0].x = matrix[0][3] + matrix[0][2];
    m_planes[0].y = matrix[1][3] + matrix[1][2];
    m_planes[0].z = matrix[2][3] + matrix[2][2];
    m_planes[0].w = matrix[3][3] + matrix[3][2];

    // Calculate far plane of frustum.
    m_planes[1].x = matrix[0][3] - matrix[0][2];
    m_planes[1].y = matrix[1][3] - matrix[1][2];
    m_planes[1].z = matrix[2][3] - matrix[2][2];
    m_planes[1].w = matrix[3][3] - matrix[3][2];

    // Calculate left plane of frustum.
    m_planes[2].x = matrix[0][3] + matrix[0][0];
    m_planes[2].y = matrix[1][3] + matrix[1][0];
    m_planes[2].z = matrix[2][3] + matrix[2][0];
    m_planes[2].w = matrix[3][3] + matrix[3][0];

    // Calculate right plane of frustum.
    m_planes[3].x = matrix[0][3] - matrix[0][0];
    m_planes[3].y = matrix[1][3] - matrix[1][0];
    m_planes[3].z = matrix[2][3] - matrix[2][0];
    m_planes[3].w = matrix[3][3] - matrix[3][0];

    // Calculate top plane of frustum.
    m_planes[4].x = matrix[0][3] - matrix[0][1];
    m_planes[4].y = matrix[1][3] - matrix[1][1];
    m_planes[4].z = matrix[2][3] - matrix[2][1];
    m_planes[4].w = matrix[3][3] - matrix[3][1];

    // Calculate bottom plane of frustum.
    m_planes[5].x = matrix[0][3] + matrix[0][1];
    m_planes[5].y = matrix[1][3] + matrix[1][1];
    m_planes[5].z = matrix[2][3] + matrix[2][1];
    m_planes[5].w = matrix[3][3] + matrix[3][1];

    for (auto plane : m_planes)
        ANormalize(plane);
}

bool Frustum::CheckPoint(const APoint& point)
{
    return CheckPoint(point.x, point.y, point.z);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
    int i;


    // Check if the point is inside all six planes of the view frustum.
    for (i = 0; i<6; i++)
        if (ADotProduct(m_planes[i], AVector(x, y, z, 1)) < 0.0f)
            return false;

    return true;
}
bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
    // Check if any one point of the cube is in the view frustum.
    for (int i = 0; i<6; i++)
    {
        if (ADotProduct(m_planes[i], AVector((xCenter - radius), (yCenter - radius), (zCenter - radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + radius), (yCenter - radius), (zCenter - radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - radius), (yCenter + radius), (zCenter - radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + radius), (yCenter + radius), (zCenter - radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - radius), (yCenter - radius), (zCenter + radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + radius), (yCenter - radius), (zCenter + radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - radius), (yCenter + radius), (zCenter + radius), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + radius), (yCenter + radius), (zCenter + radius), 1)) >= 0.0f)
            continue;

        return false;
    }

    return true;
}
bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
    // Check if the radius of the sphere is inside the view frustum.
    for (int i = 0; i < 6; i++)
        if (ADotProduct(m_planes[i], AVector(xCenter, yCenter, zCenter, 1)) < -radius)
            return false;

    return true;
}

bool Frustum::CheckBox(const AABB &box)
{
    float xCenter, yCenter, zCenter, xSize, ySize, zSize;
    box.GetCenter(xCenter, yCenter, zCenter);
    box.GetSize(xSize, ySize, zSize);
    return CheckRectangle(xCenter, yCenter, zCenter, xSize, ySize, zSize);
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
    // Check if any one point of the cube is in the view frustum.
    for (int i = 0; i < 6; i++)
    {
        if (ADotProduct(m_planes[i], AVector((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1)) >= 0.0f)
            continue;

        if (ADotProduct(m_planes[i], AVector((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1)) >= 0.0f)
            continue;

        return false;
    }
    return true;
}

Frustum::~Frustum()
{
}
