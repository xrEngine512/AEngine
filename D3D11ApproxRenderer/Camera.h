#pragma once
#include <DirectXMath.h>
#include "AlignedObject.h"

using namespace DirectX;

__declspec(align(16)) class Camera : public AlignedObject
{
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
    XMFLOAT3 m_ForwardDirection, m_SideDirection, m_UpDirection;
public:
	Camera(void);
	~Camera(void);
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
    void Rotate(float, float, float);
    void MoveLocal(float x, float y, float z);
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
    XMFLOAT3 GetDirection();

	void Render();
	void GetViewMatrix(XMMATRIX&);
};

