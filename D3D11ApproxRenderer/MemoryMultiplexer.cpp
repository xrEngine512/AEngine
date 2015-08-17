#include "MemoryMultiplexer.h"
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
namespace ShaderSystem
{
	/*void MemoryMultiplexer::operator = (void* pData)
	{
		m_Data = static_cast<byte*>(pData);
	}

	MemoryMultiplexer::MemoryMultiplexer(const vector<pair<void*, GenericType>>& data)
	{
		

		Update();
	}

	MemoryMultiplexer::MemoryMultiplexer(const vector<void*>& ptrs, const vector<GenericType>& types) : m_ptrsToCopy(ptrs), m_Data(nullptr)
	{
		vector<int> sizes;
		sizes.reserve(types.size());
		m_shifts.reserve(types.size());
		m_sizes.reserve(types.size());
		for (auto type : types)
		{
			int size = type;
			sizes.push_back(size);
			int aligned_size = struct_alignment;
			if (!size)
				throw std::exception("Unsupported type passed to MemoryMultiplexer");
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
		m_Vars.push_back(RGSV(m_Data, static_cast<GenericType>(sizes[0])));
		for (int i = 0; i < m_sizes.size() - 1; i++)
		{
			shift += m_sizes[i];
			m_shifts.push_back(shift);
			m_Vars.push_back(RGSV(m_Data + shift, static_cast<GenericType>(sizes[i + 1])));
		}

		Update();
	}
	//ÝÒÎ ÁÐÅÄ ÍÀÄÎ ÏÐÈÂÅÑÒÈ Â ÏÎÐßÄÎÊ
	MemoryMultiplexer::MemoryMultiplexer(const vector<pair<GenericType, string>>& TypesAndNames)
	{
		vector<int> sizes;
		sizes.reserve(TypesAndNames.size());
		m_shifts.reserve(TypesAndNames.size());
		m_sizes.reserve(TypesAndNames.size());
		for (auto type : TypesAndNames)
		{
			int size = type.first;
			sizes.push_back(size);
			int aligned_size = struct_alignment;
			if (!size)
				throw std::exception("Unsupported type passed to MemoryMultiplexer");
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
		m_Data = new byte[m_size];

		int shift = 0;
		m_shifts.push_back(shift);

		m_Vars.push_back(RGSV(m_Data, static_cast<GenericType>(sizes[0])));
		m_varsByName.insert(pair<string, RGSV*>(TypesAndNames[0].second, &m_Vars[0]));
		for (int i = 0; i < m_sizes.size() - 1; i++)
		{
			shift += m_sizes[i];
			m_Vars.push_back(RGSV(m_Data, static_cast<GenericType>(sizes[i + 1])));
			m_varsByName.insert(pair<string, RGSV*>(TypesAndNames[i + 1].second, &m_Vars[i + 1]));
			m_shifts.push_back(shift);
		}
	}*/

	void MemoryMultiplexer::process()
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

	int MemoryMultiplexer::size()
	{
		return m_size;
	}

	MemoryMultiplexer::MemoryMultiplexer() :m_Data(nullptr), m_size(0)
	{
	}

	void MemoryMultiplexer::setInput(const vector<pair<void*, GenericType>>& data)
	{
		vector<int> sizes;
		sizes.reserve(data.size());
		m_shifts.reserve(data.size());
		m_sizes.reserve(data.size());
		int vector16 = 0;
		int shift = 0;
		for (auto type : data)
		{
			int size = type.second;

			if (vector16 >= size)
			{
				vector16 -= size;
				m_shifts.push_back(shift);
				m_sizes.push_back(size);
				shift += size;
			}
			else
			{
				if (vector16 == struct_alignment)
				{
					m_shifts.push_back(shift);
					m_sizes.push_back(size);
					shift += size;
				}
				else if (vector16 == 0)
				{
					m_shifts.push_back(shift);
					m_sizes.push_back(size);
					shift += size;
					vector16 = struct_alignment - size;
					if (vector16 < 0) vector16 = 0;
				}
				else
				{
					m_shifts.push_back(shift + vector16);
					m_sizes.push_back(size + vector16);
					shift += size + vector16;
					vector16 = struct_alignment;
				}
			}
			m_ptrsToCopy.push_back(type.first);
		}
		m_size = (shift / struct_alignment) * struct_alignment;
		if (shift > m_size) m_size += struct_alignment;
	}

	void MemoryMultiplexer::setOutput(void* out)
	{
		m_Data = static_cast<byte*>(out);
	}

	void*& MemoryMultiplexer::Input(int index)
	{
		return m_ptrsToCopy[index];
	}

	MemoryMultiplexer::~MemoryMultiplexer()
	{
	}
}