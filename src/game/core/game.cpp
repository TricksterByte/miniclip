#include "puyo/common/log.hpp"
#include "puyo/game/core/game.hpp"

#include "puyo/game/components/gameplay/belonging_chain.hpp"
#include "puyo/game/components/gameplay/color.hpp"
#include "puyo/game/components/gameplay/falling.hpp"
#include "puyo/game/components/gameplay/grid.hpp"
#include "puyo/game/components/gameplay/chains.hpp"
#include "puyo/game/components/gameplay/pair.hpp"
#include "puyo/game/components/gameplay/score.hpp"
#include "puyo/game/components/gameplay/state.hpp"
#include "puyo/game/components/graphics/drawable.hpp"
#include "puyo/game/components/movement/transform.hpp"
#include "puyo/game/components/movement/velocity.hpp"

#include "puyo/game/systems/gameplay/add_falling_pair.hpp"
#include "puyo/game/systems/gameplay/check_lose.hpp"
#include "puyo/game/systems/gameplay/clear_chains.hpp"
#include "puyo/game/systems/gameplay/clear_falling.hpp"
#include "puyo/game/systems/gameplay/destroy_pair.hpp"
#include "puyo/game/systems/gameplay/filter_chains.hpp"
#include "puyo/game/systems/gameplay/find_combos.hpp"
#include "puyo/game/systems/gameplay/spawn_pair.hpp"
#include "puyo/game/systems/graphics/render_grid.hpp"
#include "puyo/game/systems/general/reset_entities.hpp"
#include "puyo/game/systems/input/handle_general_input.hpp"
#include "puyo/game/systems/input/handle_pair_input.hpp"
#include "puyo/game/systems/movement/fall_pieces.hpp"
#include "puyo/game/systems/movement/try_move_pair.hpp"

namespace puyo
{
	void game::handle_input(const input &input)
	{
		if (sys::handle_general_input(_paused, input))
			_reset_game();

		if (!_paused)
		{
			if (_state == _game_state::pair)
				sys::handle_pair_input(_coord, _grid, _pair, input);
		}
	}

	void game::tick(float dt)
	{
		if (!_paused)
		{
			switch (_state)
			{
			case _game_state::pair:
			{
				if (_pair == 0u)
				{
					sys::spawn_pair(_coord, _pair);
					if (sys::check_lose(_coord, _grid, _pair))
						_reset_game();
				}
				else
				{
					if (!sys::try_move_pair(_coord, _pair, _grid, dt))
					{
						sys::add_falling_pair(_coord, _falling, _pair);
						sys::destroy_pair(_coord, _pair);
						_state = _game_state::falling;
					}
				}

				break;
			}

			case _game_state::falling:
			{
				if (!sys::fall_pieces(_coord, _grid, _falling, _chains, dt))
				{
					sys::clear_falling(_coord, _falling);
					_state = _game_state::score;
				}

				break;
			}

			case _game_state::score:
			{
				sys::find_combos(_coord, _grid, _chains);
				if (sys::filter_chains(_coord, _chains))
				{
					sys::clear_chains(_coord, _grid, _falling, _chains, _score);
					_state = _game_state::falling;

					{
						auto &s = _coord.get_component<score>(_score);
						log::logline(log::info, "%d", s.current);
					}

				}
				else
					_state = _game_state::pair;

				break;
			}
			}
		}
	}

	void game::render(graphics &gfx)
	{
		sys::render_grid(_coord, gfx, _grid);
	}

	void game::on_start()
	{
		_coord.register_component<belonging_chain>();
		_coord.register_component<color>();
		_coord.register_component<falling>();
		_coord.register_component<grid>();
		_coord.register_component<chains>();
		_coord.register_component<pair>();
		_coord.register_component<score>();
		_coord.register_component<state>();

		_coord.register_component<drawable>();

		_coord.register_component<transform>();
		_coord.register_component<velocity>();

		_init_game();

		_score = _coord.create_entity();
		_coord.add_component<score>(_score, { /* empty */ });
	}

	void game::on_exit()
	{

	}

	void game::_init_game()
	{
		_grid = _coord.create_entity();
		_coord.add_component<grid>(_grid, { { 0 } });

		_falling = _coord.create_entity();
		_coord.add_component<falling>(_falling, { /* empty*/ });

		_chains = _coord.create_entity();
		_coord.add_component<chains>(_chains, { /* empty */ });
	}

	void game::_reset_game()
	{
		sys::reset_entites(_coord, _grid, _falling, _chains, _pair, _score);
		_state = _game_state::pair;
		_init_game();
	}
}
