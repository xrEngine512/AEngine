#pragma once
#include <string>
#include <msgpack.hpp>

namespace ASL
{
	struct ShaderParameterInfo
	{
		int id;
		std::string Type;
		std::string Name;

		MSGPACK_DEFINE(id, Type, Name);
	};
}