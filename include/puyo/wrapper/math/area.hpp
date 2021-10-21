#pragma once

#include <type_traits>

namespace puyo
{
	namespace sdl
	{
		template <typename T>
		struct basic_area final
		{
			static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);

			using value_type = T;

			T width{ 0 };
			T height{ 0 };
		};

		using iarea = basic_area<int>;
		using farea = basic_area<float>;

		template <typename T>
		[[nodiscard]] constexpr basic_area<T> area(const T width, const T height) noexcept
		{
			return { width, height };
		}

		template <typename T>
		[[nodiscard]] constexpr T area_of(const basic_area<T> area) noexcept
		{
			return area.width * area.height;
		}
	}
}
