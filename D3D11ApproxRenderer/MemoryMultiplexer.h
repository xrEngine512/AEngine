#pragma once
#include <vector>
#include "GenericType.h"
using namespace std;

typedef unsigned char byte;
namespace ShaderSystem
{
	const int struct_alignment = 16;
	const int memory_alignment = 16;
	class MemoryMultiplexer
	{
		vector<void*> m_ptrsToCopy;
		vector<int> m_sizes, m_shifts;

		byte* m_Data;
		int m_size;
	public:
		MemoryMultiplexer();
		void setInput(const vector<pair<void*, GenericType>>& data);
		void setOutput(void* out);
		void process();
		int size();
		~MemoryMultiplexer();
	};
}
