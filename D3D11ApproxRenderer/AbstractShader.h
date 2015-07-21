#pragma once
#include "IShader.h"
#include <d3d11.h>
#include "ShaderParams.h" 
#include <ApproxSystemTools.h>

enum StandartVertexOffsets:unsigned int {SVO_POSITION = 0, SVO_COLOR = 12, SVO_TEXCOORD = 28, SVO_NORMAL = 36};

class AbstractShader : public IShader
{
	
private:
	AbstractShaderParams* m_basicParams;
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
    ID3D11GeometryShader* m_geometryShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
    ID3D11ShaderResourceView** m_textures;
    unsigned short m_NumberOfTextureSlots;
    
    template<class TO>
    inline TO shader_params_cast(IShaderParams* params)
    {
        if (params->GetShaderDesc() == this->GetType())
        {
            return dynamic_cast<TO>(params);
        }
        else
        {
            return nullptr;
        }
    }

    void InitializeShader(ID3D11Device* device, WCHAR* vsFilename, WCHAR* psFilename, unsigned short NumberOfTextureSlots, WCHAR* gsFileName = nullptr);
	virtual bool CreateInputLayout(ID3D11Device* device, ID3DBlob* vertexShaderBuffer);
	virtual bool CreateSamplerState(ID3D11Device* device);
	virtual bool SetupConstantBuffer(ID3D11Device* device);
	virtual bool SetupBuffers(ID3D11Device* device);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob*, WCHAR*);

    bool __vectorcall SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&);
    void RenderShader(ID3D11DeviceContext*);
public:
	AbstractShader(void);
	virtual ~AbstractShader(void);
    virtual ShaderErrors SetParams(IShaderParams* params) override;
    bool SetTexture(ID3D11ShaderResourceView* texture, unsigned int slot)override;
    virtual bool UpdateLight(ID3D11DeviceContext* context)override;
    virtual bool UpdateCamera(ID3D11DeviceContext* context)override;
    unsigned short GetNumberOfTextureSlots() override;
    bool Render(ID3D11DeviceContext*)override;
};

