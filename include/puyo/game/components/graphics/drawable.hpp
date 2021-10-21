#pragma once

#include <cstdint>

#include "../../../wrapper/math/rect.hpp"

namespace puyo
{
	struct drawable final
	{
		std::uint8_t texture;
		sdl::irect src;
		sdl::frect dst;
	};
}
