#pragma once

#include <SDL.h>

#include <cstdint>

namespace puyo
{
	namespace sdl
	{
		enum class event_type : std::uint32_t
		{
			quit = SDL_QUIT,

			window = SDL_WINDOWEVENT,
			system = SDL_SYSWMEVENT,

			key_down = SDL_KEYDOWN,
			key_up = SDL_KEYUP,

			user = SDL_USEREVENT
		};
	}
}
