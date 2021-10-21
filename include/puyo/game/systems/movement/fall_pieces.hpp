#pragma once

#include <algorithm>

#include "../../core/constants.hpp"
#include "../../core/ecs/entity.hpp"
#include "../../core/ecs/coordinator.hpp"

#include "../../components/gameplay/belonging_chain.hpp"
#include "../../components/gameplay/chains.hpp"
#include "../../components/gameplay/color.hpp"
#include "../../components/gameplay/falling.hpp"
#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/state.hpp"
#include "../../components/movement/transform.hpp"
#include "../../components/movement/velocity.hpp"

namespace puyo
{
	namespace sys
	{
		bool fall_pieces(coordinator &coord, entity &gr, entity &fall, entity &ch, float dt)
		{
			auto &g = coord.get_component<grid>(gr);
			auto &f = coord.get_component<falling>(fall);

			bool res = false;

			std::for_each(f.pieces.begin(), f.pieces.end(),
				[&](entity &e)
				{
					auto &s = coord.get_component<state>(e);

					if (s.blob_state == state_t::placed)
						return;

					auto &t = coord.get_component<transform>(e);
					auto &v = coord.get_component<velocity>(e);

					t.position.set_y(t.position.y() + dt * v.speed);
					int y = static_cast<int>(t.position.y() / y_interval<>);

					if (y == t.grid_position.y() && s.blob_state == state_t::dropping)
					{
						res |= true;
						return;
					}
					else if (y != t.grid_position.y())
					{
						if (y >= grid_height)
							s.blob_state = state_t::placed;
						else if (g.board_blobs[t.grid_position.x() + y * grid_width] != 0u)
						{
							auto &so = coord.get_component<state>(g.board_blobs[t.grid_position.x() + y * grid_width]);
							if (so.blob_state == state_t::placed)
								s.blob_state = state_t::placed;
						}
						else
						{
							g.board_blobs[t.grid_position.x() + t.grid_position.y() * grid_width] = 0u;
							t.grid_position.set_y(y);
							g.board_blobs[t.grid_position.x() + y * grid_width] = e;
						}
					}

					res |= true;
				}
			);

			return res;
		}
	}
}
