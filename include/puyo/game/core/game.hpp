#pragma once

#include "ecs/coordinator.hpp"
#include "ecs/entity.hpp"

#include "input.hpp"
#include "graphics.hpp"

namespace puyo
{
	class game final
	{
	public:
		void handle_input(const input &input);

		void tick(float dt);

		void render(graphics &gfx);

		void on_start();

		void on_exit();

	private:
		bool _paused{ false };

		coordinator _coord{};

		enum class _game_state
		{
			pair,
			falling,
			score
		};
		_game_state _state{ _game_state::pair };

		entity _grid;
		entity _falling;
		entity _chains;
		entity _pair;
		entity _score;

		void _init_game();
		void _reset_game();
	};
}
