#pragma once

#include "../../../wrapper/math/point.hpp"

namespace puyo
{
	struct transform final
	{
		sdl::fpoint position;
		sdl::ipoint grid_position;
	};
}
