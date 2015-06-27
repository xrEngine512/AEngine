#include <d3d11.h>
#include "IShader.h"
/*#include "ColorShader.h"
#include "TextureShader.h"
#include "AmbientLightShader.h"
#include "SpecularShader.h"
#include "MultitextureShader.h"
#include "WireframeShader.h"*/
#include "UnifiedShader.h"

IShader* ShaderFactory(ShaderType type)
{
	switch (type)
	{
		/*case COLOR_SHADER:			return new ColorShader;
		case TEXTURE_SHADER:		return new TextureShader;
		case AMBIENT_LIGHT_SHADER:  return new AmbientLightShader;
		case SPECULAR_SHADER:		return new SpecularShader;
        case MULTITEXTURE_SHADER:   return new MultitextureShader;
        case WIREFRAME_SHADER:      return new WireframeShader;*/
        case UNIFIED_SHADER:        return new UnifiedShader(L"D:/BackUpBecauseofShitHappens/Users/Islam/Desktop/DX/GUI/Editor/testFile.acs");
		default:					return nullptr;
	}
}
