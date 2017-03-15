#include "ShaderPool.h"
#include "AbstractShader.h"
#include "IObjectFactory.h"

#include <ApproxSystemErrors.h>
#include <OS.h>
#include <ApproxStatistics.h>

using namespace std;
using namespace ShaderSystem;


void ShaderPool::Load(const std::string &dir) {
    int ID = 0;
    for (auto name : OS::Files::list(dir, "*.acs*"))
    {
        auto path = dir + name;
        g_Statistics->IncNumOfShaders();
        ShaderDesc desc;
        desc.ID = ID++;
        try {
            auto newShader = getObjectFactory()->createShader(path, desc);
            m_ShaderCache.insert({newShader->GetDesc().name, newShader});
        }
        catch (const ApproxException& reason) {
            ApproxException("Could not create a shader object.", "ShaderPool").becauseOf(reason)();
        }
    }
}

void ShaderPool::Clear() {
    m_ShaderCache.clear();
}

ShaderPool& ShaderPool::instance() {
    static ShaderPool instance;
    return instance;
}

IShaderRef ShaderPool::GetShader(const ShaderDesc& desc)
{
	return get_shader(desc.name);
}

IShaderRef ShaderPool::get_shader(const string &name)
{
	auto res = m_ShaderCache.find(name);
    if (res == m_ShaderCache.end())
        return m_ShaderCache.empty() ? nullptr : m_ShaderCache.begin()->second;

	return res->second;
}

void ShaderPool::UpdatePerFrameBuffersForAll()
{
	for(auto shader : m_ShaderCache) {
		shader.second->UpdatePerFrameBuffers();
	}
}

void ShaderPool::UpdateSceneConstantsBuffersForAll()
{
	for(auto shader : m_ShaderCache) {
		shader.second->UpdateSceneConstantsBuffers();
	}
}

vector<ShaderDesc> ShaderPool::LoadedShadersDescs() const
{
    vector<ShaderDesc> descriptions;
    descriptions.reserve(m_ShaderCache.size());
    for (auto shader : m_ShaderCache) {
        descriptions.push_back(shader.second->GetDesc());
    }
	return descriptions;
}

ShaderPool::~ShaderPool() {

}
