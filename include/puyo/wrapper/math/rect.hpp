#pragma once

#include <SDL.h>

#include <type_traits>

#include "area.hpp"
#include "point.hpp"

namespace puyo
{
	namespace sdl
	{
		template <typename T, std::enable_if_t<std::is_convertible_v<T, int> || std::is_convertible_v<T, float>, bool> = true>
		class rect_traits final
		{
		public:
			inline constexpr static bool is_integral = std::is_integral_v<T>;
			inline constexpr static bool is_floating = std::is_floating_point_v<T>;

			using value_type = std::conditional_t<is_integral, int, float>;
			using point_type = std::conditional_t<is_integral, ipoint, fpoint>;
			using area_type = std::conditional_t<is_integral, iarea, farea>;
			using rect_type = std::conditional_t<is_integral, SDL_Rect, SDL_FRect>;
		};

		template <typename T>
		class basic_rect final
		{
		public:
			inline constexpr static bool is_integral = rect_traits<T>::is_integral;
			inline constexpr static bool is_floating = rect_traits<T>::is_floating;

			using value_type = typename rect_traits<T>::value_type;
			using point_type = typename rect_traits<T>::point_type;
			using area_type = typename rect_traits<T>::area_type;
			using rect_type = typename rect_traits<T>::rect_type;

			constexpr basic_rect() noexcept = default;

			constexpr explicit basic_rect(const rect_type &rect) noexcept : _rect{ rect }
			{
				// empty
			}

			constexpr basic_rect(const point_type &position, const area_type &size) noexcept : _rect{ position.x(), position.y(), size.width, size.height }
			{
				// empty
			}

			constexpr basic_rect(const value_type x, const value_type y, const value_type width, const value_type height) noexcept : _rect{ x, y, width, height }
			{
				// empty
			}

			[[nodiscard]] constexpr area_type size() const noexcept
			{
				return area_type{ _rect.w, _rect.h };
			}

			constexpr void set_size(const area_type &size) noexcept
			{
				_rect.w = size.with;
				_rect.h = size.height;
			}

			constexpr void set_x(const value_type x) noexcept
			{
				_rect.x = x;
			}

			constexpr void set_y(const value_type y) noexcept
			{
				_rect.y = y;
			}

			constexpr void set_max_x(const value_type max_x) noexcept
			{
				_rect.x = max_x - _rect.w;
			}

			constexpr void set_max_y(const value_type max_y) noexcept
			{
				_rect.y = max_y - _rect.h;
			}

			constexpr void offset_x(const value_type off) noexcept
			{
				_rect.x += off;
			}

			constexpr void offset_y(const value_type off) noexcept
			{
				_rect.y += off;
			}

			[[nodiscard]] constexpr value_type x() const noexcept
			{
				return _rect.x;
			}

			[[nodiscard]] constexpr value_type y() const noexcept
			{
				return _rect.y;
			}

			[[nodiscard]] constexpr value_type width() const noexcept
			{
				return _rect.w;
			}

			[[nodiscard]] constexpr value_type height() const noexcept
			{
				return _rect.h;
			}

			constexpr void set_width(const value_type width) noexcept
			{
				_rect.w = width;
			}

			constexpr void set_height(const value_type height) noexcept
			{
				_rect.h = height;
			}

			constexpr void offset_width(const value_type offset) noexcept
			{
				_rect.w += offset;
			}

			constexpr void offset_height(const value_type offset) noexcept
			{
				_rect.h += offset;
			}

			[[nodiscard]] constexpr point_type position() const noexcept
			{
				return point_type{ _rect.x, _rect.y };
			}

			[[nodiscard]] constexpr value_type center_x() const noexcept
			{
				return x() + (width() / static_cast<value_type>(2));
			}

			[[nodiscard]] constexpr value_type center_y() const noexcept
			{
				return y() + (height() / static_cast<value_type>(2));
			}

			[[nodiscard]] constexpr point_type center() const noexcept
			{
				return { center_x(), center_y };
			}

			[[nodiscard]] constexpr value_type area() const noexcept
			{
				return width() * height();
			}

			[[nodiscard]] constexpr bool contains(const point_type &point) const noexcept
			{
				const auto px = point.x();
				const auto py = point.y();
				return !(px < x() || py < y() || px > max_x() || py > max_y());
			}

			[[nodiscard]] constexpr bool has_area() const noexcept
			{
				return (width() > 0) && (height() > 0);
			}

			[[nodiscard]] rect_type *data() noexcept
			{
				return &_rect;
			}

			[[nodiscard]] const rect_type *data() const noexcept
			{
				return &_rect;
			}

		private:
			rect_type _rect;
		};

		using irect = basic_rect<int>;
		using frect = basic_rect<float>;
	}
}
