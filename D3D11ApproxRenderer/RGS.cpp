#include "RGS.h"
#include <DirectXMath.h>

inline int GetSize(const string& type)
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
	return 0;
}

void RGS::operator = (void* pData)
{
	m_Data = static_cast<byte*>(pData);
}


RGS::RGS(const pair<vector<void*>, vector<string>>& data) :RGS(data.first, data.second)
{
}

RGS::RGS(const vector<void*>& ptrs, const vector<string>& types) : m_ptrsToCopy(ptrs), m_Data(nullptr)
{
	vector<int> aligned_sizes;
	aligned_sizes.reserve(types.size());
	m_shifts.reserve(types.size());
	m_sizes.reserve(types.size());
	for (auto type : types)
	{
		int size = GetSize(type);
		int aligned_size = struct_alignment;
		if (!size)
			throw std::exception("Unsupported type passed to RGS");
		//m_sizes.push_back(size);
		if (size % struct_alignment != 0)
		{
			while (aligned_size < size)
			{
				aligned_size += struct_alignment;
			}
		}
		else
		{
			aligned_size = size;
		}
		m_size += aligned_size;
		m_sizes.push_back(aligned_size);
		aligned_sizes.push_back(aligned_size);
	}

	int aligned_size = memory_alignment;
	if (m_size % memory_alignment != 0)
	{
		while (aligned_size < m_size)
		{
			aligned_size += 16;
		}
		m_size = aligned_size;
	}
	

	int shift = 0;
	m_shifts.push_back(shift);

	for (int i = 0; i < m_sizes.size() - 1; i++)
	{
		shift += aligned_sizes[i];
		m_shifts.push_back(shift);
	}

	Update();
}

void RGS::Update()const
{
	if (m_Data)
	{
		int i = 0;
		for (auto ptr : m_ptrsToCopy)
		{
			memcpy(m_Data + m_shifts[i], ptr, m_sizes[i]);
			i++;
		}
	}
}

int RGS::Size()const
{
    return m_size;
}

byte* RGS::Data()const
{
    return m_Data;
}

RGS::~RGS()
{
    free(m_Data);
}
