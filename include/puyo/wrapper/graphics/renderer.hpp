#pragma once

#include <SDL.h>

#include <cassert>
#include <cstdint>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>

#include "../fonts/font.hpp"
#include "../fonts/font_cache.hpp"

#include "../math/area.hpp"
#include "../math/point.hpp"
#include "../math/rect.hpp"

#include "color.hpp"
#include "texture.hpp"
#include "window.hpp"

namespace puyo
{
	namespace sdl
	{
		class renderer final
		{
		public:
			enum renderer_flags : std::uint32_t
			{
				software = SDL_RENDERER_SOFTWARE,
				accelerated = SDL_RENDERER_ACCELERATED,
				target_texture = SDL_RENDERER_TARGETTEXTURE,
				vsync = SDL_RENDERER_PRESENTVSYNC
			};

			explicit renderer(SDL_Renderer *renderer) : _renderer{ renderer }
			{
				if (!_renderer)
					throw std::runtime_error("Cannot create renderer from a null pointer.");
			}

			explicit renderer(const window &win, const std::uint32_t flags = software)
			{
				_renderer.reset(SDL_CreateRenderer(win.get(), -1, flags));

				if (!_renderer)
					throw std::runtime_error("Error creating renderer.");
			}

			[[nodiscard]] SDL_Renderer *get() const noexcept
			{
				return _renderer.get();
			}

			void clear() noexcept
			{
				SDL_RenderClear(_renderer.get());
			}

			void clear_with(const color &color) noexcept
			{
				const auto old_color = get_color();
				set_color(color);

				clear();
				set_color(old_color);
			}

			void present() noexcept
			{
				SDL_RenderPresent(_renderer.get());
			}

			void fill() noexcept
			{
				fill_rect<int>({ { }, output_size() });
			}

			void fill_with(const color &color) noexcept
			{
				const auto old_color = get_color();

				set_color(color);
				fill();

				set_color(old_color);
			}

			template <typename U>
			bool draw_rect(const basic_rect<U> &rect) noexcept
			{
				if constexpr (basic_rect<U>::is_integral)
					return SDL_RenderDrawRect(get(), rect.data()) == 0;
				else
					return SDL_RenderDrawRectF(get(), rect.data()) == 0;
			}

			template <typename U>
			bool fill_rect(const basic_rect<U> &rect) noexcept
			{
				if constexpr (basic_rect<U>::is_integral)
					return SDL_RenderFillRect(get(), rect.data()) == 0;
				else
					return SDL_RenderFillRectF(get(), rect.data()) == 0;
			}

			template <typename U>
			bool draw_line(const basic_point<U> &start, const basic_point<U> &end) noexcept
			{
				if constexpr (basic_point<U>::is_integral)
					return SDL_RenderDrawLine(get(), start.x(), start.y(), end.x(), end.y()) == 0;
				else
					return SDL_RenderDrawLineF(get(), start.x(), start.y(), end.x(), end.y()) == 0;
			}

			template <typename Container>
			bool draw_lines(const Container &container) noexcept
			{
				using point_t = typename Container::value_type;
				using value_t = typename point_t::value_type;

				if (!container.empty())
				{
					const auto &front = container.front();
					const auto *first = front.data();

					if constexpr (std::is_same_v<value_t, int>)
						return SDL_RenderDrawLines(get(), first, container.size()) == 0;
					else
						return SDL_RenderDrawLinesF(get(), first, container.sie()) == 0;
				}
				else
					return false;
			}

			template <typename U>
			bool draw_point(const basic_point<U> &point) noexcept
			{
				if constexpr (basic_point<U>::is_integral)
					return SDL_RenderDrawPoint(get(), point.x(), point.y()) == 0;
				else
					return SDL_RenderDrawPointF(get(), point.x(), point.y()) == 0;
			}

			template <typename U>
			bool draw_rect_t(const basic_rect<U> &rect) noexcept
			{
				return draw_rect(_translate(rect));
			}

			template <typename U>
			bool fill_rect_t(const basic_rect<U> &rect) noexcept
			{
				return fill_rect(_translate(rect));
			}

			template <typename U>
			bool draw_point_t(const basic_point<U> &point) noexcept
			{
				return draw_point(_translate(point));
			}

			[[nodiscard]] texture render_blended_utf8(const char *str, const font &font)
			{
				return _render_text(TTF_RenderUTF8_Blended(font.get(), str, get_color().get()));
			}

			[[nodiscard]] texture render_blended_utf8(const std::string &str, const font &font)
			{
				return render_blended_utf8(str.c_str(), font);
			}

			[[nodiscard]] texture render_blended_wrapped_utf8(const char *str, const font &font, const std::uint32_t wrap)
			{
				return _render_text(TTF_RenderUTF8_Blended_Wrapped(font.get(), str, get_color().get(), wrap));
			}

			[[nodiscard]] texture render_blended_wrapped_utf8(const std::string &str, const font &font, std::uint32_t wrap)
			{
				return render_blended_wrapped_utf8(str.c_str(), font, wrap);
			}

			[[nodiscard]] texture render_shaded_utf8(const char *str, const font &font, const color &background)
			{
				return _render_text(TTF_RenderUTF8_Shaded(font.get(), str, get_color().get(), background.get()));
			}

			[[nodiscard]] texture render_shaded_utf8(const std::string &str, const font &font, const color &background)
			{
				return render_shaded_utf8(str.c_str(), font, background);
			}

			[[nodiscard]] texture render_solid_utf8(const char *str, const font &font)
			{
				return _render_text(TTF_RenderUTF8_Solid(font.get(), str, get_color().get()));
			}

			[[nodiscard]] texture render_solid_utf8(const std::string &str, const font &font)
			{
				return render_solid_utf8(str.c_str(), font);
			}

			[[nodiscard]] texture render_blended_unicode(const unicode_string &str, const font &font)
			{
				return _render_text(TTF_RenderUNICODE_Blended(font.get(), str.data(), get_color().get()));
			}

			[[nodiscard]] texture render_blended_wrapped_unicode(const unicode_string &str, const font &font, const std::uint32_t wrap)
			{
				return _render_text(TTF_RenderUNICODE_Blended_Wrapped(font.get(), str.data(), get_color().get(), wrap));
			}

			[[nodiscard]] texture render_shaded_unicode(const unicode_string &str, const font &font, const color &background)
			{
				return _render_text(TTF_RenderUNICODE_Shaded(font.get(), str.data(), get_color().get(), background.get()));
			}

			[[nodiscard]] texture render_solid_unicode(const unicode_string &str, const font &font)
			{
				return _render_text(TTF_RenderUNICODE_Solid(font.get(), str.data(), get_color().get()));
			}

			int render_glyph(const font_cache &cache, const unicode glyph, const ipoint position)
			{
				if (const auto *data = cache.try_at(glyph))
				{
					const auto &[texture, metrics] = *data;

					const auto outline = cache.get_font().outline();

					const auto x = position.x() + metrics.min_x - outline;
					const auto y = position.y() - outline;

					render(texture, ipoint{ x, y });

					return x + metrics.advance;
				}
				else
					return position.x();
			}

			template <typename String>
			void render_text(const font_cache &cache, const String &str, ipoint position)
			{
				const auto &font = cache.get_font();

				const auto original_x = position.x();
				const auto line_skip = font.line_skip();

				for (const unicode glyph : str)
				{
					if (glyph == '\n')
					{
						position.set_x(original_x);
						position.set_y(position.y() + line_skip);
					}
					else
					{
						const auto x = render_glyph(cache, glyph, position);
						position.set_x(x);
					}
				}
			}

			void set_translation_viewport(const frect &viewport) noexcept
			{
				_translation = viewport;
			}

			[[nodiscard]] const frect &translation_viewport() const noexcept
			{
				return _translation;
			}

			void add_font(const std::size_t id, font &&font)
			{
				auto &fonts = _fonts;
				if (const auto it = fonts.find(id); it != fonts.end())
					fonts.erase(it);
				fonts.try_emplace(id, std::move(font));
			}

			template <typename... Args>
			void emplace_font(const std::size_t id, Args &&...args)
			{
				auto &fonts = _fonts;
				if (const auto it = fonts.find(id); it != fonts.end())
					fonts.erase(it);
				fonts.try_emplace(id, std::forward<Args>(args)...);
			}

			void remove_font(const std::size_t id)
			{
				_fonts.erase(id);
			}

			[[nodiscard]] font &get_font(const std::size_t id)
			{
				return _fonts.at(id);
			}

			[[nodiscard]] const font &get_font(const std::size_t id) const
			{
				return _fonts.at(id);
			}

			[[nodiscard]] bool has_font(const std::size_t id) const
			{
				return _fonts.find(id) != _fonts.end();
			}

			bool set_clip(const std::optional<irect> area) noexcept
			{
				return SDL_RenderSetClipRect(get(), area ? area->data() : nullptr) == 0;
			}

			bool set_viewport(const irect viewport) noexcept
			{
				return SDL_RenderSetViewport(get(), viewport.data()) == 0;
			}

			bool set_blend_mode(const blend_mode mode) noexcept
			{
				return SDL_SetRenderDrawBlendMode(get(), static_cast<SDL_BlendMode>(mode)) == 0;
			}

			bool reset_target() noexcept
			{
				return SDL_SetRenderTarget(get(), nullptr) == 0;
			}

			bool set_scale(const float x, const float y) noexcept
			{
				assert(x > 0 && y > 0);
				return SDL_RenderSetScale(get(), x, y) == 0;
			}

			bool set_logical_integer_scaling(const bool enabled) noexcept
			{
				return SDL_RenderSetIntegerScale(get(), static_cast<SDL_bool>(enabled)) == 0;
			}


			[[nodiscard]] texture get_render_target()
			{
				return texture{ SDL_GetRenderTarget(get()) };
			}

			[[nodiscard]] int logical_width() const noexcept
			{
				int width{};
				SDL_RenderGetLogicalSize(get(), &width, nullptr);
				return width;
			}

			[[nodiscard]] int logical_height() const noexcept
			{
				int height{};
				SDL_RenderGetLogicalSize(get(), nullptr, &height);
				return height;
			}

			void set_logical_size(const iarea size) noexcept
			{
				assert(size.width >= 0 && size.height >= 0);
				SDL_RenderSetLogicalSize(get(), size.width, size.height);
			}

			void set_color(const color &color) noexcept
			{
				SDL_SetRenderDrawColor(
					_renderer.get(),
					color.red(),
					color.green(),
					color.blue(),
					color.alpha()
				);
			}

			[[nodiscard]] iarea logical_size() const noexcept
			{
				iarea size{};
				SDL_RenderGetLogicalSize(get(), &size.width, &size.height);
				return size;
			}

			[[nodiscard]] float x_scale() const noexcept
			{
				float x{};
				SDL_RenderGetScale(get(), &x, nullptr);
				return x;
			}

			[[nodiscard]] float y_scale() const noexcept
			{
				float y{};
				SDL_RenderGetScale(get(), nullptr, &y);
				return y;
			}

			[[nodiscard]] std::pair<float, float> scale() const noexcept
			{
				float x{};
				float y{};
				SDL_RenderGetScale(get(), &x, &y);
				return { x, y };
			}

			[[nodiscard]] std::optional<irect> clip() const noexcept
			{
				irect rect{};
				SDL_RenderGetClipRect(get(), rect.data());
				if (!rect.has_area())
					return std::nullopt;
				else
					return rect;
			}

			[[nodiscard]] int output_width() const noexcept
			{
				int width{};
				SDL_GetRendererOutputSize(get(), &width, nullptr);
				return width;
			}

			[[nodiscard]] int output_height() const noexcept
			{
				int height{};
				SDL_GetRendererOutputSize(get(), nullptr, &height);
				return height;
			}

			[[nodiscard]] iarea output_size() const noexcept
			{
				iarea size{};
				SDL_GetRendererOutputSize(get(), &size.width, &size.height);
				return size;
			}

			[[nodiscard]] blend_mode get_blend_mode() const noexcept
			{
				SDL_BlendMode mode{};
				SDL_GetRenderDrawBlendMode(get(), &mode);
				return static_cast<blend_mode>(mode);
			}

			[[nodiscard]] bool is_using_interger_logical_scaling() const noexcept
			{
				return SDL_RenderGetIntegerScale(get());
			}

			[[nodiscard]] bool is_clipping_enabled() const noexcept
			{
				return SDL_RenderIsClipEnabled(get());
			}

			[[nodiscard]] color get_color() const noexcept
			{
				std::uint8_t red{};
				std::uint8_t green{};
				std::uint8_t blue{};
				std::uint8_t alpha{};
				SDL_GetRenderDrawColor(get(), &red, &green, &blue, &alpha);
				return { red, green, blue, alpha };
			}

			[[nodiscard]] irect viewport() const noexcept
			{
				irect viewport{};
				SDL_RenderGetViewport(get(), viewport.data());
				return viewport;
			}

			[[nodiscard]] constexpr static std::uint32_t default_flags() noexcept
			{
				return accelerated | vsync;
			}

			template <typename P>
			bool render(const texture &tex, const basic_point<P> &at) noexcept
			{
				if constexpr (basic_point<P>::is_floating)
				{
					const farea size = tex.size();
					const SDL_FRect dst{ at.x(), at.y(), size.width, size.height };
					return SDL_RenderCopyF(get(), tex.get(), nullptr, &dst) == 0;
				}
				else
				{
					const SDL_Rect dst{ at.x(), at.y(), tex.width(), tex.height() };
					return SDL_RenderCopy(get(), tex.get(), nullptr, &dst) == 0;
				}
			}

			template <typename P>
			bool render(const texture &tex, const basic_rect<P> &dst) noexcept
			{
				if constexpr (basic_rect<P>::is_floating)
				{
					return SDL_RenderCopyF(get(), tex.get(), nullptr, dst.data()) == 0;
				}
				else
				{
					return SDL_RenderCopy(get(), tex.get(), nullptr, dst.data()) == 0;
				}
			}

			template <typename P>
			bool render(const texture &tex, const irect &src, const basic_rect<P> &dst) noexcept
			{
				if constexpr (basic_rect<P>::is_floating)
				{
					return SDL_RenderCopyF(get(), tex.get(), src.data(), dst.data()) == 0;
				}
				else
				{
					return SDL_RenderCopy(get(), tex.get(), src.data(), dst.data()) == 0;
				}
			}

			template <typename P>
			bool render_t(const texture &tex, const basic_point<P> &position) noexcept
			{
				return render(tex, _translate(position));
			}

			template <typename P>
			bool render_t(const texture &tex, const basic_rect<P> &dst) noexcept
			{
				return render(tex, _translate(dst));
			}

			template <typename P>
			bool render_t(const texture &tex, const irect &src, const basic_rect<P> &dst) noexcept
			{
				return render(tex, src, _translate(dst));
			}

		private:
			struct deleter final
			{
				void operator()(SDL_Renderer *renderer) noexcept
				{
					SDL_DestroyRenderer(renderer);
				}
			};

			std::unique_ptr<SDL_Renderer, deleter> _renderer;
			frect _translation{};
			std::unordered_map<std::size_t, font> _fonts{};

			[[nodiscard]] texture _render_text(SDL_Surface *s)
			{
				surface surface{ s };
				texture texture{ SDL_CreateTextureFromSurface(get(), surface.get()) };
				return texture;
			}

			template <typename P>
			[[nodiscard]] basic_point<P> _translate(const basic_point<P> &point) const noexcept
			{
				using value_type = typename basic_point<P>::value_type;

				const auto x = point.x() - static_cast<value_type>(_translation.x());
				const auto y = point.y() - static_cast<value_type>(_translation.y());

				return basic_point<P>{ x, y };
			}

			template <typename P>
			[[nodiscard]] basic_rect<P> _translate(const basic_rect<P> &rect) const noexcept
			{
				return basic_rect<P>{ _translate(rect.position()), rect.size() };
			}
		};
	}
}
