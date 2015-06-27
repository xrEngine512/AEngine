#pragma once
#include "IShader.h"
#include <d3d11.h>
#include "ShaderParams.h" 
#include <string>
#include <vector>
class ACSReader;
class RGS;

class UnifiedShader :  public IShader
{
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };


    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11GeometryShader* m_geometryShader;
    ID3D11ComputeShader* m_computeShader;
    ID3D11HullShader* m_hullShader;
    ID3D11DomainShader* m_domainShader;

    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_perObjectBuffer , *m_perFrameBuffer , *m_sceneConstants;
    ID3D11SamplerState* m_sampleState;
    ID3D11ShaderResourceView** m_textures;
    unsigned short m_NumberOfTextureSlots;
    ACSReader* m_reader;
	RGS *m_perObjectData, *m_perFrameData, *m_sceneConstantsData;
    std::string m_ShaderName;
    std::vector<char*> m_semanticNames;

	bool UpdateBuffer(ID3D11DeviceContext* context, ID3D11Buffer*, RGS *);

	bool UpdatePerObjectBuffer(ID3D11DeviceContext* context);

public:
    explicit UnifiedShader(wchar_t *acs_filename);
	void Initialize(ID3D11Device*)override final;
    void Shutdown()override final;
    ShaderType GetType()override final;
	unsigned short GetNumberOfTextureSlots()override final;
	bool UpdateSceneConstants(ID3D11DeviceContext* context)override final;
	bool UpdatePerFrameBuffer(ID3D11DeviceContext* context)override final;
	bool SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot)override final;
	bool Render(ID3D11DeviceContext* context, unsigned int indexCount)override final;
    virtual ~UnifiedShader();
};