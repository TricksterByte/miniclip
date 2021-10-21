#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>
#include <string>
#include <memory>
#include <type_traits>

#include "../math/area.hpp"
#include "../math/point.hpp"
#include "enums.hpp"
#include "surface.hpp"

namespace puyo
{
	namespace sdl
	{
		class texture final
		{
		public:
			explicit texture(SDL_Texture *source) : _texture{ source }
			{
				if (!_texture)
					throw std::runtime_error("Cannot create texture from null pointer.");
			}

			template <typename Renderer>
			explicit texture(const Renderer &rend, const char *path) : _texture{ IMG_LoadTexture(rend.get(), path) }
			{
				if (!_texture)
					throw std::runtime_error("Error creating texture.");
			}

			template <typename Renderer>
			explicit texture(const Renderer &rend, const std::string &path) : texture{ rend, path.c_str() }
			{
				// empty
			}

			template <typename Renderer>
			texture(const Renderer &rend, const surface &surface) : _texture{ SDL_CreateTextureFromSurface(rend.get(), surface.get()) }
			{
				if (!_texture)
					throw std::runtime_error("Error creating texture.");
			}

			template <typename Renderer>
			texture(const Renderer &rend, const pixel_format format, const texture_access access, const iarea size)
				: _texture{ SDL_CreateTexture(rend.get(), static_cast<std::underlying_type_t<pixel_format>>(format), static_cast<std::underlying_type_t<texture_access>>(access), size.width, size.height) }
			{
				if (!_texture)
					throw std::runtime_error("Error creating texture.");
			}

			[[nodiscard]] int width() const noexcept
			{
				const auto [width, height] = size();
				return width;
			}

			[[nodiscard]] int height() const noexcept
			{
				const auto [width, height] = size();
				return height;
			}

			[[nodiscard]] iarea size() const noexcept
			{
				int width{};
				int height{};
				SDL_QueryTexture(_texture.get(), nullptr, nullptr, &width, &height);
				return { width, height };
			}

			[[nodiscard]] SDL_Texture *get() const noexcept
			{
				return _texture.get();
			}

		private:
			struct deleter final
			{
				void operator()(SDL_Texture *texture)
				{
					SDL_DestroyTexture(texture);
				}
			};

			std::unique_ptr<SDL_Texture, deleter> _texture;
		};
	}
}
