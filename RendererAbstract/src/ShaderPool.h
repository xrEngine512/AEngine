#pragma once
#include "IShader.h"
#include "ShaderDesc.h"

#include <unordered_map>
#include <vector>


class ShaderPool
{
    std::unordered_map<std::string, ShaderSystem::IShaderPtr> m_ShaderCache;
    ShaderPool() = default;
public:
    void Load(const std::string &shadersDir);
    void Clear();
    static ShaderPool& instance();
	ShaderSystem::IShaderRef GetShader(const ShaderDesc&);
	ShaderSystem::IShaderRef get_shader(const std::string &name);
	void UpdatePerFrameBuffersForAll();
	void UpdateSceneConstantsBuffersForAll();
    std::vector<ShaderDesc> LoadedShadersDescs()const;
    ~ShaderPool();
};
