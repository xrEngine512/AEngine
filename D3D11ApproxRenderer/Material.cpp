#include "Material.h"
#include "ShaderParams.h"
#include "ShaderStructures.h"
#include <ApproxSystemTools.h>
#include <ApproxSystemErrors.h>
#include "ShaderPool.h"

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

    m_NumberOFTextureSlots = m_Shader->GetNumberOfTextureSlots();
    m_Textures.resize(m_NumberOFTextureSlots);
    for (auto ptr : m_Textures)
    {
        ptr = nullptr;
    }
    m_State = INITIALIZED;
    if (m_NumberOFTextureSlots)
        LoadTexture(L"../Engine/Resources/Textures/NoTexture.dds", 0);
    
    while (!m_newTextures.empty())
    {
        pair<wstring, unsigned short> element = m_newTextures.front();
        LoadTexture(element.first.c_str(), element.second);
        m_newTextures.pop();
    }
    return true;
}

inline const ShaderSystem::ShaderSettings& Material::Settings()const
{
	return m_Shader->GetSettings();
}

inline void Material::SetParameter(const std::string& name, const ShaderSystem::floatVariant& value)
{
	return m_Shader->SetParameter(name, value);
}

bool Material::LoadTexture(const wchar_t* diffTexfilename, unsigned short slot)
{
    if (m_State >= INITIALIZED)
    {
        if (diffTexfilename&&slot < m_NumberOFTextureSlots)
        {
            D3DTexture *Texture = new D3DTexture;
            if (!Texture->Initialize(m_device, diffTexfilename))
                return false;
            if (!m_Shader->SetTexture(Texture->GetTexture(), slot))
                return false;
            if (m_Textures[slot])
            {
                DELETE_SYS_OBJECT(m_Textures[slot]);
                m_Textures[slot] = Texture;
            }
            else
            {
                m_Textures[slot] = Texture;
            }
            return true;
        }
        return false;
    }
    else
    {
        if (diffTexfilename)
        {
            m_newTextures.push(pair<const wchar_t*, unsigned short>(diffTexfilename, slot));
            return true;
        }
		return false;
    }
}

bool Material::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
    // Render the model
    return m_Shader->Render(deviceContext,indexCount);
}

void Material::ChangeShaderAndSaveTextures(const ShaderDesc& type)
{
    if (m_Shader->GetDesc().ID != type.ID)
    {
        DELETE_SYS_OBJECT(m_Shader)

        m_Shader = g_ShaderPool->GetShader(type);
        
        if (!m_Textures.empty())
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
        }
    }
}

void Material::Shutdown()
{
    for (auto Texture:m_Textures)
    {
        DELETE_SYS_OBJECT(Texture)
    }
}



Material::~Material()
{
    Material::Shutdown();
}
