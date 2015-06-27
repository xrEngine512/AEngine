#pragma once
#include "IShader.h"
#include <map>
class ShaderPool
{
    std::map<ShaderType, IShader*> m_IndexTable;
    ID3D11Device* m_device;
    HWND m_hwnd;
public:
    explicit ShaderPool(ID3D11Device*, HWND);
    IShader* GetShader(ShaderType);
    void Shutdown();
    ~ShaderPool();
};

extern ShaderPool* g_ShaderPool;