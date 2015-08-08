#include "ShaderPool.h"
#include <ApproxSystemErrors.h>
#include <ApproxSystemTools.h>
#include <ApproxStatistics.h>
#include <vector>
#include "UnifiedShader.h"

using namespace std;
using namespace ShaderSystem;

inline vector<wstring> get_all_files_names_within_folder(const wstring& folder)
{
	vector<wstring> names;
	wchar_t search_path[200];
	wsprintf(search_path, L"%s*.acs*", folder.c_str());
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

ShaderPool::ShaderPool(ID3D11Device* device, const std::wstring& dir, HWND hwnd) :m_device(device)
{
	int ID = 0;
	for (auto name : get_all_files_names_within_folder(dir))
	{
		auto path = dir + name;
		g_Statistics->IncNumOfShaders();
		ShaderDesc desc;
		desc.ID = ID;
		IShader* tmpRef = new UnifiedShader(path.c_str(), desc);
		try
		{
			tmpRef->Initialize(m_device);
		}
		catch (const ApproxException& reason)
		{
			ApproxException(std::wstring(L"Could not initialize the shader object."), L"Material").becauseOf(reason)(hwnd);
		}
		m_shaderDescs.push_back(&tmpRef->GetDesc());
		m_ShaderCache.insert(std::pair<int, IShader*>(ID++, tmpRef));
	}
}

ShaderPool* g_ShaderPool = nullptr;

IShader* ShaderPool::GetShader(const ShaderDesc& desc)
{
	return GetShader(desc.ID);
}

IShader* ShaderPool::GetShader(int ID)
{
	auto res = m_ShaderCache.find(ID);
	if (res != m_ShaderCache.end())
    {
        return res->second;
    }
    else
    {
        
        return nullptr;
    }
}

void ShaderPool::UpdatePerFrameBuffersForAll(ID3D11DeviceContext* c)
{
	for each (auto shader in m_ShaderCache)
	{
		shader.second->UpdatePerFrameBuffers(c);
	}
}

void ShaderPool::UpdateSceneConstantsBuffersForAll(ID3D11DeviceContext* c)
{
	for each (auto shader in m_ShaderCache)
	{
		shader.second->UpdateSceneConstantsBuffers(c);
	}
}

const std::vector<const ShaderDesc*>& ShaderPool::LoadedShadersDescs() const
{
	return m_shaderDescs;
}

void ShaderPool::Shutdown()
{
    for each (auto shader in m_ShaderCache)
    {
        //DELETE_SYS_OBJECT(shader.second)
		if (shader.second)
		{
		shader.second->Shutdown();
		delete shader.second; 
		shader.second = nullptr; 
		}
    }
	m_ShaderCache.clear();
}

ShaderPool::~ShaderPool()
{
	Shutdown();
}
