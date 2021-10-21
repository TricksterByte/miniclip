#pragma once

#include "../../core/ecs/entity.hpp"

namespace puyo
{
	struct pair final
	{
		entity center;
		entity other;
	};
}
