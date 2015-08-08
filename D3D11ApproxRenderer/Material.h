#pragma once

#include "D3DTexture.h"
#include "IInternalMaterial.h"
#include "IExternalMaterial.h"
#include <vector>
#include <queue>
using namespace std;

namespace ShaderSystem
{
	struct ShaderSettings;
}

class Material : public IInternalMaterial, public IExternalMaterial
{
    enum State : unsigned short {NOT_INITIALIZED, INITIALIZED} m_State;
	ShaderSystem::IShader *m_Shader;
    unsigned short m_NumberOFTextureSlots;
    vector<D3DTexture*> m_Textures;
    queue<pair<wstring, unsigned short>> m_newTextures;
    ID3D11Device* m_device;
    HWND m_hwnd;
public:
    Material();
	bool Initialize(ID3D11Device *device, ShaderDesc type)override;
	const ShaderSystem::ShaderSettings& Settings()const override;
	void SetParameter(const std::string& name, const ShaderSystem::floatVariant& value)override;
    bool LoadTexture(const wchar_t* diffTexfilename, unsigned short slot)override;
    bool Render(ID3D11DeviceContext *deviceContext, int indexCount)override;
    void ChangeShaderAndSaveTextures(const ShaderDesc& type)override;
    void Shutdown()override;
    virtual ~Material();
};