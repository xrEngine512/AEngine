#include "Light.h"

#include <algorithm>

using namespace std;
using namespace Math;

Light::Light(void)
{    
}

void Light::set_diffuse_color(Math::AVector &&color) {
	m_diffuseColor = move(color);
}

void Light::set_ambient_color(Math::AVector &&color) {
	m_ambientColor = move(color);
}

void Light::set_direction(Math::AVector3 &&direction) {
	m_direction = move(direction);
}

void Light::SetDiffuseColor(const Math::AVector& color) {
    set_diffuse_color(AVector(color));
}

void Light::SetAmbientColor(const Math::AVector& color) {
	set_ambient_color(AVector(color));
}

void Light::SetDirection(const Math::AVector3& direction) {
	set_direction(AVector3(direction));
}

void Light::ApplyLight(LightingData *lighting)
{
	lighting->ambientColor = m_ambientColor;
    lighting->diffuseColor = m_diffuseColor;
    lighting->lightDirection = m_direction;
    return;
}

AVector Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

AVector Light::GetAmbientColor()
{
	return m_ambientColor;
}

AVector3 Light::GetDirection()
{
	return m_direction;
}

Light::~Light(void)
{
}
