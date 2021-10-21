#pragma once

#include "../../core/ecs/entity.hpp"
#include "../../core/ecs/coordinator.hpp"

#include "../../components/gameplay/falling.hpp"
#include "../../components/gameplay/pair.hpp"

namespace puyo
{
	namespace sys
	{
		void add_falling_pair(coordinator &coord, entity &fall, entity &e)
		{
			auto &f = coord.get_component<falling>(fall);
			auto &p = coord.get_component<pair>(e);

			f.pieces.push_back(p.center);
			f.pieces.push_back(p.other);
		}
	}
}
