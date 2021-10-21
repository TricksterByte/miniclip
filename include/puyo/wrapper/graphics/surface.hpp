#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <memory>
#include <string>
#include <stdexcept>
#include <type_traits>

#include "../math/area.hpp"
#include "enums.hpp"

namespace puyo
{
	namespace sdl
	{
		class surface final
		{
		public:
			explicit surface(SDL_Surface *source) : _surface{ source }
			{
				if (!_surface)
					throw std::runtime_error("Cannot create surface from null pointer.");
			}

			explicit surface(const char *path) : _surface{ IMG_Load(path) }
			{
				if (!_surface)
					throw std::runtime_error("Error creating surface.");
			}

			explicit surface(const std::string &path) : surface{ path.c_str() }
			{
				// empty
			}

			surface(const iarea size, const pixel_format format) : _surface{ SDL_CreateRGBSurfaceWithFormat(0, size.width, size.height, 0, static_cast<std::underlying_type_t<pixel_format>>(format)) }
			{
				if (!_surface)
					throw std::runtime_error("Error creating surface.");
			}

			SDL_Surface *get() const noexcept
			{
				return _surface.get();
			}

		private:
			struct deleter final
			{
				void operator()(SDL_Surface *surface) noexcept
				{
					SDL_FreeSurface(surface);
				}
			};

			std::unique_ptr<SDL_Surface, deleter> _surface;
		};
	}
}
