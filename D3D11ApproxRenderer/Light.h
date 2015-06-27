#pragma once
#include <DirectXMath.h>
#include "ShaderStructures.h"

using namespace DirectX;
class Light
{
    XMFLOAT4 m_ambientColor;
    XMFLOAT4 m_diffuseColor;
    XMFLOAT3 m_direction;
public:
	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetAmbientColor();
	XMFLOAT3 GetDirection();
    void ApplyLight(LightingData *lighting);
	Light(void);
	~Light(void);
};