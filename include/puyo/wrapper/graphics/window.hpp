#pragma once

#include <SDL.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include "enums.hpp"
#include "../math/area.hpp"

namespace puyo
{
	namespace sdl
	{
		class window final
		{
		public:
			enum window_flags : std::uint32_t
			{
				fullscreen = SDL_WINDOW_FULLSCREEN,
				opengl = SDL_WINDOW_OPENGL,
				shown = SDL_WINDOW_SHOWN,
				hidden = SDL_WINDOW_HIDDEN,
				borderless = SDL_WINDOW_BORDERLESS,
				resizable = SDL_WINDOW_RESIZABLE,
				minimized = SDL_WINDOW_MINIMIZED,
				maximized = SDL_WINDOW_MAXIMIZED,
				input_grabbed = SDL_WINDOW_INPUT_GRABBED,
				input_focus = SDL_WINDOW_INPUT_FOCUS,
				mouse_focus = SDL_WINDOW_MOUSE_FOCUS,
				fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
				foreign = SDL_WINDOW_FOREIGN,
				high_dpi = SDL_WINDOW_ALLOW_HIGHDPI,
				mouse_capture = SDL_WINDOW_MOUSE_CAPTURE,
				always_on_top = SDL_WINDOW_ALWAYS_ON_TOP,
				skip_taskbar = SDL_WINDOW_SKIP_TASKBAR,
				utility = SDL_WINDOW_UTILITY,
				tooltip = SDL_WINDOW_TOOLTIP,
				popup_menu = SDL_WINDOW_POPUP_MENU,
				vulkan = SDL_WINDOW_VULKAN
			};

			explicit window(SDL_Window *window) : _window{ window }
			{
				if (!_window)
					throw std::runtime_error("Cannot create window from a null pointer.");
			}

			explicit window(const char *title, const sdl::iarea size = default_size(), const std::uint32_t flags = 0)
			{
				if (size.width < 1 || size.height < 1)
					throw std::logic_error("Window width and height must be positive integers.");

				_window.reset(SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, flags));

				if (!_window)
					throw std::runtime_error("Error creating window.");
			}

			explicit window(const std::string &title, const sdl::iarea size = default_size(), const std::uint32_t flags = 0)
				: window(title.c_str(), size, flags)
			{
				// empty
			}

			void show() noexcept
			{
				SDL_ShowWindow(_window.get());
			}

			void hide() noexcept
			{
				SDL_HideWindow(_window.get());
			}

			void raise() noexcept
			{
				SDL_RaiseWindow(_window.get());
			}

			void maximize() noexcept
			{
				SDL_MaximizeWindow(_window.get());
			}

			void minimize() noexcept
			{
				SDL_MinimizeWindow(_window.get());
			}

			[[nodiscard]] SDL_Window *get() const noexcept
			{
				return _window.get();
			}

			[[nodiscard]] pixel_format get_pixel_format() const noexcept
			{
				return static_cast<pixel_format>(SDL_GetWindowPixelFormat(_window.get()));
			}

			[[nodiscard]] constexpr static sdl::iarea default_size() noexcept
			{
				return { 640, 480 };
			}

		private:
			struct deleter final
			{
				void operator()(SDL_Window *window) noexcept
				{
					SDL_DestroyWindow(window);
				}
			};

			std::unique_ptr<SDL_Window, deleter> _window;
		};
	}
}
