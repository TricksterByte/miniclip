#pragma once

#include <filesystem>

#include "../../wrapper/graphics/window.hpp"
#include "../../wrapper/events/event.hpp"

#include "game.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "semi_fixed_game_loop.hpp"

namespace puyo
{
	template <typename Game = game, typename Graphics = graphics>
	class engine
	{
		friend class semi_fixed_game_loop<Game, Graphics>;

	public:
		using game_type = Game;
		using graphics_type = Graphics;
		using loop_type = semi_fixed_game_loop<game_type, graphics_type>;

		engine() : _loop{ this }, _window{ "PuyoPuyo" }, _graphics{ _window }
		{
			std::filesystem::path path = std::filesystem::current_path();

			path /= "resources/textures";

			if (std::filesystem::exists(path))
			{
				_graphics.load(0, (path / "tile_palette.png").string());
			}
		}

		int run()
		{
			auto &renderer = _graphics.renderer();

			_window.show();
			_loop.fetch_current_time();

			_game.on_start();

			while (_loop.is_running())
			{
				_loop.tick();

				renderer.clear();
				_game.render(_graphics);
				renderer.present();
			}

			_game.on_exit();

			_window.hide();

			return 0;
		}

	private:
		sdl::window _window;
		graphics_type _graphics;
		game_type _game{};
		loop_type _loop;
		input _input;

		bool update_input()
		{
			_input.keyboard.update();

			sdl::event::update();

			_game.handle_input(_input);

			return !sdl::event::in_queue(sdl::event_type::quit);
		}

		void update_logic(float dt)
		{
			_game.tick(dt);
		}
	};
}
