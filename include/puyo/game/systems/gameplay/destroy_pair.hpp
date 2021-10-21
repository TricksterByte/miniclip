#pragma once

#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

namespace puyo
{
	namespace sys
	{
		void destroy_pair(coordinator &coord, entity &p)
		{
			coord.destroy_entity(p);
			p = 0u;
		}
	}
}
