#pragma once
#include <DirectXMath.h>
using namespace DirectX;
struct ShaderParamElement 
{

};

struct LightingData
{
	XMFLOAT3 lightDirection;
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
};

struct SpecularFXData
{
	XMFLOAT4 specularColor;
	float specularPower;
};