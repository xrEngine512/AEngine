#pragma once
#include "ShaderStructures.h"

#include <ApproxMath.h>

class Light
{
    Math::AVector m_ambientColor;
	Math::AVector m_diffuseColor;
	Math::AVector3 m_direction;
public:
	void set_diffuse_color(Math::AVector &&);
	void set_ambient_color(Math::AVector &&);
	void set_direction(Math::AVector3 &&);

	void SetDiffuseColor(const Math::AVector&);
	void SetAmbientColor(const Math::AVector&);
	void SetDirection(const Math::AVector3&);

	Math::AVector GetDiffuseColor();
	Math::AVector GetAmbientColor();
	Math::AVector3 GetDirection();
    void ApplyLight(LightingData *lighting);
	Light(void);
	~Light(void);
};