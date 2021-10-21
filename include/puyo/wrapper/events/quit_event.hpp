#pragma once

#include <SDL.h>

#include "base_event.hpp"
#include "event_type.hpp"

namespace puyo
{
	namespace sdl
	{
		class quit_event final : public base_event<SDL_QuitEvent>
		{
		public:
			quit_event() noexcept : base_event{ event_type::quit }
			{
				// empty
			}

			explicit quit_event(const SDL_QuitEvent &e) noexcept : base_event{ e }
			{
				// empty
			}
		};

		template <>
		inline SDL_Event as_sdl_event(const base_event<SDL_QuitEvent> &e)
		{
			SDL_Event event;
			event.quit = e.get();
			return event;
		}
	}
}
