#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

#include "../../wrapper/math/rect.hpp"

#include "../../wrapper/graphics/texture.hpp"
#include "../../wrapper/graphics/renderer.hpp"
#include "../../wrapper/graphics/window.hpp"

namespace puyo
{
	class graphics final
	{
	public:
		explicit graphics(const sdl::window &win);

		void render(std::size_t idx, const sdl::irect &src, const sdl::frect &dst) noexcept;
		std::size_t load(std::uint8_t id, const std::string &path);

		[[nodiscard]] std::size_t to_index(std::uint8_t id) const;
		[[nodiscard]] const sdl::texture &find(std::size_t idx) const noexcept;

		[[nodiscard]] sdl::renderer &renderer() noexcept
		{
			return _renderer;
		}

	private:
		sdl::renderer _renderer;

		std::vector<sdl::texture> _textures;
		std::unordered_map<std::size_t, std::size_t> _identifiers;
	};
}
