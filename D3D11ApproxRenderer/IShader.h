#pragma once
#include "IShaderParams.h"
#include <d3d11.h>

enum ShaderErrors {SHDR_OK,SHDR_WRONG_PARAMS_TYPE,SHDR_PARAMS_IS_NULL};

__interface IShader
{
	void Initialize(ID3D11Device*);
	bool UpdateSceneConstantsBuffers(ID3D11DeviceContext* context);
	bool UpdatePerFrameBuffers(ID3D11DeviceContext* context);
	void Shutdown();
	const ShaderDesc& GetDesc();							
    unsigned short GetNumberOfTextureSlots();
    bool SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot);
	bool Render(ID3D11DeviceContext* context, unsigned int indexCount);
};

//IShader* ShaderFactory(const ShaderDesc& type);
