#pragma once

#include <SDL.h>

#include <cstdint>
#include <type_traits>
#include <utility>

#include "event_type.hpp"

namespace puyo
{
	namespace sdl
	{
		template <typename T>
		class base_event
		{
		public:
			explicit base_event(const event_type type) noexcept
			{
				set_time(SDL_GetTicks());
				set_type(type);
			}

			explicit base_event(const T &e) : _event{ e }
			{
				// empty;
			}

			explicit base_event(T &&e) : _event{ std::move(e) }
			{
				// empty
			}

			void set_time(const std::uint32_t ts) noexcept
			{
				_event.timestamp = ts;
			}

			void set_type(const event_type type) noexcept
			{
				_event.type = static_cast<std::underlying_type_t<event_type>>(type);
			}

			[[nodiscard]] std::uint32_t time() const noexcept
			{
				return _event.timestamp;
			}

			[[nodiscard]] event_type type() const noexcept
			{
				return static_cast<event_type>(_event.type);
			}

			[[nodiscard]] const T &get() const noexcept
			{
				return _event;
			}

		protected:
			T _event{};
		};

		template <typename T>
		[[nodiscard]] SDL_Event as_sdl_event(const base_event<T> &e);
	}
}
