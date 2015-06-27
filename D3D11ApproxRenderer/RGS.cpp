#include "RGS.h"
#include <DirectXMath.h>

inline int GetSize(string type)
{
    if (type.compare("float") == 0)
    {
        return 4;
    }
    if (type.compare("XMFLOAT4") == 0)
    {
        return sizeof(DirectX::XMFLOAT4);
    }
    if (type.compare("XMFLOAT3") == 0)
    {
        return sizeof(DirectX::XMFLOAT3);
    }
    if (type.compare("XMFLOAT2") == 0)
    {
        return sizeof(DirectX::XMFLOAT2);
    }
    if (type.compare("XMMATRIX") == 0)
    {
        return sizeof(DirectX::XMMATRIX);
    }
    if (type.compare("XMVECTOR") == 0)
    {
        return sizeof(DirectX::XMVECTOR);
    }
}

void RGS::operator = (void* pData)
{
	if (m_Data)
	{
		free(m_Data);
	}
	m_Data = static_cast<byte*>(pData);
}

RGS::RGS(vector<string> types, vector<void*> ptrs) :m_ptrsToCopy(ptrs)
{
    m_ptrs.reserve(types.size());
    m_sizes.reserve(types.size());
    for (auto type : types)
    {
        int size = GetSize(type);
        m_sizes.push_back(size);
        m_size += size;
    }
    int aligned_size = 16;
    while (m_size > aligned_size)
    {
        aligned_size *= 2;
    }
    m_size = aligned_size;
    m_Data = static_cast<byte*>(malloc(aligned_size));

    int shift = 0;
    m_ptrs.push_back(m_Data);
	m_shifts.push_back(shift);
	for (int i = 0; i < m_sizes.size() - 1; i++)
    {
		shift += m_sizes[i];
        m_ptrs.push_back(m_Data + shift); 
		m_shifts.push_back(shift);
    }
    Update();
}

void RGS::Update()
{
	int i = 0;
	for (auto ptr : m_ptrsToCopy)
    {
        memcpy(m_Data + m_shifts[i], ptr, m_sizes[i]);   
		i++;
    }
}

int RGS::Size()
{
    return m_size;
}

byte* RGS::Data()
{
    return m_Data;
}

RGS::~RGS()
{
    free(m_Data);
}
