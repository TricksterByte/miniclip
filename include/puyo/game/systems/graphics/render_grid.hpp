#pragma once

#include <algorithm>

#include "../../core/ecs/entity.hpp"
#include "../../core/ecs/coordinator.hpp"

#include "../../components/gameplay/grid.hpp"
#include "../../components/graphics/drawable.hpp"
#include "../../components/movement/transform.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			void update_dst(coordinator &coord, entity &e)
			{
				auto &t = coord.get_component<transform>(e);
				auto &d = coord.get_component<drawable>(e);

				d.dst.set_x(t.grid_position.x() * x_interval<>);
				d.dst.set_y(t.grid_position.y() * y_interval<>);
			}
		}

		void render_grid(coordinator &coord, graphics &gfx, entity &gr)
		{
			auto &g = coord.get_component<grid>(gr);

			std::for_each(g.board_blobs.begin(), g.board_blobs.end(),
				[&](entity &e)
				{
					if (e == 0u)
						return;
					update_dst(coord, e);
					auto &d = coord.get_component<drawable>(e);
					gfx.render(d.texture, d.src, d.dst);
				}
			);
		}
	}
}
