#pragma once
#include <string>
#include <memory>

struct ShaderDesc;

namespace ShaderSystem{
	struct ShaderSettings;
	union floatVariant; }

interface IExternalMaterial
{
	virtual const ShaderSystem::ShaderSettings& Settings()const = 0;
	virtual void SetParameter(const std::string& name, const ShaderSystem::floatVariant& value) = 0;
	virtual bool LoadTexture(const std::string& diffTexfilename, unsigned short slot) = 0;
	virtual void ChangeShaderAndSaveTextures(const ShaderDesc& type) = 0;
	virtual ~IExternalMaterial(){};
};

typedef std::shared_ptr<IExternalMaterial> IExternalMaterialPtr;
typedef std::weak_ptr<IExternalMaterial> IExternalMaterialRef;
