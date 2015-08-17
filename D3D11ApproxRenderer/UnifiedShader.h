#pragma once
#include "IShader.h"
#include <d3d11.h>
#include <vector>
#include "ACSReader.h"
#include "ShaderSystemTypes.h"

class ShaderPool;
class ACSReader;

using std::vector;

namespace ShaderSystem
{
	class MemoryMultiplexer;
	class GenericStruct;
	
	class UnifiedShader : public IShader
	{
		friend ShaderPool;
		ShaderDesc m_desc;

		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11GeometryShader* m_geometryShader;
		ID3D11ComputeShader* m_computeShader;
		ID3D11HullShader* m_hullShader;
		ID3D11DomainShader* m_domainShader;

		ID3D11InputLayout* m_layout;
		struct BufferPack
		{
			struct Buffer{
				ID3D11Buffer* m_Buffer = nullptr;
				short num = 0;
				~Buffer();
			}m_sceneConstants, m_perFrameBuffer, m_perObjectBuffer;
			MemoryMultiplexer *m_perObjectData = nullptr, *m_perFrameData = nullptr, *m_sceneConstantsData = nullptr;
			~BufferPack();
		}VS_buffers, PS_buffers;
		GenericStruct *m_ParamsData = nullptr;
		ID3D11SamplerState* m_sampleState;
		ACSReader* m_reader;
		ShaderSettings m_Settings;

		void InitializeBuffer(ID3D11Device* device, const ASL::RuntimeBufferInfo&, BufferPack&, short BufferNum, const vector<int>& ParamIDs);
		bool UpdateBuffer(ID3D11DeviceContext* context, ID3D11Buffer*, MemoryMultiplexer *);
		void LoadBuffers(ID3D11Device* device, const ShaderElement& elem, BufferPack& pack);
		void LoadShaderParams();
		HRESULT CreateInputLayoutDescFromVertexShaderSignature(LPCVOID bufPtr, SIZE_T bufSize, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout, int* inputLayoutByteLength = nullptr);
		bool UpdatePerObjectBuffers(ID3D11DeviceContext* context);
		UnifiedShader(const wchar_t *acs_filename, const ShaderDesc& desc);
	public:
		void Initialize(ID3D11Device*)override final;
		const GenericStruct& ShaderParams()const override final;
		void Shutdown()override final;
		const ShaderDesc& GetDesc()override final;
		const ShaderSettings& GetSettings()override final;
		unsigned short GetNumberOfTextureSlots()override final;
		bool UpdateSceneConstantsBuffers(ID3D11DeviceContext* context)override final;
		bool UpdatePerFrameBuffers(ID3D11DeviceContext* context)override final;
		bool Render(ID3D11DeviceContext* context, unsigned int indexCount, ID3D11ShaderResourceView** textures, const GenericStruct*)override final;
		virtual ~UnifiedShader();
	};
}