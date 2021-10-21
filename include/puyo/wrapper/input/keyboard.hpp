#pragma once

#include <SDL.h>

#include <algorithm>
#include <array>
#include <cstdint>

#include "key_code.hpp"
#include "scan_code.hpp"

namespace puyo
{
	namespace
	{
		[[nodiscard]] SDL_Scancode _key_to_scan(const SDL_Keycode &key) noexcept
		{
			return SDL_GetScancodeFromKey(key);
		}

		[[nodiscard]] SDL_Keycode _scan_to_key(const SDL_Scancode &scan) noexcept
		{
			return SDL_GetKeyFromScancode(scan);
		}
	}

	namespace sdl
	{
		class keyboard final
		{
		public:
			keyboard() noexcept
			{
				_states = SDL_GetKeyboardState(&_nkeys);
			}

			void update()
			{
				std::copy(_states, _states + _nkeys, _previous.begin());
			}

			[[nodiscard]] bool is_pressed(const scan_code &code) const noexcept
			{
				return _check_state(code, [this](const SDL_Scancode sc) noexcept { return _states[sc]; });
			}

			[[nodiscard]] bool is_pressed(const key_code &code) const noexcept
			{
				return is_pressed(code.to_scan_code());
			}

			[[nodiscard]] bool is_held(const scan_code &code) const noexcept
			{
				return _check_state(code, [this](const SDL_Scancode sc) { return _states[sc] && _previous[sc]; });
			}

			[[nodiscard]] bool is_held(const key_code &code) const noexcept
			{
				return is_held(code.to_scan_code());
			}

			[[nodiscard]] bool just_pressed(const scan_code &code) const noexcept
			{
				return _check_state(code, [this](const SDL_Scancode sc) { return _states[sc] && !_previous[sc]; });
			}

			[[nodiscard]] bool just_pressed(const key_code &code) const noexcept
			{
				return just_pressed(code.to_scan_code());
			}

			[[nodiscard]] bool just_released(const scan_code &code) const noexcept
			{
				return _check_state(code, [this](const SDL_Scancode sc) {return !_states[sc] && _previous[sc]; });
			}

			[[nodiscard]] bool just_released(const key_code &code) const noexcept
			{
				return just_released(code.to_scan_code());
			}

			[[nodiscard]] int key_count() const noexcept
			{
				return _nkeys;
			}
		
		private:
			template <typename Callback>
			bool _check_state(const scan_code &sc, Callback &&cb) const
			{
				const auto code = sc.get();
				if (code >= 0 && code < _nkeys)
					return cb(code);
				else
					return false;
			}

			const std::uint8_t *_states{};
			std::array<std::uint8_t, SDL_NUM_SCANCODES> _previous{};
			int _nkeys;
		};
	}
}
