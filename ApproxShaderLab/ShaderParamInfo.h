#pragma once
#include <string>
#include "ISaveData.h"

namespace ASL
{
	struct ShaderParamInfo : ISaveData
	{
		std::string Type;
		std::string Name;
		void* Serialize(int& size) override
		{
			if (serializedBuf)
				delete[] serializedBuf;
			size = 0;
			size += sizeof(int);
			size += Type.size();
			size += Name.size();
			serializedBuf = new char[size];
			int sizeT = Type.size();
			memcpy(serializedBuf, &sizeT, sizeof(int));
			memcpy(serializedBuf + sizeof(int), Type.c_str(), Type.size());
			memcpy(serializedBuf + sizeof(int) + Type.size(), Name.c_str(), Name.size());
			return serializedBuf;
		}
		void Deserialize(void* buf, size_t size) override
		{
			int sz;
			memcpy(&sz, buf, sizeof(int));
			size -= sizeof(int);
			Type.reserve(sz);
			Type.assign(sz, '0');
			Type.copy(static_cast<char*>(buf)+sizeof(int), sz);
			size -= sz;
			Name.copy(static_cast<char*>(buf)+sizeof(int) + sz, size);
		}
		ShaderParamInfo* ToShaderParameterInfo()override
		{
			return this;
		}
	};
}