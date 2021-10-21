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
		};
	}
}
