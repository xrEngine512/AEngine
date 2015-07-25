#pragma once
#include <vector>
#include "IMaterialVariable.h"
#include "ISaveData.h"

namespace ASL
{
	struct RuntimeBufferInfo : ISaveData
	{
		MatInterface::VarGroup group;
		std::vector<int> IDs;
		const void* Serialize(int& size)override
		{
			/*if (serializedBuf)
				delete[] serializedBuf;
			size = 0;
			size += sizeof(MatInterface::VarGroup);
			size += IDs.size() * sizeof(int);
			serializedBuf = new char[size];
			memcpy(serializedBuf, &group, sizeof(MatInterface::VarGroup));
			memcpy(serializedBuf + sizeof(MatInterface::VarGroup), IDs.data(), IDs.size() * sizeof(int));
			return serializedBuf;*/
			size = Serialization(group, IDs);
			return SerializedBuf();
		}
		void Deserialize(void* buf, size_t size)override
		{
			/*memcpy(&group, buf, sizeof(MatInterface::VarGroup));
			size -= sizeof(MatInterface::VarGroup);
			IDs.reserve(size / sizeof(int));
			IDs.assign(size / sizeof(int), 0);
			memcpy(IDs.data(), static_cast<char*>(buf)+sizeof(MatInterface::VarGroup), size);*/
			Deserialization(buf, size, group, IDs);
		}
		RuntimeBufferInfo* ToRuntimeBufferInfo()override
		{
			return this;
		}

	};
}
