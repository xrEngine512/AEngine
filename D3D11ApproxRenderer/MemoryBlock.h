#pragma once

const int struct_alignment = 16;
const int memory_alignment = 16;

inline size_t AlignedSize(size_t size, const int alignment = struct_alignment);

namespace ShaderSystem
{
	struct MemoryBlock
	{
		template<typename T>
		void operator=(T val)
		{
			if (sizeof(T) != size)
				throw std::exception("Different sizes during assign");
			memcpy(Data, &val, Size);
		}

		void* Data()const;
		void setData(void* ptr);
		int Size(bool aligned = false);
		void setSize(int size);

	private:
		void* data = nullptr;
		int size = 0, alignedSize = 0;
	};
}