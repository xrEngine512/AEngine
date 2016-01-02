#include "Frustum.h"
#include "MeshBox.h"

Frustum::Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, const XMMATRIX &projectionMatrix, const XMMATRIX &viewMatrix)
{
    float zMinimum, r;
    XMMATRIX matrix;
    XMFLOAT4X4 f_matrix, f_projectionMatrix, f_viewMatrix;
    XMFLOAT4 planes[6];
    // Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = XMMatrixMultiply(viewMatrix, projectionMatrix);
    XMStoreFloat4x4(&f_projectionMatrix, projectionMatrix);
    XMStoreFloat4x4(&f_viewMatrix, viewMatrix);
    XMStoreFloat4x4(&f_matrix, matrix);
    // Calculate the minimum Z distance in the frustum.
    zMinimum = -f_projectionMatrix._43 / f_projectionMatrix._33;
    r = screenDepth / (screenDepth - zMinimum);
    f_projectionMatrix._33 = r;
    f_projectionMatrix._43 = -r * zMinimum;

    

    // Calculate near plane of frustum.
    planes[0].x = f_matrix._14 + f_matrix._13;
    planes[0].y = f_matrix._24 + f_matrix._23;
    planes[0].z = f_matrix._34 + f_matrix._33;
    planes[0].w = f_matrix._44 + f_matrix._43;

    // Calculate far plane of frustum.
    planes[1].x = f_matrix._14 - f_matrix._13;
    planes[1].y = f_matrix._24 - f_matrix._23;
    planes[1].z = f_matrix._34 - f_matrix._33;
    planes[1].w = f_matrix._44 - f_matrix._43;

    // Calculate left plane of frustum.
    planes[2].x = f_matrix._14 + f_matrix._11;
    planes[2].y = f_matrix._24 + f_matrix._21;
    planes[2].z = f_matrix._34 + f_matrix._31;
    planes[2].w = f_matrix._44 + f_matrix._41;

    // Calculate right plane of frustum.
    planes[3].x = f_matrix._14 - f_matrix._11;
    planes[3].y = f_matrix._24 - f_matrix._21;
    planes[3].z = f_matrix._34 - f_matrix._31;
    planes[3].w = f_matrix._44 - f_matrix._41;

    // Calculate top plane of frustum.
    planes[4].x = f_matrix._14 - f_matrix._12;
    planes[4].y = f_matrix._24 - f_matrix._22;
    planes[4].z = f_matrix._34 - f_matrix._32;
    planes[4].w = f_matrix._44 - f_matrix._42;

    // Calculate bottom plane of frustum.
    planes[5].x = f_matrix._14 + f_matrix._12;
    planes[5].y = f_matrix._24 + f_matrix._22;
    planes[5].z = f_matrix._34 + f_matrix._32;
    planes[5].w = f_matrix._44 + f_matrix._42;

    int i = 0;
    for (auto plane : planes)
    {
        m_planes[i] = XMLoadFloat4(&plane);
        XMPlaneNormalize(m_planes[i]);
        i++;
    }

    return;
}

bool Frustum::CheckPoint(XMFLOAT3 point)
{
    return CheckPoint(point.x, point.y, point.z);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
    int i;


    // Check if the point is inside all six planes of the view frustum.
    for (i = 0; i<6; i++)
    {
        XMFLOAT4 res;
        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet(x, y, z, 1)));
        if (res.w < 0.0f)
        {
            return false;
        }
    }

    return true;
}
bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
    int i;


    // Check if any one point of the cube is in the view frustum.
    for (i = 0; i<6; i++)
    {
        XMFLOAT4 res;
        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        return false;
    }

    return true;
}
bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
    int i;
    XMFLOAT4 res;

    // Check if the radius of the sphere is inside the view frustum.
    for (i = 0; i < 6; i++)
    {
        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter, yCenter, zCenter, 1)));
        if (res.w < -radius)
        {
            return false;
        }
    }
    return true;
}

bool Frustum::CheckBox(MeshBox* box)
{
    float xCenter, yCenter, zCenter, xSize, ySize, zSize;
    box->GetCenter(xCenter, yCenter, zCenter);
    box->GetSize(xSize, ySize, zSize);
    return CheckRectangle(xCenter, yCenter, zCenter, xSize, ySize, zSize);
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
    int i;


    // Check if any one point of the cube is in the view frustum.
    for (i = 0; i<6; i++)
    {
        XMFLOAT4 res;
        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        XMStoreFloat4(&res, XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1)));

        if (res.w >= 0.0f)
        {
            continue;
        }

        return false;
    }
    return true;
}

Frustum::~Frustum()
{
}
