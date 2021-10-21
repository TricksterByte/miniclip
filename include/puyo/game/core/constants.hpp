#pragma once

#include "../../wrapper/math/area.hpp"

namespace puyo
{
	template <typename T = int>
	inline constexpr T logical_width{ 640 };

	template <typename T = int>
	inline constexpr T logical_height{ 1280 };

	template <typename T = sdl::iarea>
	inline constexpr T logical_size{ logical_width<>, logical_height<> };

	inline constexpr std::size_t grid_width{ 8 };
	inline constexpr std::size_t grid_height{ 16 };
	inline constexpr std::size_t grid_size{ grid_width * grid_height };

	template <typename T = int>
	inline constexpr T x_interval{ logical_width<T> / grid_width };

	template <typename T = int>
	inline constexpr T y_interval{ logical_height<T> / grid_height };

	inline constexpr float speed = 240.f;
}
