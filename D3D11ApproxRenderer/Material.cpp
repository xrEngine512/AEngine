#include "Material.h"
#include "GenericStruct.h"
#include "GenericVariable.h"
#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>
#include "ShaderPool.h"
#include "D3DTexture.h"

#ifdef EDITOR_BUILD
void Material::UpdateTextures(ID3D11DeviceContext* context)
{
	ID3D11Device* device;
	context->GetDevice(&device);
	UpdateTextures(device);
}
void Material::UpdateTextures(ID3D11Device* device)
{
	while (m_newTextures.size())
	{
		const auto& info = m_newTextures.front();
		m_Textures[info.second].Shutdown();
		m_Textures[info.second].Initialize(device, info.first.c_str());
		m_TextureCache[info.second] = m_Textures[info.second].GetTexture();
		m_newTextures.pop();
	}
}
#endif

Material::Material() :m_State(NOT_INITIALIZED), m_Shader(nullptr), m_NumberOFTextureSlots(0)
{
}

bool Material::Initialize(ID3D11Device* device, ShaderDesc type)
{
    m_Shader = g_ShaderPool->GetShader(type);
    m_device = device;

    if (!m_Shader)
    {
        throw ApproxException(L"Could not create the shader object.", L"Material");
    }

	m_ParamsData = new ShaderSystem::GenericStruct(m_Shader->ShaderParams());
    m_NumberOFTextureSlots = m_Shader->GetNumberOfTextureSlots();
    m_Textures.resize(m_NumberOFTextureSlots);
	m_TextureCache = new ID3D11ShaderResourceView*[m_NumberOFTextureSlots];
    
    m_State = INITIALIZED;
    
#ifdef EDITOR_BUILD
	UpdateTextures(device);
#endif
        
    return true;
}

inline const ShaderSystem::ShaderSettings& Material::Settings()const
{
	return m_Shader->GetSettings();
}

inline void Material::SetParameter(const std::string& name, const ShaderSystem::floatVariant& value)
{
	(*m_ParamsData)[name] = value;
}

bool Material::LoadTexture(const wchar_t* diffTexfilename, unsigned short slot)
{
    if (m_State >= INITIALIZED)
    {
		if (diffTexfilename&&slot < m_NumberOFTextureSlots)
		{
#ifdef EDITOR_BUILD
			m_newTextures.push(TextureQueue::value_type(diffTexfilename, slot));
#else
			m_Textures[slot].Shutdown();
			m_Textures[slot].Initialize(m_device, diffTexfilename);
			m_TextureCache[slot] = m_Textures[slot].GetTexture();
#endif
			return true;
		}
        return false;
    }
	else
	{
		return false;
	}
}

bool Material::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
#ifdef EDITOR_BUILD
	UpdateTextures(deviceContext);
#endif
    // Render the model
    return m_Shader->Render(deviceContext,indexCount,m_TextureCache,m_ParamsData);
}

void Material::ChangeShaderAndSaveTextures(const ShaderDesc& type)
{
    if (m_Shader->GetDesc().ID != type.ID)
    {
        DELETE_SYS_OBJECT(m_Shader)

        m_Shader = g_ShaderPool->GetShader(type);
        
        /*if (!m_Textures.empty())
        {
            int i = 0;
            for (auto Texture : m_Textures)
            {
                if (m_Shader->SetTexture(Texture->GetTexture(), i))
                    i++;
                else
                    break;
            }
            m_NumberOFTextureSlots = m_Shader->GetNumberOfTextureSlots();
            if (m_NumberOFTextureSlots < m_Textures.size())
            {
                for (int j = m_NumberOFTextureSlots; j < m_Textures.size();j++)
                {
                    DELETE_SYS_OBJECT(m_Textures[j]);
                }
                m_Textures.resize(m_NumberOFTextureSlots);
            }
            else if (m_NumberOFTextureSlots> m_Textures.size())
            {
                int prevSize = m_Textures.size();
                m_Textures.resize(m_NumberOFTextureSlots);
                for (int j = prevSize - 1; j < m_Textures.size(); j++)
                {
                    m_Textures[j] = nullptr;
                }
            }
        }*/
    }
}

void Material::Shutdown()
{
	m_Textures.clear();
	delete[] m_TextureCache;
}



Material::~Material()
{
	Material::Shutdown();
}
