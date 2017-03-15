#include "Camera.h"

using namespace Math;

Camera::Camera(void):m_positionX(0),m_positionY(0),m_positionZ(0),m_rotationX(0),m_rotationY(0),m_rotationZ(0)
{
}


Camera::~Camera(void)
{
}

void Camera::set_position(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::set_rotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

void Camera::Rotate(float x, float y, float z)
{
    m_rotationX += x;
    m_rotationY += y;
    m_rotationZ += z;
    return;
}

void Camera::MoveLocal(float x, float y, float z)
{
    m_positionX += m_ForwardDirection.x * z + m_SideDirection.x * x + m_UpDirection.x * y;
    m_positionY += m_ForwardDirection.y * z + m_SideDirection.y * x + m_UpDirection.y * y;
    m_positionZ += m_ForwardDirection.z * z + m_SideDirection.z * x + m_UpDirection.z * y;
    return;
}

APoint Camera::GetPosition()
{
	return {m_positionX, m_positionY, m_positionZ};
}


AVector3 Camera::GetRotation()
{
	return {m_rotationX, m_rotationY, m_rotationZ};
}

AVector3 Camera::GetDirection()
{
    return m_ForwardDirection;
}

void Camera::Render()
{
	AVector3 up{0.0f, 1.0f, 0.0f},
			position{m_positionX,m_positionY,m_positionZ},
			lookAt{0.0f, 0.0f, 1.0f};
	float yaw, pitch, roll;
	AMatrix3 rotationMatrix;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = AFromEuler(pitch,yaw,roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = lookAt * rotationMatrix;

	up = up * rotationMatrix;

    m_UpDirection = ANormalize(up);
    m_ForwardDirection = ANormalize(lookAt);
    m_SideDirection = ANormalize(ACrossProduct(lookAt, up));
    
    // Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = ALookAt(AVector3(position), AVector3(lookAt), AVector3(up));

	return;
}

AMatrix Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}