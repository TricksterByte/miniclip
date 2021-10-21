#pragma once

#include <algorithm>
#include <iterator>

#include "../../core/constants.hpp"
#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/belonging_chain.hpp"
#include "../../components/gameplay/chains.hpp"
#include "../../components/gameplay/falling.hpp"
#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/score.hpp"
#include "../../components/gameplay/state.hpp"
#include "../../components/movement/transform.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			void delete_blob(coordinator &coord, entity &gr, entity &e)
			{
				auto &g = coord.get_component<grid>(gr);
				auto &t = coord.get_component<transform>(e);
				g.board_blobs[t.grid_position.x() + t.grid_position.y() * grid_width] = 0u;
				coord.destroy_entity(e);
			}

			void set_falling(coordinator &coord, entity &gr, entity &fall, entity &ch, entity &e)
			{
				auto &g = coord.get_component<grid>(gr);
				auto &c = coord.get_component<chains>(ch);
				auto &f = coord.get_component<falling>(fall);

				auto &t = coord.get_component<transform>(e);
				int
					x = t.grid_position.x(),
					y = t.grid_position.y() - 1;

				auto &b = coord.get_component<belonging_chain>(e);
				
				entity up = g.board_blobs[x + y * grid_width];

				while (y > -1 && up != 0u)
				{
					auto &bu = coord.get_component<belonging_chain>(up);

					if (bu.chain != 0u)
						break;

					auto &s = coord.get_component<state>(up);
					s.blob_state = state_t::dropping;
					f.pieces.push_back(up);

					up = g.board_blobs[x + (--y) * grid_width];
				}
			}
		}

		void clear_chains(coordinator &coord, entity &gr, entity &fall, entity &ch, entity &sc)
		{
			auto &c = coord.get_component<chains>(ch);
			auto &s = coord.get_component<score>(sc);

			int multiplier = 10;

			for (auto &pair : c.blob_chains)
			{
				std::for_each(pair.second.begin(), pair.second.end(),
					[&](entity &e)
					{
						set_falling(coord, gr, fall, ch, e);
						delete_blob(coord, gr, e);
					}
				);

				s.current += multiplier * pair.second.size();
				multiplier += 10;

				coord.destroy_entity(pair.first);
			}

			c.blob_chains.clear();
		}
	}
}
