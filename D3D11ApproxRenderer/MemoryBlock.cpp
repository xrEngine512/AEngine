#include "MemoryBlock.h"

inline size_t AlignedSize(size_t size, const int alignment)
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

	int MemoryBlock::Size(bool aligned)
	{
		return aligned ? alignedSize : size;
	}

	void MemoryBlock::setSize(int size)
	{
		this->size = size; alignedSize = AlignedSize(size);
	}
}