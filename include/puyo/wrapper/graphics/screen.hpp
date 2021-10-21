#pragma once

#include <SDL.h>

#include <optional>

#include "enums.hpp"

namespace puyo
{
	namespace sdl
	{
		struct dpi_info final
		{
			float
				diagonal = 0.f,
				horizontal = 0.f,
				vertical = 0.f;
		};

		enum class screen_orientation : int
		{
			unknown = SDL_ORIENTATION_UNKNOWN,
			landscape = SDL_ORIENTATION_LANDSCAPE,
			landscape_flipped = SDL_ORIENTATION_LANDSCAPE_FLIPPED,
			portrait = SDL_ORIENTATION_PORTRAIT,
			portrait_flipped = SDL_ORIENTATION_PORTRAIT_FLIPPED
		};

		[[nodiscard]] inline int get_screen_count() noexcept
		{
			return SDL_GetNumVideoDisplays();
		}

		namespace screen
		{
			[[nodiscard]] inline const char *name(const int index = 0) noexcept
			{
				return SDL_GetDisplayName(index);
			}

			[[nodiscard]] inline screen_orientation orientation(const int index = 0) noexcept
			{
				return static_cast<screen_orientation>(SDL_GetDisplayOrientation(index));
			}

			[[nodiscard]] inline std::optional<SDL_DisplayMode> display_mode(const int index = 0) noexcept
			{
				SDL_DisplayMode mode{};
				if (SDL_GetDesktopDisplayMode(index, &mode) == 0)
					return mode;
				else
					return std::nullopt;
			}

			[[nodiscard]] inline std::optional<int> refresh_rate(const int index = 0) noexcept
			{
				if (const auto mode = display_mode(index))
					return mode->refresh_rate;
				else
					return std::nullopt;
			}
		}
	}
}
