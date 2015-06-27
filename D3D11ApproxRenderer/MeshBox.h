#pragma once
#include <DirectXMath.h>
using namespace DirectX;

struct MeshBox
{
    float left;
    float right;
    float bottom;
    float top;
    float front;
    float back;

    void GetSize(float &SizeX, float &SizeY, float &SizeZ)
    {
        SizeX = abs(left - right);
        SizeY = abs(top - bottom);
        SizeZ = abs(back - front);
    }
    void GetCenter(float &CenterX, float &CenterY, float &CenterZ)
    {
        CenterX = (left + right) / 2;
        CenterY = (top + bottom) / 2;
        CenterZ = (back + front) / 2;
    }
    bool __fastcall Intersects(MeshBox* box);
    XMFLOAT3 GetSize()
    {
        return XMFLOAT3(abs(left - right), abs(top - bottom), abs(back - front));
    }
    XMFLOAT3 GetCenter()
    {
        return XMFLOAT3((left + right) / 2, (top + bottom) / 2, (back + front) / 2);
    }
};
inline bool Inters(float min1, float max1, float min2, float max2)
{
    if (min1 > max2) return false;
    if (max1 < min2) return false;
    return true;
}

inline bool MeshBox::Intersects(MeshBox* box)
{
    
    if (!Inters(left, right, box->left, box->right))
        return false;
    if (!Inters(bottom, top, box->bottom, box->top))
        return false;
    if (!Inters(front, back, box->front, box->back))
        return false;

    return true;
}

