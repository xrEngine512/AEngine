#pragma once

#include "GenericType.h"

#include <vector>
#include <stdint.h>

typedef unsigned char byte;
namespace ShaderSystem
{
	const int struct_alignment = 16;
	const int memory_alignment = 16;
	class MemoryMultiplexer
	{
		std::vector<void*> pointers_to_copy;
		std::vector<uint32_t> sizes, shifts;

		uint64_t size;
	public:
		MemoryMultiplexer();
		void set_input(const std::vector<std::pair<void *, GenericType>> & data);
		void write_to(void * out) const;
		void*& input(int index);
		uint64_t get_size() const;
		~MemoryMultiplexer();
	};
}
