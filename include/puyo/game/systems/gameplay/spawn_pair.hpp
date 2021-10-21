#pragma once

#include <random>

#include "../../core/constants.hpp"
#include "../../core/ecs/entity.hpp"
#include "../../core/ecs/coordinator.hpp"

#include "../../components/gameplay/color.hpp"
#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/pair.hpp"
#include "../../components/gameplay/state.hpp"
#include "../../components/graphics/drawable.hpp"
#include "../../components/movement/transform.hpp"
#include "../../components/movement/velocity.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			void populate_blob(coordinator &coord, entity &blob, color_t col, sdl::ipoint pos)
			{
				coord.add_component<color>(blob, { col });

				coord.add_component<belonging_chain>(blob, { 0 });

				coord.add_component<state>(blob, { state_t::dropping });

				coord.add_component<transform>(blob, {
						{ float(pos.x()) * x_interval<>, float(pos.y()) * y_interval<> },
						{ pos.x(), pos.y() }
				});

				coord.add_component<velocity>(blob, { puyo::speed });

				coord.add_component<drawable>(blob, {
					0,
					{ { (col - 1) * 50, 0 }, { 50, 50 } },
					{ { float(pos.x()) * x_interval<>, float(pos.y()) * y_interval<> }, { x_interval<>, x_interval<> } }
				});
			}
		}

		void spawn_pair(coordinator &coord, entity &e)
		{
			e = coord.create_entity();
			coord.add_component<pair>(e, {
				coord.create_entity(),
				coord.create_entity()
			});

			auto &p = coord.get_component<pair>(e);

			std::random_device r;
			std::default_random_engine eng(r());
			std::uniform_int_distribution<int> dist(1, 4);

			populate_blob(coord, p.center, static_cast<color_t>(dist(eng)), { 0, 0 });
			populate_blob(coord, p.other, static_cast<color_t>(dist(eng)), { 0, 1 });
		}
	}
}
