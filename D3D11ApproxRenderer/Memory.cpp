#include "Memory.h"

using Byte = unsigned char;
namespace ShaderSystem
{
	inline size_t AlignedSize(size_t size, const int alignment = 16)
	{
		if (size % alignment != 0)
		{
			int aligned_size = (size / alignment) * alignment; //Optimizaton may affect
			while (aligned_size < size)
			{
				aligned_size += alignment;
			}
			return aligned_size;
		}
		return size;
	}

	Memory::Memory() :m_pData(nullptr), m_allocatedSize(0), m_usedSize(0)
	{}

	Memory::Memory(const Memory& mem)
	{
		*this = mem;
	}

	void Memory::setSize(size_t size)
	{
		if (size > m_allocatedSize)
		{
			m_pData = realloc(m_pData, size);
			int pos = 0;
			for (auto& block : m_Blocks)
			{
				block.setData(static_cast<Byte*>(m_pData)+pos);
				pos += block.Size();
			}
			m_allocatedSize = size;
		}
	}

	const MemoryBlock& Memory::operator[](int index)const
	{
		return m_Blocks[index];
	}

	MemoryBlock& Memory::operator[](int index)
	{
		return m_Blocks[index];
	}

	void Memory::operator=(const Memory& mem)
	{
		if (this != &mem)
		{
			if (mem.m_allocatedSize > m_allocatedSize)
			{
				m_allocatedSize = mem.m_allocatedSize;
				m_pData = realloc(m_pData, m_allocatedSize);
			}
			m_usedSize = mem.m_usedSize;
			memcpy(m_pData, mem.m_pData, m_allocatedSize);
			m_Blocks = mem.m_Blocks;
			int pos = 0;
			for (auto& block : m_Blocks)
			{
				block.setData(static_cast<Byte*>(m_pData)+pos);
				pos += block.Size();
			}
		}
	}

	const void* Memory::MemoryPtr() const
	{
		return m_pData;
	}

	size_t Memory::Size(bool allocated) const
	{
		return allocated ? m_allocatedSize : m_usedSize;
	}

	MemoryBlock& Memory::addBlock(GenericType type)
	{
		return addBlock(static_cast<size_t>(type));
	}

	MemoryBlock& Memory::addBlock(size_t blockSize)
	{
		MemoryBlock block;
		block.setSize(blockSize);
		m_usedSize += blockSize;
		m_Blocks.push_back(block);
		expand();
		m_Blocks.back().setData(static_cast<Byte*>(m_pData) + m_usedSize - block.Size(true));
		return m_Blocks.back();
	}

	Memory::~Memory()
	{
		if (m_pData)
			free(m_pData);
	}

	void Memory::expand()
	{
		setSize(m_usedSize);
	}
}