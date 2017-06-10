#include "Material.h"
#include "GenericStruct.h"
#include "GenericVariable.h"
#include "ShaderPool.h"
#include "ShaderDesc.h"
#include "IObjectFactory.h"

#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>


using namespace std;

void Material::UpdateTextures()
{
	while (m_newTextures.size())
	{
		const auto& info = m_newTextures.front();
		m_Textures[info.second] = getObjectFactory()->createTexture(info.first);
		m_newTextures.pop();
	}
}

Material::Material(const string& shader_name, const std::vector<std::string> &textures) : m_State(NOT_INITIALIZED)
{
    m_Shader = ShaderPool::instance().get_shader(shader_name);

    if (m_Shader.expired()) {
        throw approx_exception("Shader with name '" + shader_name + "' is not loaded.", "Material");
    }

    auto shader = m_Shader.lock();
    m_ParamsData = ShaderSystem::GenericStruct(shader->ShaderParams());
    m_Textures.reserve(shader->GetNumberOfTextureSlots());
    for (auto texture : textures) {
        m_Textures.push_back(getObjectFactory()->createTexture(texture));
    }
    m_State = INITIALIZED;

    UpdateTextures();
}

inline const ShaderSystem::ShaderSettings& Material::Settings()const
{
	return m_Shader.lock()->GetSettings();
}

inline void Material::SetParameter(const std::string& name, const ShaderSystem::floatVariant& value)
{
	m_ParamsData[name] = value;
}

bool Material::LoadTexture(const string& texture_filename, unsigned short slot)
{
    if (m_State >= INITIALIZED)
    {
		if (!texture_filename.empty() && slot < m_Textures.size())
		{
			m_newTextures.push({texture_filename, slot});
			m_Textures[slot] = getObjectFactory()->createTexture(texture_filename);
			return true;
		}
        return false;
    }
	else
	{
		return false;
	}
}

bool Material::render(uint32_t indexCount)
{
	UpdateTextures();
    // show_visible the model
    for (uint32_t i = 0; i < m_Textures.size(); ++i)
        m_Textures[i]->render(i);
    return m_Shader.lock()->render(indexCount, m_ParamsData);
}

void Material::ChangeShaderAndSaveTextures(const ShaderDesc& type) {
    auto shader = m_Shader.lock();
    if (!shader || shader->GetDesc().ID != type.ID) {
        m_Shader = ShaderPool::instance().GetShader(type);
        m_Textures.resize(shader->GetNumberOfTextureSlots());
    }
}

Material::~Material() {

}

uint32_t Material::GetID() const {
    return m_Shader.lock()->GetDesc().ID;
}
