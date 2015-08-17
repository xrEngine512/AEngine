#include "MemoryBlock.h"

namespace ShaderSystem
{
	void* MemoryBlock::Data() const
	{
		return data;
	}

	void MemoryBlock::setData(void* ptr)
	{
		data = ptr;
	}

	int MemoryBlock::Size(bool aligned) const
	{
		return size;
	}

	void MemoryBlock::setSize(int size)
	{
		this->size = size;
	}
}