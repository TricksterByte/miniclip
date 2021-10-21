#include "puyo/game/core/constants.hpp"
#include "puyo/game/core/graphics.hpp"

namespace puyo
{
	graphics::graphics(const sdl::window &win) : _renderer{ win }
	{
		_renderer.set_logical_size(logical_size<>);
		_textures.reserve(10);
	}

	void graphics::render(const std::size_t idx, const sdl::irect &src, const sdl::frect &dst) noexcept
	{
		const sdl::texture &tex = find(idx);
		_renderer.render_t(tex, src, dst);
	}

	std::size_t graphics::load(const std::uint8_t id, const std::string &path)
	{
		if (const auto it = _identifiers.find(id); it != _identifiers.end())
			return it->second;

		const auto index = _textures.size();

		_textures.emplace_back(_renderer, path);
		_identifiers.try_emplace(id, index);

		return std::size_t{ index };
	}

	std::size_t graphics::to_index(const std::uint8_t id) const
	{
		return _identifiers.at(id);
	}

	const sdl::texture &graphics::find(std::size_t idx) const noexcept
	{
		assert(idx < _textures.size());
		return _textures[idx];
	}
}
