#pragma once
#include <string>
#include <msgpack.hpp>

namespace ASL
{
	struct TextureInfo
	{
		uint8_t     slot;
		std::string Name;

		MSGPACK_DEFINE(slot, Name)
	};
}