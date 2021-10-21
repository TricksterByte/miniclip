#pragma once

#include <array>

#include "../../core/ecs/entity.hpp"
#include "../../core/constants.hpp"

namespace puyo
{
	struct grid final
	{
		std::array<entity, grid_size> board_blobs;
	};
}
