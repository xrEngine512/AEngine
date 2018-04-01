#pragma once

#include "IInternalMaterial.h"
#include "IExternalMaterial.h"
#include "ITexture2D.h"
#include "IShader.h"
#include "GenericStruct.h"

#include <vector>
#include <queue>


namespace ShaderSystem
{
	struct ShaderSettings;
	class GenericStruct;
}

class Material : public IInternalMaterial, public IExternalMaterial
{
    enum State : uint8_t {NOT_INITIALIZED, INITIALIZED} m_State;
	ShaderSystem::IShaderRef    m_Shader;
    std::vector<ITexture2DPtr>  m_Textures;

	ShaderSystem::GenericStruct m_ParamsData;

	typedef std::queue<std::pair<std::string, unsigned short>> TextureQueue;
	TextureQueue m_newTextures;
	void UpdateTextures();

public:
    Material(const std::string& shader, const std::vector<std::string> &textures);
	const ShaderSystem::ShaderSettings& Settings()const override;
	void SetParameter(const std::string& name, const ShaderSystem::floatVariant& value)override;
    bool LoadTexture(const std::string& texture_filename, unsigned short slot)override;
    void render(uint32_t indexCount) override;
	uint32_t GetID() const;

    void ChangeShaderAndSaveTextures(const ShaderDesc& type)override;
    virtual ~Material();
};
