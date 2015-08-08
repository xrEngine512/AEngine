#pragma once
#include "IShaderParams.h"
#include <d3d11.h>

namespace ShaderSystem{
	union floatVariant;

	enum ShaderErrors { SHDR_OK, SHDR_WRONG_PARAMS_TYPE, SHDR_PARAMS_IS_NULL };
	struct ShaderSettings;
	interface IShader
	{
		virtual void Initialize(ID3D11Device*) = 0;
		virtual bool UpdateSceneConstantsBuffers(ID3D11DeviceContext* context) = 0;
		virtual bool UpdatePerFrameBuffers(ID3D11DeviceContext* context) = 0;
		virtual void Shutdown() = 0;
		virtual const ShaderDesc& GetDesc() = 0;
		virtual unsigned short GetNumberOfTextureSlots() = 0;
		virtual bool SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot) = 0;
		virtual bool Render(ID3D11DeviceContext* context, unsigned int indexCount) = 0;
		virtual const ShaderSettings& GetSettings() = 0;
		virtual void SetParameter(const std::string& name, const floatVariant& value) = 0;
		virtual ~IShader() {};
	};
}
//IShader* ShaderFactory(const ShaderDesc& type);
