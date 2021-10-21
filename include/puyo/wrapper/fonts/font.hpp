#pragma once

#include <SDL_ttf.h>

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include "../math/area.hpp"
#include "unicode_string.hpp"

namespace puyo
{
	namespace sdl
	{
		struct glyph_metrics final
		{
			int
				min_x,
				min_y,
				max_x,
				max_y,
				advance;
		};

		class font final
		{
		public:
			font(const char *file, const int size) : _size{ size }
			{
				if (size <= 0)
					throw std::length_error("Bad font size.");

				_font.reset(TTF_OpenFont(file, size));
				if (!_font)
					throw std::runtime_error("Error creating font.");
			}

			font(const std::string &file, const int size) : font{ file.c_str(), size }
			{
				// empty
			}

			void reset() noexcept
			{
				TTF_SetFontStyle(_font.get(), TTF_STYLE_NORMAL);
			}

			void set_bold(const bool bold) noexcept
			{
				if (bold)
					_add_style(TTF_STYLE_BOLD);
				else
					_remove_style(TTF_STYLE_BOLD);
			}

			void set_italic(const bool italic) noexcept
			{
				if (italic)
					_add_style(TTF_STYLE_ITALIC);
				else
					_remove_style(TTF_STYLE_ITALIC);
			}

			void set_underlined(const bool underlined) noexcept
			{
				if (underlined)
					_add_style(TTF_STYLE_UNDERLINE);
				else
					_remove_style(TTF_STYLE_UNDERLINE);
			}

			void set_strikethrough(const bool strikethrough) noexcept
			{
				if (strikethrough)
					_add_style(TTF_STYLE_STRIKETHROUGH);
				else
					_remove_style(TTF_STYLE_STRIKETHROUGH);
			}

			void set_outline(const int outline) noexcept
			{
				TTF_SetFontOutline(_font.get(), outline);
			}

			void set_kerning(const bool kerning) noexcept
			{
				TTF_SetFontKerning(_font.get(), kerning ? 1 : 0);
			}

			[[nodiscard]] bool is_bold() const noexcept
			{
				return TTF_GetFontStyle(_font.get()) & TTF_STYLE_BOLD;
			}

			[[nodiscard]] bool is_italic() const noexcept
			{
				return TTF_GetFontStyle(_font.get()) & TTF_STYLE_ITALIC;
			}

			[[nodiscard]] bool is_underlined() const noexcept
			{
				return TTF_GetFontStyle(_font.get()) & TTF_STYLE_UNDERLINE;
			}

			[[nodiscard]] bool is_strikethrough() const noexcept
			{
				return TTF_GetFontStyle(_font.get()) & TTF_STYLE_STRIKETHROUGH;
			}

			[[nodiscard]] int outline() const noexcept
			{
				return TTF_GetFontOutline(_font.get());
			}

			[[nodiscard]] bool has_kerning() const noexcept
			{
				return TTF_GetFontKerning(_font.get());
			}

			[[nodiscard]] int height() const noexcept
			{
				return TTF_FontHeight(_font.get());
			}

			[[nodisrcard]] int descent() const noexcept
			{
				return TTF_FontDescent(_font.get());
			}

			[[nodiscard]] int ascent() const noexcept
			{
				return TTF_FontAscent(_font.get());
			}

			[[nodiscard]] int line_skip() const noexcept
			{
				return TTF_FontLineSkip(_font.get());
			}

			[[nodiscard]] int font_faces() const noexcept
			{
				return static_cast<int>(TTF_FontFaces(_font.get()));
			}

			[[nodiscard]] bool is_outlined() const noexcept
			{
				return TTF_GetFontOutline(_font.get()) != 0;
			}

			[[nodiscard]] bool is_fixed_width() const noexcept
			{
				return TTF_FontFaceIsFixedWidth(_font.get());
			}

			[[nodiscard]] std::string family_name() const noexcept
			{
				return TTF_FontFaceFamilyName(_font.get());
			}

			[[nodiscard]] std::string style_name() const noexcept
			{
				return TTF_FontFaceStyleName(_font.get());
			}

			[[nodiscard]] int size() const noexcept
			{
				return _size;
			}

			[[nodiscard]] int kerning_amount(const unicode first, const unicode second) const noexcept
			{
				return TTF_GetFontKerningSizeGlyphs(_font.get(), first, second);
			}

			[[nodiscard]] bool is_glyph_provided(const unicode glyph) const noexcept
			{
				return TTF_GlyphIsProvided(_font.get(), glyph);
			}

			[[nodiscard]] std::optional<glyph_metrics> get_metrics(const unicode glyph) const noexcept
			{
				glyph_metrics metrics{};
				if (TTF_GlyphMetrics(_font.get(), glyph, &metrics.min_x, &metrics.min_y, &metrics.max_x, &metrics.max_y, &metrics.advance) != -1)
					return metrics;
				else
					return std::nullopt;
			}

			[[nodiscard]] std::optional<iarea> string_size(const char *str) const noexcept
			{
				iarea size{};
				if (TTF_SizeText(_font.get(), str, &size.width, &size.height) != -1)
					return size;
				else
					return std::nullopt;
			}

			[[nodiscard]] std::optional<iarea> string_size(const std::string &str) const noexcept
			{
				return string_size(str.c_str());
			}

			[[nodiscard]] std::optional<int> string_width(const char *str) const noexcept
			{
				if (const auto size = string_size(str))
					return size->width;
				else
					return std::nullopt;
			}

			[[nodiscard]] std::optional<int> string_width(const std::string &str) const noexcept
			{
				return string_width(str.c_str());
			}

			[[nodiscard]] std::optional<int> string_height(const char *str) const noexcept
			{
				if (const auto size = string_size(str))
					return size->height;
				else
					return std::nullopt;
			}

			[[nodiscard]] std::optional<int> string_height(const std::string &str) const noexcept
			{
				return string_height(str.c_str());
			}

			[[nodiscard]] explicit operator TTF_Font *() noexcept
			{
				return _font.get();
			}

			[[nodiscard]] explicit operator const TTF_Font *() noexcept
			{
				return _font.get();
			}

			[[nodiscard]] TTF_Font *get() const noexcept
			{
				return _font.get();
			}

		private:
			struct deleter final
			{
				void operator()(TTF_Font *font) noexcept
				{
					TTF_CloseFont(font);
				}
			};

			std::unique_ptr<TTF_Font, deleter> _font;
			int _size{};

			void _add_style(const int mask) noexcept
			{
				const auto style = TTF_GetFontStyle(_font.get());
				TTF_SetFontStyle(_font.get(), style | mask);
			}

			void _remove_style(const int mask) noexcept
			{
				const auto style = TTF_GetFontStyle(_font.get());
				TTF_SetFontStyle(_font.get(), style & ~mask);
			}
		};
	}
}
