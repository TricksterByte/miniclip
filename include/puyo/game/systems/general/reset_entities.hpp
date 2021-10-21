#pragma once

#include <algorithm>

#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/chains.hpp"
#include "../../components/gameplay/falling.hpp"
#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/score.hpp"

namespace puyo
{
	namespace sys
	{
		void reset_entites(coordinator &coord, entity &gr, entity &fall, entity &ch, entity &e, entity &sc)
		{
			auto &g = coord.get_component<grid>(gr);

			std::for_each(g.board_blobs.begin(), g.board_blobs.end(),
				[&](entity &e)
				{
					if (e != 0u)
					{
						coord.destroy_entity(e);
						e = 0u;
					}
				}
			);

			auto &c = coord.get_component<chains>(ch);

			std::for_each(c.blob_chains.begin(), c.blob_chains.end(),
				[&](auto &pair)
				{
					coord.destroy_entity(pair.first);
				}
			);

			c.blob_chains.clear();

			auto &f = coord.get_component<falling>(fall);

			f.pieces.clear();

			coord.destroy_entity(gr);
			coord.destroy_entity(fall);
			coord.destroy_entity(ch);
			coord.destroy_entity(e);

			gr = 0u;
			fall = 0u;
			ch = 0u;
			e = 0u;

			auto &s = coord.get_component<score>(sc);
			s.current = 0;
		}
	}
}
