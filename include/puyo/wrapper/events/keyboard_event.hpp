#pragma once

#include <SDL.h>

#include <cstdint>
#include <type_traits>

#include "../input/button_state.hpp"
#include "../input/key_code.hpp"
#include "../input/scan_code.hpp"
#include "base_event.hpp"
#include "event_type.hpp"

namespace puyo
{
	namespace sdl
	{
		class keyboard_event final : public base_event<SDL_KeyboardEvent>
		{
		public:
			keyboard_event() noexcept : base_event{ event_type::key_down }
			{
				// empty
			}

			explicit keyboard_event(const SDL_KeyboardEvent &e) noexcept : base_event{ e }
			{
				// empty
			}

			void set_scan_code(const scan_code &sc) noexcept
			{
				_event.keysym.scancode = sc.get();
			}

			void set_key_code(const key_code &key) noexcept
			{
				_event.keysym.sym = key.get();
			}

			void set_button_state(const button_state state) noexcept
			{
				_event.state = static_cast<std::underlying_type_t<button_state>>(state);
			}

			void set_repeated(const bool repeated) noexcept
			{
				_event.repeat = repeated;
			}

			void set_window_id(const std::uint32_t id) noexcept
			{
				_event.windowID = id;
			}

			[[nodiscard]] bool is_active(const scan_code &sc) const noexcept
			{
				return _event.keysym.scancode == sc.get();
			}

			[[nodiscard]] bool is_active(const key_code &key) const noexcept
			{
				return _event.keysym.sym == key.get();
			}

			[[nodiscard]] bool repeated() const noexcept
			{
				return _event.repeat;
			}

			[[nodiscard]] button_state state() const noexcept
			{
				return static_cast<button_state>(_event.state);
			}

			[[nodiscard]] bool released() const noexcept
			{
				return state() == button_state::released;
			}

			[[nodiscard]] bool pressed() const noexcept
			{
				return state() == button_state::pressed;
			}

			[[nodiscard]] scan_code scan() const noexcept
			{
				return _event.keysym.scancode;
			}

			[[nodiscard]] key_code key() const noexcept
			{
				return static_cast<SDL_KeyCode>(_event.keysym.sym);
			}

			[[nodiscard]] std::uint32_t window_id() const noexcept
			{
				return _event.windowID;
			}
		};

		template <>
		inline SDL_Event as_sdl_event(const base_event<SDL_KeyboardEvent> &e)
		{
			SDL_Event event;
			event.key = e.get();
			return event;
		}
	}
}
