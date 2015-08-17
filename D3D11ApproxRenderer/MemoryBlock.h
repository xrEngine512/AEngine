#pragma once

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
		int Size(bool aligned = false)const;
		void setSize(int size);

	private:
		void* data = nullptr;
		int size = 0;
	};
}