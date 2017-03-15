#pragma once
#include <ApproxMath.h>

struct LightingData
{
	Math::AVector3 lightDirection;
	Math::AVector ambientColor;
	Math::AVector diffuseColor;
};

struct SpecularFXData
{
	Math::AVector specularColor;
	float specularPower;
};