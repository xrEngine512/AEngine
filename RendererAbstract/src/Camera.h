#pragma once
#include "AlignedObject.h"

#include <ApproxMath.h>


class Camera : public AlignedObject
{
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	Math::AMatrix m_viewMatrix;
    Math::AVector3 m_ForwardDirection, m_SideDirection, m_UpDirection;
public:
	Camera(void);
	~Camera(void);
	void set_position(float, float, float);
	void set_rotation(float, float, float);
    void Rotate(float, float, float);
    void MoveLocal(float x, float y, float z);
	Math::APoint GetPosition();
	Math::AVector3 GetRotation();
	Math::AVector3 GetDirection();

	void Render();
	Math::AMatrix GetViewMatrix() const;
};

