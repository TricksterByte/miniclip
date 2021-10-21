#pragma once

#include <algorithm>

#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/belonging_chain.hpp"
#include "../../components/gameplay/chains.hpp"
#include "../../components/gameplay/color.hpp"
#include "../../components/gameplay/grid.hpp"
#include "../../components/movement/transform.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			void check_adjacent(coordinator &coord, entity &gr, entity &ch, entity &e, const sdl::ipoint &p)
			{
				auto &g = coord.get_component<grid>(gr);
				auto &cs = coord.get_component<chains>(ch);

				auto &c = coord.get_component<color>(e);
				auto &t = coord.get_component<transform>(e);
				std::size_t
					x = t.grid_position.x(),
					y = t.grid_position.y();

				entity adj = g.board_blobs[(x + p.x()) + (y + p.y()) * grid_width];

				if (adj == 0u)
					return;

				auto &ca = coord.get_component<color>(adj);

				if (c.blob_color == ca.blob_color)
				{
					auto &b = coord.get_component<belonging_chain>(e);
					auto &ba = coord.get_component<belonging_chain>(adj);

					if (b.chain == 0u)
					{
						if (ba.chain == 0u)
						{
							entity new_chain = coord.create_entity();

							cs.blob_chains[new_chain].push_back(e);
							cs.blob_chains[new_chain].push_back(adj);

							b.chain = new_chain;
							ba.chain = new_chain;
						}
						else
						{
							cs.blob_chains[ba.chain].push_back(e);
							b.chain = ba.chain;
						}
					}
					else
					{
						if (ba.chain == 0u)
						{
							cs.blob_chains[b.chain].push_back(adj);
							ba.chain = b.chain;
						}
						else if (ba.chain != b.chain)
						{
							cs.blob_chains[b.chain].insert(
								cs.blob_chains[b.chain].begin(),
								cs.blob_chains[ba.chain].begin(),
								cs.blob_chains[ba.chain].end()
							);

							cs.blob_chains.erase(ba.chain);
							coord.destroy_entity(ba.chain);

							ba.chain = b.chain;
						}
					}
				}
			}
		}

		void find_combos(coordinator &coord, entity &gr, entity &ch)
		{
			auto &g = coord.get_component<grid>(gr);

			std::for_each(g.board_blobs.begin(), g.board_blobs.end(),
				[&](entity &e)
				{
					if (e == 0u)
						return;

					auto &t = coord.get_component<transform>(e);

					int
						x = t.grid_position.x(),
						y = t.grid_position.y();

					if (x > 0)
						check_adjacent(coord, gr, ch, e, { -1, 0 });
					if (x < grid_width - 1)
						check_adjacent(coord, gr, ch, e, { 1, 0 });
					if (y > 0)
						check_adjacent(coord, gr, ch, e, { 0, -1 });
					if (y < grid_height - 1)
						check_adjacent(coord, gr, ch, e, { 0, 1 });
				}
			);
		}
	}
}
