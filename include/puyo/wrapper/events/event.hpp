#pragma once

#include <SDL.h>

#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

#include "base_event.hpp"
#include "event_type.hpp"
#include "keyboard_event.hpp"
#include "quit_event.hpp"
#include "user_event.hpp"
#include "window_event.hpp"

namespace puyo
{
	namespace sdl
	{
		class event final
		{
		public:
			event() noexcept = default;

			explicit event(const SDL_Event &event) noexcept : _event{ event }
			{
				_update_data(static_cast<event_type>(event.type));
			}

			template <typename T>
			explicit event(const base_event<T> &event) noexcept : _event{ as_sdl_event(event) }
			{
				_update_data(event.type());
			}

			static void update() noexcept
			{
				SDL_PumpEvents();
			}

			static void push(event &event) noexcept
			{
				auto &sdl_event = event._event;
				SDL_PushEvent(&sdl_event);
			}

			template <typename T>
			static void push(const base_event<T> &event) noexcept
			{
				auto sdl_event = as_sdl_event(event);
				return SDL_PushEvent(&sdl_event);
			}

			static void flush() noexcept
			{
				SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
			}

			static void flush_all() noexcept
			{
				SDL_PumpEvents();
				SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
			}

			bool poll() noexcept
			{
				const bool result = SDL_PollEvent(&_event);

				if (result)
					_update_data(static_cast<event_type>(_event.type));
				else
					_data.emplace<std::monostate>();

				return result;
			}

			[[nodiscard]] std::optional<event_type> type() const noexcept
			{
				if (is_empty())
					return std::nullopt;
				else
					return static_cast<event_type>(_event.type);
			}

			[[nodiscard]] static std::optional<int> queue_count() noexcept
			{
				const auto num = SDL_PeepEvents(nullptr, 0, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
				if (num != -1)
					return num;
				else
					return std::nullopt;
			}

			[[nodiscard]] static std::optional<int> queue_count(const event_type type) noexcept
			{
				const auto id = static_cast<std::underlying_type_t<event_type>>(type);
				const auto num = SDL_PeepEvents(nullptr, 0, SDL_PEEKEVENT, id, id);
				if (num != -1)
					return num;
				else
					return std::nullopt;
			}

			[[nodiscard]] static bool in_queue(const event_type type) noexcept
			{
				return queue_count(type) > 0;
			}

			template <typename T>
			[[nodiscard]] bool is() const noexcept
			{
				return std::holds_alternative<T>(_data);
			}

			template <typename T>
			[[nodiscard]] T &get()
			{
				return std::get<T>(_data);
			}

			template <typename T>
			[[nodiscard]] T *try_get() noexcept
			{
				return std::get_if<T>(&_data);
			}

			template <typename T>
			[[nodiscard]] const T *try_get() noexcept
			{
				return std::get_if<T>(&_data);
			}

			[[nodiscard]] const SDL_Event *data() const noexcept
			{
				return &_event;
			}

			[[nodiscard]] bool is_empty() const noexcept
			{
				return is<std::monostate>();
			}

		private:
			using data_type = std::variant<
				std::monostate,
				keyboard_event,
				quit_event,
				user_event,
				window_event>;

			SDL_Event _event{};
			data_type _data{};

			void _update_data(const event_type type) noexcept
			{
				switch (type)
				{
				case event_type::quit:
					_data.emplace<quit_event>(_event.quit);
					break;

				case event_type::window:
					_data.emplace<window_event>(_event.window);
					break;

				case event_type::system:
					break;

				case event_type::key_down:
				case event_type::key_up:
					_data.emplace<keyboard_event>(_event.key);
					break;

				case event_type::user:
					_data.emplace<user_event>(_event.user);
					break;

				default:
					_data.emplace<std::monostate>();
					break;
				}
			}
		};
	}
}
