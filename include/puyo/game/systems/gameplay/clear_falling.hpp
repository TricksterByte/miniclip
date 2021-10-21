#pragma once

#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/falling.hpp"

namespace puyo
{
	namespace sys
	{
		void clear_falling(coordinator &coord, entity &fall)
		{
			auto &f = coord.get_component<falling>(fall);
			f.pieces.clear();
		}
	}
}
