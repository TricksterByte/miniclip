#pragma once

#include <SDL.h>

#include <cstdint>

namespace puyo
{
	namespace sdl
	{
		enum class button_state : std::uint8_t
		{
			released = SDL_RELEASED,
			pressed = SDL_PRESSED
		};
	}
}
