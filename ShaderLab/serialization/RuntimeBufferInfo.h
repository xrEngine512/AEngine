#pragma once
#include <IMaterialVariable.h>

#include <vector>


namespace ASL
{
	struct RuntimeBufferInfo
	{
		MatInterface::VarGroup 	group;
		std::vector<int> 		IDs;


		MSGPACK_DEFINE(group, IDs);
	};
}
