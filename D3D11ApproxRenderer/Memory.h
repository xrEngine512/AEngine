#pragma once
#include "GenericType.h"

#include <vector>

using std::vector;

namespace ShaderSystem
{
	struct MemoryBlock;

	class Memory
	{
	public:
		Memory();
		void setSize(size_t size);
		const MemoryBlock& operator[](int index)const;
		MemoryBlock& operator[](int index);
		const void* MemoryPtr()const;
		size_t Size()const;
		MemoryBlock& addBlock(GenericType type);
		MemoryBlock& addBlock(size_t blockSize);
		~Memory();
	private:
		void expand();

		vector<MemoryBlock> m_Blocks;
		void* m_pData;
		size_t m_Size;
	};
}