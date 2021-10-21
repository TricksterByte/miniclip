#pragma once

#include <vector>

#include "../../core/ecs/entity.hpp"

namespace puyo
{
	struct falling final
	{
		std::vector<entity> pieces;
	};
}
