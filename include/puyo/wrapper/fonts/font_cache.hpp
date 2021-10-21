#pragma once

#include <SDL_ttf.h>

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include "../graphics/color.hpp"
#include "../graphics/surface.hpp"
#include "../graphics/texture.hpp"
#include "font.hpp"
#include "unicode_string.hpp"

namespace puyo
{
	namespace sdl
	{
		class font_cache final
		{
		public:
			using id_type = std::size_t;

			struct glyph_data final
			{
				texture cached;
				glyph_metrics metric;
			};

			explicit font_cache(font &&font) noexcept : _font{ std::move(font) }
			{
				// empty
			}

			template <typename... Args>
			explicit font_cache(Args &&...args) : _font{ std::forward<Args>(args)... }
			{
				// empty
			}

			template <typename Renderer>
			void store_blended_utf8(const id_type id, const char *str, Renderer renderer)
			{
				_store(id, renderer.render_blended_utf8(str, get_font()));
			}

			template <typename Renderer>
			void store_blended_utf8(const id_type id, const std::string &str, Renderer &renderer)
			{
				store_blended_utf8(id, str.c_str(), renderer);
			}

			template <typename Renderer>
			void store_blended_wrapped_utf8(const id_type id, const char *str, Renderer renderer, const std::uint32_t wrap)
			{
				_store(id, renderer.render_blended_wrapped_utf8(str, get_font(), wrap));
			}

			template <typename Renderer>
			void store_blended_wrapped_utf8(const id_type id, const std::string &str, Renderer &renderer, const std::uint32_t wrap)
			{
				store_blended_wrapped_utf8(id, str.c_str(), renderer, wrap);
			}

			template <typename Renderer>
			void store_shaded_utf8(const id_type id, const char *str, Renderer renderer, const color &background)
			{
				_store(id, renderer.render_shaded_utf8(str, get_font(), background));
			}

			template <typename Renderer>
			void store_shaded_utf8(const id_type id, const std::string &str, Renderer &renderer, const color &background)
			{
				store_shaded_utf8(id, str.c_str(), renderer, background);
			}

			template <typename Renderer>
			void store_solid_utf8(const id_type id, const char *str, Renderer renderer)
			{
				_store(id, renderer.render_solid_utf8(str, get_font()));
			}

			template <typename Renderer>
			void store_solid_utf8(const id_type id, const std::string &str, Renderer &renderer)
			{
				store_solid_utf8(id, str.c_str(), renderer);
			}

			template <typename Renderer>
			void store_blended_unicode(const id_type id, const unicode_string &str, Renderer renderer)
			{
				_store(id, renderer.render_blended_unicode(str, get_font()));
			}

			template <typename Renderer>
			void store_blended_wrapped_unicode(const id_type id, const unicode_string &str, Renderer renderer, const std::uint32_t wrap)
			{
				_store(id, renderer.render_blended_wrapped_unicode(str, get_font(), wrap));
			}

			template <typename Renderer>
			void store_shaded_unicode(const id_type id, const unicode_string &str, Renderer renderer, const color &background)
			{
				_store(id, renderer.render_shaded_unicode(str, get_font(), background));
			}

			template <typename Renderer>
			void store_solid_unicode(const id_type id, const unicode_string str, Renderer renderer)
			{
				_store(id, renderer.render_solid_unicode(str, get_font()));
			}

			[[nodiscard]] bool has_stored(const id_type id) const noexcept
			{
				return _strings.find(id) != _strings.end();
			}

			[[nodiscard]] const texture &get_stored(const id_type id) const
			{
				return _strings.at(id);
			}

			[[nodiscard]] const texture *try_get_stored(const id_type id) const noexcept
			{
				const auto it = _strings.find(id);
				if (it != _strings.end())
					return &it->second;
				else
					return nullptr;
			}

			template <typename Renderer>
			void add_glyph(Renderer &renderer, const unicode glyph)
			{
				if (has(glyph) || !_font.is_glyph_provided(glyph))
					return;
				glyph_data data{ _create_glyph_texture(renderer, glyph), _font.get_metrics(glyph).value() };
				_glyphs.try_emplace(glyph, std::move(data));
			}

			template <typename Renderer>
			void add_range(Renderer &renderer, const unicode begin, const unicode end)
			{
				for (auto ch = begin; ch < end; ++ch)
					add_glyph(renderer, ch);
			}

			[[nodiscard]] bool has(const unicode glyph) const noexcept
			{
				return _glyphs.count(glyph);
			}

			[[nodiscard]] const glyph_data &at(const unicode glyph) const
			{
				return _glyphs.at(glyph);
			}

			[[nodiscard]] const glyph_data &operator[](unicode glyph) const
			{
				return at(glyph);
			}

			[[nodiscard]] const glyph_data *try_at(const unicode glyph) const
			{
				if (const auto it = _glyphs.find(glyph); it != _glyphs.end())
					return &it->second;
				else
					return nullptr;
			}

			[[nodiscard]] font &get_font() noexcept
			{
				return _font;
			}

			[[nodiscard]] const font &get_font() const noexcept
			{
				return _font;
			}

		private:
			font _font;
			std::unordered_map<unicode, glyph_data> _glyphs;
			std::unordered_map<id_type, texture> _strings;

			template <typename Renderer>
			[[nodiscard]] texture _create_glyph_texture(Renderer &renderer, const unicode glyph)
			{
				const auto color = renderer.get_color().get();
				const surface src = { TTF_RenderGlyph_Blended(_font.get(), glyph, color) };
				return texture{ renderer, src };
			}

			void _store(const id_type id, texture &&texture)
			{
				if (const auto it = _strings.find(id); it != _strings.end())
					_strings.erase(it);
				_strings.try_emplace(id, std::move(texture));
			}
		};
	}
}
