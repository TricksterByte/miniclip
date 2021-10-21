#pragma once

#include "../../core/constants.hpp"
#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/pair.hpp"
#include "../../components/gameplay/state.hpp"
#include "../../components/graphics/drawable.hpp"
#include "../../components/movement/transform.hpp"
#include "../../components/movement/velocity.hpp"

#include "../../../common/log.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			enum class relation_t
			{
				up,
				down,
				side
			};

			relation_t get_relation(int xc, int yc, int xo, int yo)
			{
				if (xc == xo)
				{
					if (yc > yo)
						return relation_t::up;
					else if (yc < yo)
						return relation_t::down;
				}
				return relation_t::side;
			}

			bool can_move(coordinator &coord, entity &gr, int xc, int yc, int xo, int yo)
			{
				if (yc >= grid_height || yo >= grid_height)
					return false;
				else
				{
					auto &g = coord.get_component<grid>(gr);

					relation_t rel = get_relation(xc, yc, xo, yo);
					bool check = (rel == relation_t::up && g.board_blobs[xc + yc * grid_width] == 0u) || (rel == relation_t::down && g.board_blobs[xo + yo * grid_width] == 0u) || (g.board_blobs[xc + yc * grid_width] == 0u && g.board_blobs[xo + yo * grid_width] == 0u);

					return check;
				}
			}
		}

		bool try_move_pair(coordinator &coord, entity &e, entity &gr, float dt)
		{
			auto &p = coord.get_component<pair>(e);

			auto &sc = coord.get_component<state>(p.center);
			auto &so = coord.get_component<state>(p.other);

			auto &tc = coord.get_component<transform>(p.center);
			auto &vc = coord.get_component<velocity>(p.center);

			tc.position.set_y(tc.position.y() + dt * vc.speed);
			int yc = static_cast<int>(tc.position.y() / y_interval<>);

			auto &to = coord.get_component<transform>(p.other);
			auto &vo = coord.get_component<velocity>(p.other);

			to.position.set_y(to.position.y() + dt * vo.speed);
			int yo = static_cast<int>(to.position.y() / y_interval<>);

			if (yc == tc.grid_position.y() && sc.blob_state == state_t::dropping && yo == to.grid_position.y() && so.blob_state == state_t::dropping)
			{
				return true;
			}
			else if (yc != tc.grid_position.y() && yo != to.grid_position.y() && can_move(coord, gr, tc.grid_position.x(), yc, to.grid_position.x(), yo))
			{
				auto &g = coord.get_component<grid>(gr);
				g.board_blobs[tc.grid_position.x() + tc.grid_position.y() * grid_width] = 0u;
				g.board_blobs[to.grid_position.x() + to.grid_position.y() * grid_width] = 0u;

				// update grid positions
				tc.grid_position.set_y(yc);
				to.grid_position.set_y(yo);

				// set blobs in grid
				g.board_blobs[tc.grid_position.x() + yc * grid_width] = p.center;
				g.board_blobs[to.grid_position.x() + yo * grid_width] = p.other;

				return true;
			}

			//sc.blob_state = state_t::placed;
			//so.blob_state = state_t::placed;

			return false;
		}
	}
}
