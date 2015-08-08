#include "Memory.h"
#include "MemoryBlock.h"

using Byte = unsigned char;
namespace ShaderSystem
{
	Memory::Memory() :m_pData(nullptr), m_Size(0)
	{}

	void Memory::setSize(size_t size)
	{
		size = AlignedSize(size);
		if (size > m_Size)
		{
			m_pData = realloc(m_pData, size);
			int pos = 0;
			for (auto block : m_Blocks)
			{
				block.setData(static_cast<Byte*>(m_pData)+pos);
				pos += block.Size();
			}
			m_Size = size;
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

	const void* Memory::MemoryPtr() const
	{
		return m_pData;
	}

	size_t Memory::Size() const
	{
		return m_Size;
	}

	MemoryBlock& Memory::addBlock(GenericType type)
	{
		return addBlock(static_cast<size_t>(type));
	}

	MemoryBlock& Memory::addBlock(size_t blockSize)
	{
		MemoryBlock block;
		block.setSize(blockSize);
		m_Blocks.push_back(block);
		expand();
		m_Blocks.back().setData(static_cast<Byte*>(m_pData) + m_Size - block.Size(true));
		return m_Blocks.back();
	}

	Memory::~Memory()
	{
		if (m_pData)
			free(m_pData);
	}

	void Memory::expand()
	{
		size_t size = 0;
		for (auto block : m_Blocks)
		{
			size += block.Size(true);
		}
		setSize(size);
	}
}