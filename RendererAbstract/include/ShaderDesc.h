#pragma once
#include <string>

/*enum ShaderDesc : unsigned short
{
    COLOR_SHADER, TEXTURE_SHADER, AMBIENT_LIGHT_SHADER, SPECULAR_SHADER, MULTITEXTURE_SHADER, WIREFRAME_SHADER,
    UNIFIED_SHADER
};*/

struct ShaderDesc
{
	ShaderDesc() = default;
	ShaderDesc(uint32_t ID):ID(ID){}
	void operator=(uint32_t ID)
	{
		this->ID = ID;
	}
	uint32_t ID;
	uint8_t TextureSlots = 0;
	std::string name;
};

/*inline wchar_t* GetShaderDescWStr(ShaderDesc type)
{
    switch (type)
    {
    case COLOR_SHADER: return L"COLOR_SHADER";
    case TEXTURE_SHADER: return L"TEXTURE_SHADER";
    case AMBIENT_LIGHT_SHADER: return L"AMBIENT_LIGHT_SHADER";
    case SPECULAR_SHADER: return L"SPECULAR_SHADER";
    case MULTITEXTURE_SHADER: return L"MULTITEXTURE_SHADER";
    case WIREFRAME_SHADER: return L"WIREFRAME_SHADER";
    default: return nullptr;
    }
}*/