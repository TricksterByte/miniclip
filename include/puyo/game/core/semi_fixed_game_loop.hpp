#pragma once

#include <algorithm>
#include <chrono>

#include "../../wrapper/graphics/screen.hpp"

namespace puyo
{
	template <typename T>
	[[nodiscard]] T tick_rate()
	{
		return 60.0;
		//return std::min<T>(60.0, static_cast<T>(sdl::screen::refresh_rate().value()));
	}

	template <typename Game, typename Graphics>
	class engine;

	template <typename Game, typename Graphics>
	class semi_fixed_game_loop
	{
	public:
		using game_type = Game;
		using graphics_type = Graphics;
		using engine_type = engine<Game, Graphics>;
		using precision_type = float;
		using seconds_type = std::chrono::duration<precision_type>;

		semi_fixed_game_loop(engine_type *engine)
			: _engine{ engine }
			, _current_time{ seconds_type{ static_cast<precision_type>(SDL_GetPerformanceCounter()) / static_cast<precision_type>(SDL_GetPerformanceFrequency()) } }
			, _tick_rate{ tick_rate<precision_type>() }
			, _fixed_delta{ 1.0 / _tick_rate }
		{
			// empty
		}

		void fetch_current_time() noexcept
		{
			_current_time = seconds_type{ static_cast<precision_type>(SDL_GetPerformanceCounter()) / static_cast<precision_type>(SDL_GetPerformanceFrequency()) };
		}

		void tick()
		{
			const auto new_time = seconds_type{ static_cast<precision_type>(SDL_GetPerformanceCounter()) / static_cast<precision_type>(SDL_GetPerformanceFrequency()) };
			auto frame_time = new_time - _current_time;
			_current_time = new_time;

			int steps = 0;

			while (frame_time > seconds_type::zero())
			{
				if (steps > _max_steps)
					break;

				_running = _engine->update_input();

				if (!_running)
					break;

				const seconds_type delta_time = std::min(frame_time, _fixed_delta);
				const precision_type delta = static_cast<precision_type>(delta_time.count());
				_engine->update_logic(delta);

				frame_time -= delta_time;

				++steps;
			}
		}

		[[nodiscard]] bool is_running() const noexcept
		{
			return _running;
		}

	private:
		constexpr inline static int _max_steps = 5;

		engine_type *_engine;

		seconds_type _current_time;
		precision_type _tick_rate;
		seconds_type _fixed_delta;
		bool _running{ true };
	};
}
