#include "ShaderPool.h"
#include <ApproxSystemErrors.h>
#include <ApproxSystemTools.h>
#include <ApproxStatistics.h>

ShaderPool::ShaderPool(ID3D11Device* device, HWND _hwnd) :m_device(device), m_hwnd(_hwnd)
{
}

ShaderPool* g_ShaderPool = nullptr;

IShader* ShaderPool::GetShader(ShaderType type)
{
    auto res = m_IndexTable.find(type);
    if (res != m_IndexTable.end())
    {
        return res->second;
    }
    else
    {
        g_Statistics->IncNumOfShaders();
        IShader* tmpRef = ShaderFactory(type);
        
        m_IndexTable.insert(std::pair<ShaderType, IShader*>(type, tmpRef));
        return tmpRef;
    }
}

void ShaderPool::Shutdown()
{
    for each (auto shader in m_IndexTable)
    {
        DELETE_SYS_OBJECT(shader.second)
    }
}

ShaderPool::~ShaderPool()
{
}
