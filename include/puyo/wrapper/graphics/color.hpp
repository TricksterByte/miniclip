#pragma once

#include <SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace puyo
{
	namespace sdl
	{
		class color final
		{
		public:
			constexpr color() noexcept = default;

			constexpr color(const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue, const std::uint8_t alpha = std::numeric_limits<std::uint8_t>::max()) noexcept
				: _color{ red, green, blue, alpha }
			{
				// empty
			}

			constexpr explicit color(const SDL_Color &color) noexcept : _color{ color }
			{
				// empty
			}

			[[nodiscard]] const SDL_Color &get() const noexcept
			{
				return _color;
			}

			constexpr std::uint8_t red() const noexcept
			{
				return _color.r;
			}

			constexpr std::uint8_t green() const noexcept
			{
				return _color.g;
			}

			constexpr std::uint8_t blue() const noexcept
			{
				return _color.b;
			}

			constexpr std::uint8_t alpha() const noexcept
			{
				return _color.a;
			}

			[[nodiscard]] static color from_norm(float red, float blue, float green, float alpha = 1.f) noexcept
			{
				red = std::clamp(red, 0.f, 1.f);
				green = std::clamp(green, 0.f, 1.f);
				blue = std::clamp(blue, 0.f, 1.f);
				alpha = std::clamp(alpha, 0.f, 1.f);

				const auto r = static_cast<std::uint8_t>(std::round(red * 255.f));
				const auto g = static_cast<std::uint8_t>(std::round(green * 255.f));
				const auto b = static_cast<std::uint8_t>(std::round(blue * 255.f));
				const auto a = static_cast<std::uint8_t>(std::round(alpha * 255.f));

				return color{ r, g, b, a };
			}

			constexpr void set_red(const std::uint8_t red) noexcept
			{
				_color.r = red;
			}

			constexpr void set_green(const std::uint8_t green) noexcept
			{
				_color.g = green;
			}

			constexpr void set_blue(const std::uint8_t blue) noexcept
			{
				_color.b = blue;
			}

			constexpr void set_alpha(const std::uint8_t alpha) noexcept
			{
				_color.a = alpha;
			}

			[[nodiscard]] SDL_Color *data() noexcept
			{
				return &_color;
			}

			[[nodiscard]] const SDL_Color *data() const noexcept
			{
				return &_color;
			}

			[[nodiscard]] explicit constexpr operator SDL_Color() const noexcept
			{
				return { red(), green(), blue(), alpha() };
			}

			[[nodiscard]] explicit operator SDL_Color *() noexcept
			{
				return &_color;
			}


			[[nodiscard]] explicit operator const SDL_Color *() const noexcept
			{
				return &_color;
			}

			[[nodiscard]] constexpr color with_alpha(const std::uint8_t alpha) const noexcept
			{
				return { red(), green(), blue(), alpha };
			}

			[[nodiscard]] constexpr static std::uint8_t max() noexcept
			{
				return 0xFF;
			}

		private:
			SDL_Color _color{ 0, 0, 0, 0xFF };
		};
	}
}
