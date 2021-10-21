#pragma once

#include <tuple>

#include "../../core/ecs/entity.hpp"
#include "../../core/ecs/coordinator.hpp"
#include "../../core/input.hpp"
#include "../../core/constants.hpp"

#include "../../ctx/binds.hpp"

#include "../../components/gameplay/grid.hpp"
#include "../../components/gameplay/pair.hpp"
#include "../../components/movement/transform.hpp"
#include "../../components/movement/velocity.hpp"

#include "../../ctx/binds.hpp"

namespace puyo
{
	namespace sys
	{
		namespace
		{
			enum class shape_t
			{
				left,
				right,
				up,
				down
			};

			shape_t get_shape(int xc, int yc, int xo, int yo)
			{
				if (xc < xo && yc == yo)
					return shape_t::right;
				else if (xc > xo && yc == yo)
					return shape_t::left;
				else if (xc == xo && yc > yo)
					return shape_t::up;
				else
					return shape_t::down;
			}
		}

		namespace
		{
			enum class button_t
			{
				pressed,
				released
			};

			std::tuple<bool, button_t> check_pressed_velocity(const sdl::keyboard &keyboard)
			{
				if (keyboard.is_held(ctx::binds::down))
					return std::make_tuple(true, button_t::pressed);
				else if (keyboard.just_released(ctx::binds::down))
					return std::make_tuple(true, button_t::released);

				return std::make_tuple(false, button_t::released);
			}

			void change_velocity(coordinator &coord, entity &e, button_t state)
			{
				auto &p = coord.get_component<pair>(e);

				auto &vc = coord.get_component<velocity>(p.center);
				auto &vo = coord.get_component<velocity>(p.other);

				switch (state)
				{
					case (button_t::pressed):
					{
						vc.speed = vo.speed = speed * 2.f;
						break;
					}
					case (button_t::released):
					{
						vc.speed = vo.speed = speed;
					}
				}
			}
		}

		namespace
		{
			enum class direction_t
			{
				left,
				right
			};

			bool can_move(coordinator &coord, entity &gr, entity &e, direction_t dir)
			{
				auto &p = coord.get_component<pair>(e);

				auto &tc = coord.get_component<transform>(p.center);
				auto &to = coord.get_component<transform>(p.other);

				std::size_t
					xc = tc.grid_position.x(),
					yc = tc.grid_position.y(),
					xo = to.grid_position.x(),
					yo = to.grid_position.y();

				shape_t shape = get_shape(xc, yc, xo, yo);

				auto &g = coord.get_component<grid>(gr);

				switch (dir)
				{
					case direction_t::left:
					{
						if (xc > 0 && xo > 0)
						{
							if (shape == shape_t::left && g.board_blobs[(xo - 1) + yo * grid_width] != 0u)
								return false;
							else if (shape == shape_t::right && g.board_blobs[(xc - 1) + yc * grid_width] != 0u)
								return false;
							else if ((shape == shape_t::up || shape == shape_t::down) && (g.board_blobs[(xc - 1 + yc * grid_width)] != 0u || g.board_blobs[(xo - 1) + yo * grid_width] != 0u))
								return false;
							else
								return true;
						}
						return false;
					}
					
					case direction_t::right:
					{
						if (xc < grid_width - 1 && xo < grid_width - 1)
						{
							if (shape == shape_t::left && g.board_blobs[(xc + 1) + yc * grid_width] != 0u)
								return false;
							else if (shape == shape_t::right && g.board_blobs[(xo + 1) + yo * grid_width] != 0u)
								return false;
							else if ((shape == shape_t::up || shape == shape_t::down) && (g.board_blobs[(xc + 1 + yc * grid_width)] != 0u || g.board_blobs[(xo + 1) + yo * grid_width] != 0u))
								return false;
							else
								return true;
						}
						return false;
					}
				}

				return false;
			}

			std::tuple<bool, direction_t> check_pressed_move(const sdl::keyboard &keyboard)
			{
				const auto left = keyboard.just_pressed(ctx::binds::left);
				const auto right = keyboard.just_pressed(ctx::binds::right);

				return std::make_tuple(left != right, left ? direction_t::left : direction_t::right);
			}

			bool can_rotate(coordinator &coord, entity &gr, entity &e, direction_t dir)
			{
				auto &p = coord.get_component<pair>(e);

				auto &tc = coord.get_component<transform>(p.center);
				auto &to = coord.get_component<transform>(p.other);

				std::size_t
					xc = tc.grid_position.x(),
					yc = tc.grid_position.y(),
					xo = to.grid_position.x(),
					yo = to.grid_position.y();

				shape_t shape = get_shape(xc, yc, xo, yo);

				auto &g = coord.get_component<grid>(gr);

				switch (dir)
				{
					case direction_t::left:
					{
						if (shape == shape_t::right && yc != 0)
							return true;
						else if (shape == shape_t::up && xc > 0 && g.board_blobs[(xc - 1) + yc * grid_width] == 0u)
							return true;
						else if (shape == shape_t::left && yc < grid_height - 1 && g.board_blobs[xc + (yc + 1) *grid_width] == 0u)
							return true;
						else if (shape == shape_t::down && xc < grid_width - 1 && g.board_blobs[(xc + 1) + yc * grid_width] == 0u)
							return true;
						else
							return false;
					}

					case direction_t::right:
					{
						if (shape == shape_t::right && yc < grid_height - 1 && g.board_blobs[xc + (yc + 1) * grid_width] == 0u)
							return true;
						else if (shape == shape_t::up && xc < grid_width - 1 && g.board_blobs[(xc + 1) + yc * grid_width] == 0u)
							return true;
						else if (shape == shape_t::left && yc != 0)
							return true;
						else if (shape == shape_t::down && xc > 0 && g.board_blobs[(xc - 1) + yc * grid_width] == 0u)
							return true;
						else
							return false;
					}
				}

				return false;
			}

			std::tuple<bool, direction_t> check_pressed_rotate(const sdl::keyboard &keyboard)
			{
				const auto left = keyboard.just_pressed(ctx::binds::rotl);
				const auto right = keyboard.just_pressed(ctx::binds::rotr);

				return std::make_tuple(left != right, left ? direction_t::left : direction_t::right);
			}

			void move(coordinator &coord, entity &gr, entity &e, direction_t dir)
			{
				auto &p = coord.get_component<pair>(e);

				auto &tc = coord.get_component<transform>(p.center);
				auto &to = coord.get_component<transform>(p.other);

				std::size_t
					xc = tc.grid_position.x(),
					yc = tc.grid_position.y(),
					xo = to.grid_position.x(),
					yo = to.grid_position.y();

				shape_t shape = get_shape(xc, yc, xo, yo);

				auto &g = coord.get_component<grid>(gr);

				g.board_blobs[xc + yc * grid_width] = 0u;
				g.board_blobs[xo + yo * grid_width] = 0u;

				switch (dir)
				{
					case direction_t::left:
					{
						tc.position.set_x(tc.position.x() - x_interval<>);
						tc.grid_position.set_x(--xc);
						to.position.set_x(to.position.x() - x_interval<>);
						to.grid_position.set_x(--xo);

						break;
					}

					case direction_t::right:
					{
						tc.position.set_x(tc.position.x() + x_interval<>);
						tc.grid_position.set_x(++xc);
						to.position.set_x(to.position.x() + x_interval<>);
						to.grid_position.set_x(++xo);

						break;
					}
				}

				g.board_blobs[xc + yc * grid_width] = p.center;
				g.board_blobs[xo + yo * grid_width] = p.other;
			}

			void rotate(coordinator &coord, entity &gr, entity &e, direction_t dir)
			{
				auto &p = coord.get_component<pair>(e);

				auto &tc = coord.get_component<transform>(p.center);
				auto &to = coord.get_component<transform>(p.other);

				std::size_t
					xc = tc.grid_position.x(),
					yc = tc.grid_position.y(),
					xo = to.grid_position.x(),
					yo = to.grid_position.y();

				shape_t shape = get_shape(xc, yc, xo, yo);

				auto &g = coord.get_component<grid>(gr);

				g.board_blobs[xo + yo * grid_width] = 0u;

				switch (dir)
				{
					case direction_t::left:
					{
						if (shape == shape_t::right)
						{
							to.position.set_x(to.position.x() - x_interval<>);
							to.grid_position.set_x(--xo);
							to.position.set_y(to.position.y() - y_interval<>);
							to.grid_position.set_y(--yo);
						}
						else if (shape == shape_t::up)
						{
							to.position.set_x(to.position.x() - x_interval<>);
							to.grid_position.set_x(--xo);
							to.position.set_y(to.position.y() + y_interval<>);
							to.grid_position.set_y(++yo);
						}
						else if (shape == shape_t::left)
						{
							to.position.set_x(to.position.x() + x_interval<>);
							to.grid_position.set_x(++xo);
							to.position.set_y(to.position.y() + y_interval<>);
							to.grid_position.set_y(++yo);
						}
						else if (shape == shape_t::down)
						{
							to.position.set_x(to.position.x() + x_interval<>);
							to.grid_position.set_x(++xo);
							to.position.set_y(to.position.y() - y_interval<>);
							to.grid_position.set_y(--yo);
						}

						break;
					}

					case direction_t::right:
					{
						if (shape == shape_t::right)
						{
							to.position.set_x(to.position.x() - x_interval<>);
							to.grid_position.set_x(--xo);
							to.position.set_y(to.position.y() + y_interval<>);
							to.grid_position.set_y(++yo);
						}
						else if (shape == shape_t::up)
						{
							to.position.set_x(to.position.x() + x_interval<>);
							to.grid_position.set_x(++xo);
							to.position.set_y(to.position.y() + y_interval<>);
							to.grid_position.set_y(++yo);
						}
						else if (shape == shape_t::left)
						{
							to.position.set_x(to.position.x() + x_interval<>);
							to.grid_position.set_x(++xo);
							to.position.set_y(to.position.y() - y_interval<>);
							to.grid_position.set_y(--yo);
						}
						else if (shape == shape_t::down)
						{
							to.position.set_x(to.position.x() - x_interval<>);
							to.grid_position.set_x(--xo);
							to.position.set_y(to.position.y() - y_interval<>);
							to.grid_position.set_y(--yo);
						}

						break;
					}
				}

				g.board_blobs[xo + yo * grid_width] = p.other;
			}
		}

		void handle_pair_input(coordinator &coord, entity &gr, entity &e, const input &input)
		{
			if (auto [check, dir] = check_pressed_move(input.keyboard); check)
			{
				if (can_move(coord, gr, e, dir))
				{
					move(coord, gr, e, dir);
				}
			}
			else if (auto [check, dir] = check_pressed_rotate(input.keyboard); check)
			{
				if (can_rotate(coord, gr, e, dir))
				{
					rotate(coord, gr, e, dir);
				}
			}
			else if (auto [check, state] =  check_pressed_velocity(input.keyboard); check)
			{
				change_velocity(coord, e, state);
			}
		}
	}
}
