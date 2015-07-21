#pragma once
#include <string>
#include "ISaveData.h"

namespace ASL
{
	struct TextureInfo : ISaveData
	{
		int Slot;
		std::string Name;
		void* Serialize(int& size) override
		{
			if (serializedBuf)
				delete[] serializedBuf;
			size = 0;
			size += sizeof(int);
			size += Name.size();
			serializedBuf = new char[size];
			memcpy(serializedBuf, &Slot, sizeof(int));
			memcpy(serializedBuf + sizeof(int), Name.c_str(), Name.size());
			return serializedBuf;
		}
		void Deserialize(void* buf, size_t size)override
		{
			memcpy(&Slot, buf, sizeof(int));
			size -= sizeof(int);
			Name.reserve(size);
			Name.assign(size, '0');
			Name.copy(static_cast<char*>(buf)+sizeof(int), size);
		}
		TextureInfo* ToTextureInfo()override
		{
			return this;
		}
	};
}