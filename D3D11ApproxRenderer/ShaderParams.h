#pragma once
#include "ShaderStructures.h"

//#pragma pack(push,16)
struct __declspec(align(16)) AbstractShaderParams : public IShaderParams
{
	bool ContainsLight();
	bool ContainsSpecularity();
	ShaderType GetShaderType();
	XMMATRIX worldMatrix;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	int indexCount;    
protected:
	ShaderType type = WIREFRAME_SHADER;
};

struct __declspec(align(16)) ColorShaderParams : public AbstractShaderParams
{
    ColorShaderParams();
};

struct __declspec(align(16)) TextureShaderParams : public AbstractShaderParams
{
    TextureShaderParams();
};

struct __declspec(align(16)) AmbientLightShaderParams : public AbstractShaderParams
{
	AmbientLightShaderParams();
	LightingData lighting;
};

struct __declspec(align(16)) SpecularShaderParams : public AmbientLightShaderParams
{
	SpecularShaderParams();
	SpecularFXData specular;
	XMFLOAT3 cameraPosition;
};

//#pragma pack(pop)
IShaderParams* ShaderParamsFactory(ShaderType);

typedef std::atomic <AmbientLightShaderParams> TS_AmbientLightShaderParams;
typedef std::atomic < SpecularShaderParams > TS_SpecularShaderParams;