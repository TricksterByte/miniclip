#pragma once

#include <SDL.h>

#include <cmath>
#include <type_traits>

namespace puyo
{
	namespace sdl
	{
		template <typename T, std::enable_if_t<std::is_convertible_v<T, int> || std::is_convertible_v<T, float>, bool> = true>
		class point_traits final
		{
		public:
			inline constexpr static bool is_integral = std::is_integral_v<T>;
			inline constexpr static bool is_floating = std::is_floating_point_v<T>;
			
			using value_type = std::conditional_t<is_integral, int, float>;
			using point_type = std::conditional_t<is_integral, SDL_Point, SDL_FPoint>;
		};

		template <typename T>
		class basic_point final
		{
		public:
			inline constexpr static bool is_integral = point_traits<T>::is_integral;
			inline constexpr static bool is_floating = point_traits<T>::is_floating;
			
			using value_type = typename point_traits<T>::value_type;
			using point_type = typename point_traits<T>::point_type;

			constexpr basic_point() noexcept = default;

			constexpr basic_point(const value_type x, const value_type y) noexcept
			{
				_point.x = x;
				_point.y = y;
			}

			constexpr void set_x(const value_type x) noexcept
			{
				_point.x = x;
			}

			constexpr void set_y(const value_type y) noexcept
			{
				_point.y = y;
			}

			[[nodiscard]] constexpr value_type x() const noexcept
			{
				return _point.x;
			}

			[[nodiscard]] constexpr value_type y() const noexcept
			{
				return _point.y;
			}

			[[nodiscard]] constexpr point_type &get() noexcept
			{
				return _point;
			}

			[[nodiscard]] constexpr const point_type &get() const noexcept
			{
				return _point;
			}

			[[nodiscard]] constexpr explicit operator point_type() const noexcept
			{
				return _point;
			}

		private:
			point_type _point{ 0, 0 };
		};

		using ipoint = basic_point<int>;
		using fpoint = basic_point<float>;
	}
}
