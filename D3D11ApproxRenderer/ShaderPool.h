#pragma once
#include "IShader.h"
#include <unordered_map>
class ShaderPool
{
	std::vector<const ShaderDesc*> m_shaderDescs;
    std::unordered_map<int, ShaderSystem::IShader*> m_ShaderCache;
    ID3D11Device* m_device;
public:
    ShaderPool(ID3D11Device*, const std::wstring& shadersDir, HWND);
	ShaderSystem::IShader* GetShader(const ShaderDesc&);
	ShaderSystem::IShader* GetShader(int ID);
	void UpdatePerFrameBuffersForAll(ID3D11DeviceContext*);
	void UpdateSceneConstantsBuffersForAll(ID3D11DeviceContext*);
	const std::vector<const ShaderDesc*>& LoadedShadersDescs()const;
    void Shutdown();
    ~ShaderPool();
};

extern ShaderPool* g_ShaderPool;