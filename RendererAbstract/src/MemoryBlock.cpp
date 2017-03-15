#include "MemoryBlock.h"

using namespace std;

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

	size_t MemoryBlock::Size(bool aligned) const
	{
		return size;
	}

	void MemoryBlock::setSize(size_t size)
	{
		this->size = size;
	}

	MemoryBlock::MemoryBlock(MemoryBlock && other) {
		data = other.data;
		size = other.size;
		other.data = nullptr;
		other.size = 0;
	}

    void MemoryBlock::operator=(const MemoryBlock & other) {
		if (this != &other)
			MemoryBlock(other);
    }

    void MemoryBlock::operator=(MemoryBlock && other) {
		if (this != &other)
			MemoryBlock(move(other));
    }

	MemoryBlock::MemoryBlock(const MemoryBlock & other): data(other.data), size(other.size) {

	}
}