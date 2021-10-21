#pragma once

#include <SDL.h>

#include <string>

namespace puyo
{
	namespace sdl
	{
		class key_code final
		{
		public:
			constexpr key_code() noexcept = default;

			constexpr key_code(const key_code &) noexcept = default;
			constexpr key_code &operator=(const key_code &) noexcept = default;

			constexpr key_code(key_code &&) noexcept = default;
			constexpr key_code &operator=(key_code &&) noexcept = default;

			constexpr key_code(const SDL_KeyCode key) noexcept : _key{ key }
			{
				// empty
			}

			constexpr key_code &operator=(const SDL_KeyCode key) noexcept
			{
				_key = key;
				return *this;
			}

			explicit key_code(const SDL_Scancode scancode) noexcept : _key{ static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(scancode)) }
			{
				// empty
			}

			key_code &operator=(const SDL_Scancode scancode) noexcept
			{
				_key = static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(scancode));
				return *this;
			}

			explicit key_code(const char *name) noexcept : _key{ static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name)) }
			{
				// emtpy
			}

			key_code &operator=(const char *name) noexcept
			{
				_key = static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name));
				return *this;
			}

			explicit key_code(const std::string &name) : key_code{ name.c_str() }
			{
				// empty
			}

			key_code &operator=(const std::string &name)
			{
				return operator=(name.c_str());
			}

			[[nodiscard]] constexpr bool unknown() const noexcept
			{
				return _key == SDLK_UNKNOWN;
			}

			[[nodiscard]] std::string name() const
			{
				return SDL_GetKeyName(_key);
			}

			[[nodisacrd]] SDL_Scancode to_scan_code() const noexcept
			{
				return SDL_GetScancodeFromKey(_key);
			}

			[[nodiscard]] constexpr SDL_KeyCode get() const noexcept
			{
				return _key;
			}

			constexpr explicit operator SDL_KeyCode() const noexcept
			{
				return _key;
			}

			constexpr explicit operator SDL_Keycode() const noexcept
			{
				return _key;
			}

			explicit operator SDL_Scancode() const noexcept
			{
				return to_scan_code();
			}

		private:
			SDL_KeyCode _key{ SDLK_UNKNOWN };
		};

		namespace keycodes
		{
			inline constexpr key_code unknown;

			inline constexpr key_code a{ SDLK_a };
			inline constexpr key_code b{ SDLK_b };
			inline constexpr key_code c{ SDLK_c };
			inline constexpr key_code d{ SDLK_d };
			inline constexpr key_code e{ SDLK_e };
			inline constexpr key_code f{ SDLK_f };
			inline constexpr key_code g{ SDLK_g };
			inline constexpr key_code h{ SDLK_h };
			inline constexpr key_code i{ SDLK_i };
			inline constexpr key_code j{ SDLK_j };
			inline constexpr key_code k{ SDLK_k };
			inline constexpr key_code l{ SDLK_l };
			inline constexpr key_code m{ SDLK_m };
			inline constexpr key_code n{ SDLK_n };
			inline constexpr key_code o{ SDLK_o };
			inline constexpr key_code p{ SDLK_p };
			inline constexpr key_code q{ SDLK_q };
			inline constexpr key_code r{ SDLK_r };
			inline constexpr key_code s{ SDLK_s };
			inline constexpr key_code t{ SDLK_t };
			inline constexpr key_code u{ SDLK_u };
			inline constexpr key_code v{ SDLK_v };
			inline constexpr key_code w{ SDLK_w };
			inline constexpr key_code x{ SDLK_x };
			inline constexpr key_code y{ SDLK_y };
			inline constexpr key_code z{ SDLK_z };

			inline constexpr key_code one{ SDLK_1 };
			inline constexpr key_code two{ SDLK_2 };
			inline constexpr key_code three{ SDLK_3 };
			inline constexpr key_code four{ SDLK_4 };
			inline constexpr key_code five{ SDLK_5 };
			inline constexpr key_code six{ SDLK_6 };
			inline constexpr key_code seven{ SDLK_7 };
			inline constexpr key_code eight{ SDLK_8 };
			inline constexpr key_code nine{ SDLK_9 };
			inline constexpr key_code zero{ SDLK_0 };

			inline constexpr key_code f1{ SDLK_F1 };
			inline constexpr key_code f2{ SDLK_F2 };
			inline constexpr key_code f3{ SDLK_F3 };
			inline constexpr key_code f4{ SDLK_F4 };
			inline constexpr key_code f5{ SDLK_F5 };
			inline constexpr key_code f6{ SDLK_F6 };
			inline constexpr key_code f7{ SDLK_F7 };
			inline constexpr key_code f8{ SDLK_F8 };
			inline constexpr key_code f9{ SDLK_F9 };
			inline constexpr key_code f10{ SDLK_F10 };
			inline constexpr key_code f11{ SDLK_F11 };
			inline constexpr key_code f12{ SDLK_F12 };

			inline constexpr key_code left{ SDLK_LEFT };
			inline constexpr key_code right{ SDLK_RIGHT };
			inline constexpr key_code up{ SDLK_UP };
			inline constexpr key_code down{ SDLK_DOWN };

			inline constexpr key_code space{ SDLK_SPACE };
			inline constexpr key_code enter{ SDLK_RETURN };
			inline constexpr key_code escape{ SDLK_ESCAPE };
			inline constexpr key_code backspace{ SDLK_BACKSPACE };
			inline constexpr key_code tab{ SDLK_TAB };

			inline constexpr key_code caps_lock{ SDLK_CAPSLOCK };
			inline constexpr key_code left_shift{ SDLK_LSHIFT };
			inline constexpr key_code right_shift{ SDLK_RSHIFT };
			inline constexpr key_code left_ctrl{ SDLK_LCTRL };
			inline constexpr key_code right_ctrl{ SDLK_RCTRL };
			inline constexpr key_code left_alt{ SDLK_LALT };
			inline constexpr key_code right_alt{ SDLK_RALT };

			inline constexpr key_code left_gui{ SDLK_LGUI };
			inline constexpr key_code right_gui{ SDLK_RGUI };
		}
	}
}
