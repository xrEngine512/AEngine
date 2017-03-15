#pragma once
#include <vector>
#include "GenericType.h"


typedef unsigned char byte;
namespace ShaderSystem
{
	const int struct_alignment = 16;
	const int memory_alignment = 16;
	class MemoryMultiplexer
	{
		std::vector<void*> m_ptrsToCopy;
		std::vector<int> m_sizes, m_shifts;

		byte* m_Data;
		int m_size;
	public:
		MemoryMultiplexer();
		void setInput(const std::vector<std::pair<void*, GenericType>>& data);
		void setOutput(void* out);
		void*& Input(int index);
		void process();
		int size();
		~MemoryMultiplexer();
	};
}
