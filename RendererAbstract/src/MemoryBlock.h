#pragma once

#include <cstring>
#include <stdexcept>

namespace ShaderSystem
{
	struct MemoryBlock
	{
		template<typename T>
		void operator=(T val)
		{
			if (sizeof(T) != size)
				throw std::runtime_error("Different sizes during assign");
			std::memcpy(data, &val, size);
		}
        void operator=(MemoryBlock&&);
        void operator=(const MemoryBlock&);
		void* Data()const;
		void setData(void* ptr);
		size_t Size(bool aligned = false)const;
		void setSize(size_t size);
        MemoryBlock() = default;
        MemoryBlock(const MemoryBlock&);
		MemoryBlock(MemoryBlock&&);
	private:
		void* data = nullptr;
		size_t size = 0;
	};
}