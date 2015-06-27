#include "Light.h"


Light::Light(void)
{    
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void Light::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

void Light::ApplyLight(LightingData *lighting)
{
	lighting->ambientColor = m_ambientColor;
    lighting->diffuseColor = m_diffuseColor;
    lighting->lightDirection = m_direction;
    return;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT3 Light::GetDirection()
{
	return m_direction;
}

Light::~Light(void)
{
}
