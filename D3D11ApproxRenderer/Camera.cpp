#include "Camera.h"


Camera::Camera(void):m_positionX(0),m_positionY(0),m_positionZ(0),m_rotationX(0),m_rotationY(0),m_rotationZ(0)
{
}


Camera::~Camera(void)
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
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

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

XMFLOAT3 Camera::GetDirection()
{
    return m_ForwardDirection;
}

void Camera::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f,1.0f,0.0f,1.0f);

	// Setup the position of the camera in the world.
	position = XMVectorSet(m_positionX,m_positionY,m_positionZ,0);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f,0.0f,1.0f,1.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch,yaw,roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
    
    
    
	up = XMVector3TransformCoord(up, rotationMatrix);   

    XMStoreFloat3(&m_UpDirection, XMVector3Normalize(up));
    XMStoreFloat3(&m_ForwardDirection, XMVector3Normalize(lookAt));
    XMStoreFloat3(&m_SideDirection, XMVector3Normalize(XMVector3Cross(lookAt, up)));
    
    // Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}