#pragma once

#include "../../core/constants.hpp"
#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/pair.hpp"
#include "../../components/movement/transform.hpp"

namespace puyo
{
	namespace sys
	{
		bool check_lose(coordinator &coord, entity &gr, entity &e)
		{
			auto &g = coord.get_component<grid>(gr);
			auto &p = coord.get_component<pair>(e);

			auto &tc = coord.get_component<transform>(p.center);
			auto &to = coord.get_component<transform>(p.other);

			int
				xc = tc.grid_position.x(),
				yc = tc.grid_position.y(),
				xo = to.grid_position.x(),
				yo = to.grid_position.y();

			if (g.board_blobs[xc + yc * grid_width] != 0u || g.board_blobs[xo + yo * grid_width] != 0u)
				return true;

			g.board_blobs[xc + yc * grid_width] = p.center;
			g.board_blobs[xo + yo * grid_width] = p.other;

			return false;
		}
	}
}
