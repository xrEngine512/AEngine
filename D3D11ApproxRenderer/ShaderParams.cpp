#include "IShaderParams.h"
#include "ShaderParams.h"
#include <map>
#include <memory>
bool ff[2] = { false, false }, tf[2] = { true, false }, tt[2] = {true, true};
std::map<ShaderDesc, bool*> shaderDesc = { { COLOR_SHADER, ff }, { TEXTURE_SHADER, ff }, 
{AMBIENT_LIGHT_SHADER, tf}, {SPECULAR_SHADER, tt} };	//Заполнять из XML

ShaderDesc AbstractShaderParams::GetShaderDesc()
{
	return type;
}

ColorShaderParams::ColorShaderParams()
{
    type = COLOR_SHADER;
}

TextureShaderParams::TextureShaderParams()
{
    type = TEXTURE_SHADER;
}

AmbientLightShaderParams::AmbientLightShaderParams()
{
	type = AMBIENT_LIGHT_SHADER;
}

SpecularShaderParams::SpecularShaderParams()
{
	type = SPECULAR_SHADER;
}

bool AbstractShaderParams::ContainsLight()
{
    if (type == WIREFRAME_SHADER)
        return false;
	return shaderDesc.find(type)->second[0];
}

bool AbstractShaderParams::ContainsSpecularity()
{
    if (type == WIREFRAME_SHADER)
        return false;
	return shaderDesc.find(type)->second[1];
}

template <class T>
inline size_t InitParam(IShaderParams* &param)
{
    param = new T;
    return sizeof(T);
}


inline void* aligned_alloc(size_t size, size_t alignment = 16)
{
    size_t buffSize = size + 64;
    void *pBuf = new char[buffSize];
    void* aligned = std::align(alignment, size, pBuf, buffSize);
    return aligned;
}

IShaderParams* ShaderParamsFactory(ShaderDesc type)
{
   /* IShaderParams* param;
    size_t size;*/
	switch (type)
	{
    case COLOR_SHADER:          return new ColorShaderParams;           //size = InitParam<ColorShaderParams>(param); break;
    case TEXTURE_SHADER:		return new TextureShaderParams;         //size = InitParam<TextureShaderParams>(param); break;
    case AMBIENT_LIGHT_SHADER:  return new AmbientLightShaderParams;    //size = InitParam<AmbientLightShaderParams>(param); break;
    case SPECULAR_SHADER:		return new SpecularShaderParams;        //size = InitParam<SpecularShaderParams>(param); break;
    default:					return new AbstractShaderParams;        //size = InitParam<AbstractShaderParams>(param); break;
	}
   /* void *mem = _aligned_malloc(size, 16);
    memcpy(mem, param, size);
    delete param;
    
    return reinterpret_cast<IShaderParams*>(mem);*/
}