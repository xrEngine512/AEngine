#pragma once
#include <string>
#include "ISaveData.h"

namespace ASL
{
	struct ShaderParamInfo : ISaveData
	{
		int ID;
		std::string Type;
		std::string Name;
		const void* Serialize(int& size) override
		{
			/*if (serializedBuf)
				delete[] serializedBuf;
			size = 0;
			size += sizeof(int) * 2;
			size += Type.size();
			size += Name.size();
			serializedBuf = new char[size];
			memcpy(serializedBuf, &ID, sizeof(int));
			int sizeT = Type.size();
			memcpy(serializedBuf + sizeof(int), &sizeT, sizeof(int));
			memcpy(serializedBuf + sizeof(int)*2, Type.c_str(), Type.size());
			memcpy(serializedBuf + sizeof(int)*2 + Type.size(), Name.c_str(), Name.size());
			return serializedBuf;*/

			size = Serialization(ID, Type, Name);
			return SerializedBuf();
		}
		void Deserialize(void* buf, size_t size) override
		{
			/*int sz;
			memcpy(&sz, buf, sizeof(int));
			size -= sizeof(int);
			Type.reserve(sz);
			Type.assign(sz, '0');
			Type.copy(static_cast<char*>(buf)+sizeof(int), sz);
			size -= sz;
			Name.copy(static_cast<char*>(buf)+sizeof(int) + sz, size);*/
			Deserialization(buf, size, ID, Type, Name);
		}
		ShaderParamInfo* ToShaderParameterInfo()override
		{
			return this;
		}
	};
}