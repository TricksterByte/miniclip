#pragma once

#include <SDL.h>

#include <string>

namespace puyo
{
	namespace sdl
	{
		class scan_code final
		{
		public:
			constexpr scan_code() noexcept = default;

			constexpr scan_code(const scan_code &) noexcept = default;
			constexpr scan_code &operator=(const scan_code &) noexcept = default;

			constexpr scan_code(scan_code &&) noexcept = default;
			constexpr scan_code &operator=(scan_code &&) noexcept = default;

			constexpr scan_code(const SDL_Scancode scancode) noexcept : _code{ scancode }
			{
				// empty
			}

			constexpr scan_code &operator=(const SDL_Scancode code) noexcept
			{
				_code = code;
				return *this;
			}

			explicit scan_code(const SDL_Keycode key) noexcept : _code{ SDL_GetScancodeFromKey(key) }
			{
				// empty
			}

			scan_code &operator=(const SDL_Keycode key) noexcept
			{
				_code = SDL_GetScancodeFromKey(key);
				return *this;
			}

			explicit scan_code(const char *name) noexcept : _code{ SDL_GetScancodeFromName(name) }
			{
				// emtpy
			}

			scan_code &operator=(const char *name) noexcept
			{
				_code = SDL_GetScancodeFromName(name);
				return *this;
			}

			explicit scan_code(const std::string &name) : scan_code{ name.c_str() }
			{
				// empty
			}

			scan_code &operator=(const std::string &name)
			{
				return operator=(name.c_str());
			}

			[[nodiscard]] constexpr static int count() noexcept
			{
				return SDL_NUM_SCANCODES;
			}

			[[nodiscard]] constexpr bool unknown() const noexcept
			{
				return _code == SDL_SCANCODE_UNKNOWN;
			}

			[[nodiscard]] std::string name() const
			{
				return SDL_GetScancodeName(_code);
			}

			[[nodisacrd]] SDL_KeyCode to_key_code() const noexcept
			{
				return static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(_code));
			}

			[[nodiscard]] constexpr SDL_Scancode get() const noexcept
			{
				return _code;
			}

			constexpr explicit operator SDL_Scancode() const noexcept
			{
				return _code;
			}

			explicit operator SDL_KeyCode() const noexcept
			{
				return to_key_code();
			}

		private:
			SDL_Scancode _code{ SDL_SCANCODE_UNKNOWN };
		};

		namespace scancodes
		{
			inline constexpr scan_code unknown;

			inline constexpr scan_code a{ SDL_SCANCODE_A };
			inline constexpr scan_code b{ SDL_SCANCODE_B };
			inline constexpr scan_code c{ SDL_SCANCODE_C };
			inline constexpr scan_code d{ SDL_SCANCODE_D };
			inline constexpr scan_code e{ SDL_SCANCODE_E };
			inline constexpr scan_code f{ SDL_SCANCODE_F };
			inline constexpr scan_code g{ SDL_SCANCODE_G };
			inline constexpr scan_code h{ SDL_SCANCODE_H };
			inline constexpr scan_code i{ SDL_SCANCODE_I };
			inline constexpr scan_code j{ SDL_SCANCODE_J };
			inline constexpr scan_code k{ SDL_SCANCODE_K };
			inline constexpr scan_code l{ SDL_SCANCODE_L };
			inline constexpr scan_code m{ SDL_SCANCODE_M };
			inline constexpr scan_code n{ SDL_SCANCODE_N };
			inline constexpr scan_code o{ SDL_SCANCODE_O };
			inline constexpr scan_code p{ SDL_SCANCODE_P };
			inline constexpr scan_code q{ SDL_SCANCODE_Q };
			inline constexpr scan_code r{ SDL_SCANCODE_R };
			inline constexpr scan_code s{ SDL_SCANCODE_S };
			inline constexpr scan_code t{ SDL_SCANCODE_T };
			inline constexpr scan_code u{ SDL_SCANCODE_U };
			inline constexpr scan_code v{ SDL_SCANCODE_V };
			inline constexpr scan_code w{ SDL_SCANCODE_W };
			inline constexpr scan_code x{ SDL_SCANCODE_X };
			inline constexpr scan_code y{ SDL_SCANCODE_Y };
			inline constexpr scan_code z{ SDL_SCANCODE_Z };

			inline constexpr scan_code one{ SDL_SCANCODE_1 };
			inline constexpr scan_code two{ SDL_SCANCODE_2 };
			inline constexpr scan_code three{ SDL_SCANCODE_3 };
			inline constexpr scan_code four{ SDL_SCANCODE_4 };
			inline constexpr scan_code five{ SDL_SCANCODE_5 };
			inline constexpr scan_code six{ SDL_SCANCODE_6 };
			inline constexpr scan_code seven{ SDL_SCANCODE_7 };
			inline constexpr scan_code eight{ SDL_SCANCODE_8 };
			inline constexpr scan_code nine{ SDL_SCANCODE_9 };
			inline constexpr scan_code zero{ SDL_SCANCODE_0 };

			inline constexpr scan_code f1{ SDL_SCANCODE_F1 };
			inline constexpr scan_code f2{ SDL_SCANCODE_F2 };
			inline constexpr scan_code f3{ SDL_SCANCODE_F3 };
			inline constexpr scan_code f4{ SDL_SCANCODE_F4 };
			inline constexpr scan_code f5{ SDL_SCANCODE_F5 };
			inline constexpr scan_code f6{ SDL_SCANCODE_F6 };
			inline constexpr scan_code f7{ SDL_SCANCODE_F7 };
			inline constexpr scan_code f8{ SDL_SCANCODE_F8 };
			inline constexpr scan_code f9{ SDL_SCANCODE_F9 };
			inline constexpr scan_code f10{ SDL_SCANCODE_F10 };
			inline constexpr scan_code f11{ SDL_SCANCODE_F11 };
			inline constexpr scan_code f12{ SDL_SCANCODE_F12 };

			inline constexpr scan_code left{ SDL_SCANCODE_LEFT };
			inline constexpr scan_code right{ SDL_SCANCODE_RIGHT };
			inline constexpr scan_code up{ SDL_SCANCODE_UP };
			inline constexpr scan_code down{ SDL_SCANCODE_DOWN };

			inline constexpr scan_code space{ SDL_SCANCODE_SPACE };
			inline constexpr scan_code enter{ SDL_SCANCODE_RETURN };
			inline constexpr scan_code escape{ SDL_SCANCODE_ESCAPE };
			inline constexpr scan_code backspace{ SDL_SCANCODE_BACKSPACE };
			inline constexpr scan_code tab{ SDL_SCANCODE_TAB };

			inline constexpr scan_code caps_lock{ SDL_SCANCODE_CAPSLOCK };
			inline constexpr scan_code left_shift{ SDL_SCANCODE_LSHIFT };
			inline constexpr scan_code right_shift{ SDL_SCANCODE_RSHIFT };
			inline constexpr scan_code left_ctrl{ SDL_SCANCODE_LCTRL };
			inline constexpr scan_code right_ctrl{ SDL_SCANCODE_RCTRL };
			inline constexpr scan_code left_alt{ SDL_SCANCODE_LALT };
			inline constexpr scan_code right_alt{ SDL_SCANCODE_RALT };

			inline constexpr scan_code left_gui{ SDL_SCANCODE_LGUI };
			inline constexpr scan_code right_gui{ SDL_SCANCODE_RGUI };
		}
	}
}
