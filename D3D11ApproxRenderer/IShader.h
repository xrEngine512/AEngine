#pragma once
#include "IShaderParams.h"
#include <d3d11.h>

namespace ShaderSystem{
	struct ShaderSettings;

	enum ShaderErrors { SHDR_OK, SHDR_WRONG_PARAMS_TYPE, SHDR_PARAMS_IS_NULL };
	class GenericStruct;
	interface IShader
	{
		virtual void Initialize(ID3D11Device*) = 0;
		virtual const GenericStruct& ShaderParams()const = 0;
		virtual bool UpdateSceneConstantsBuffers(ID3D11DeviceContext* context) = 0;
		virtual bool UpdatePerFrameBuffers(ID3D11DeviceContext* context) = 0;
		virtual void Shutdown() = 0;
		virtual const ShaderDesc& GetDesc() = 0;
		virtual unsigned short GetNumberOfTextureSlots() = 0;
		virtual bool Render(ID3D11DeviceContext* context, unsigned int indexCount, ID3D11ShaderResourceView** textures, const GenericStruct*) = 0;
		virtual const ShaderSettings& GetSettings() = 0;
		virtual ~IShader() {};
	};
	
	interface IStaticShader : IShader
	{
		virtual ShaderErrors SetParams(IShaderParams* params) = 0;
		virtual bool UpdateLight(ID3D11DeviceContext* context) = 0;
		virtual bool UpdateCamera(ID3D11DeviceContext* context) = 0;
	};
}
//IShader* ShaderFactory(const ShaderDesc& type);
