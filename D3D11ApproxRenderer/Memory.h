#pragma once
#include "GenericType.h"
#include "MemoryBlock.h"
#include <vector>

using std::vector;

namespace ShaderSystem
{
	class Memory
	{
	public:
		Memory();
		Memory(const Memory&);
		void setSize(size_t size);
		const MemoryBlock& operator[](int index)const;
		MemoryBlock& operator[](int index);
		void operator=(const Memory&);
		const void* MemoryPtr()const;
		size_t Size(bool allocated = true)const;
		MemoryBlock& addBlock(GenericType type);
		MemoryBlock& addBlock(size_t blockSize);
		~Memory();
	private:
		void expand();

		vector<MemoryBlock> m_Blocks;
		void* m_pData;
		size_t m_allocatedSize, m_usedSize;
	};
}