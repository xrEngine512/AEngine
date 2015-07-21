#pragma once
#include "IShader.h"
#include <d3d11.h>
#include "ShaderParams.h" 
#include <string>
#include <vector>


namespace ASL
{
	struct RuntimeBufferInfo;
}

class ShaderPool;
class ACSReader;
class RGS;

class UnifiedShader :  public IShader
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
		}m_sceneConstants, m_perFrameBuffer, m_perObjectBuffer;
		RGS *m_perObjectData = nullptr, *m_perFrameData = nullptr, *m_sceneConstantsData = nullptr;
	}VS_buffers,PS_buffers;
    ID3D11SamplerState* m_sampleState;
    ID3D11ShaderResourceView** m_textures;
    unsigned short m_NumberOfTextureSlots;
	ACSReader* m_reader;	

    std::vector<char*> m_semanticNames;
	void InitializeBuffer(ID3D11Device* device, const ASL::RuntimeBufferInfo&, BufferPack&, short BufferNum);
	bool UpdateBuffer(ID3D11DeviceContext* context, ID3D11Buffer*, RGS *);

	bool UpdatePerObjectBuffers(ID3D11DeviceContext* context);
	UnifiedShader(const wchar_t *acs_filename, const ShaderDesc& desc);
public:
    
	void Initialize(ID3D11Device*)override final;
    void Shutdown()override final;
    const ShaderDesc& GetDesc()override final;
	unsigned short GetNumberOfTextureSlots()override final;
	bool UpdateSceneConstantsBuffers(ID3D11DeviceContext* context)override final;
	bool UpdatePerFrameBuffers(ID3D11DeviceContext* context)override final;
	bool SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot)override final;
	bool Render(ID3D11DeviceContext* context, unsigned int indexCount)override final;
    virtual ~UnifiedShader();
};