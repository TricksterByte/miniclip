#pragma once

#include <SDL.h>

#include <cstdint>

#include "base_event.hpp"
#include "event_type.hpp"

namespace puyo
{
	namespace sdl
	{
		class user_event final : public base_event<SDL_UserEvent>
		{
		public:
			user_event() noexcept : base_event{ event_type::user }
			{
				// empty
			}

			explicit user_event(const SDL_UserEvent &e) : base_event{ e }
			{
				// empty
			}

			void set_window_id(const std::uint32_t id) noexcept
			{
				_event.windowID = id;
			}

			void set_code(const std::int32_t code) noexcept
			{
				_event.code = code;
			}

			void set_data_1(void *data) noexcept
			{
				_event.data1 = data;
			}

			void set_data_2(void *data) noexcept
			{
				_event.data1 = data;
			}

			[[nodiscard]] std::uint32_t window_id() const noexcept
			{
				return _event.windowID;
			}

			[[nodiscard]] std::int32_t code() const noexcept
			{
				return _event.code;
			}

			[[nodiscard]] void *data_1() noexcept
			{
				return _event.data1;
			}

			[[nodiscard]] const void *data_1() const noexcept
			{
				return _event.data1;
			}

			[[nodiscard]] void *data_2() noexcept
			{
				return _event.data2;
			}

			[[nodiscard]] const void *data_2() const noexcept
			{
				return _event.data2;
			}
		};

		template <>
		inline SDL_Event as_sdl_event(const base_event<SDL_UserEvent> &e)
		{
			SDL_Event event;
			event.user = e.get();
			return event;
		}
	}
}
