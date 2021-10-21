#pragma once

#include <SDL.h>

#include <cstdint>

#include "base_event.hpp"
#include "event_type.hpp"

namespace puyo
{
	namespace sdl
	{
		enum class window_event_id
		{
			none = SDL_WINDOWEVENT_NONE,
			shown = SDL_WINDOWEVENT_SHOWN,
			hidden = SDL_WINDOWEVENT_HIDDEN,
			exposed = SDL_WINDOWEVENT_EXPOSED,
			moved = SDL_WINDOWEVENT_MOVED,
			resized = SDL_WINDOWEVENT_RESIZED,
			size_changed = SDL_WINDOWEVENT_SIZE_CHANGED,
			minimized = SDL_WINDOWEVENT_MINIMIZED,
			maximized = SDL_WINDOWEVENT_MAXIMIZED,
			restored = SDL_WINDOWEVENT_RESTORED,
			enter = SDL_WINDOWEVENT_ENTER,
			leave = SDL_WINDOWEVENT_LEAVE,
			focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
			focus_lost = SDL_WINDOWEVENT_FOCUS_LOST,
			close = SDL_WINDOWEVENT_CLOSE,
			take_focus = SDL_WINDOWEVENT_TAKE_FOCUS,
			hit_test = SDL_WINDOWEVENT_HIT_TEST
		};

		class window_event final : public base_event<SDL_WindowEvent>
		{
		public:
			window_event() noexcept : base_event{ event_type::window }
			{
				// empty
			}

			explicit window_event(const SDL_WindowEvent &e) noexcept : base_event{ e }
			{
				// empty
			}

			[[nodiscard]] window_event_id event_id() const noexcept
			{
				return static_cast<window_event_id>(_event.event);
			}

			[[nodiscard]] std::int32_t data_1() const noexcept
			{
				return _event.data1;
			}

			[[nodiscard]] std::int32_t data_2() const noexcept
			{
				return _event.data2;
			}
		};

		template <>
		inline SDL_Event as_sdl_event(const base_event<SDL_WindowEvent> &e)
		{
			SDL_Event event;
			event.window = e.get();
			return event;
		}
	}
}
