#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <cstdint>
#include <optional>
#include <stdexcept>

namespace puyo
{
	namespace sdl
	{
		struct config
		{
			bool core = true;
			bool image = true;
			bool mixer = false;
			bool ttf = false;

			std::uint32_t core_flags = SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS;

			int image_flags = IMG_INIT_PNG | IMG_INIT_JPG;
			
			int mixer_flags = MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPUS;
			int mixer_freq = MIX_DEFAULT_FREQUENCY;
			std::uint16_t mixer_format = MIX_DEFAULT_FORMAT;
			int mixer_channels = MIX_DEFAULT_CHANNELS;
			int mixer_chunk_size = 4096;
		};

		class lib final
		{
		public:
			lib()
			{
				_init();
			}

			explicit lib(const config &cfg) : _cfg(cfg)
			{
				_init();
			}

			lib(const lib &) = delete;
			lib &operator=(const lib &) = delete;

			lib(lib &&) = delete;
			lib &operator=(lib &&) = delete;

		private:
			struct sdl_core final
			{
				explicit sdl_core(const std::uint32_t flags)
				{
					if (SDL_Init(flags) < 0)
						throw std::runtime_error("Error initializing SDL.");
				}

				~sdl_core() noexcept
				{
					SDL_Quit();
				}
			};

			struct sdl_image final
			{
				explicit sdl_image(const int flags)
				{
					if (!IMG_Init(flags))
						throw std::runtime_error("Error initializing SDL_image.");
				}

				~sdl_image() noexcept
				{
					IMG_Quit();
				}
			};

			struct sdl_mixer final
			{
				explicit sdl_mixer(const int flags, const int freq, const std::uint16_t format, const int channels, const int chunk_sz)
				{
					if (!Mix_Init(flags))
						throw std::runtime_error("Error initializing SDL_mixer.");
					if (Mix_OpenAudio(freq, format, channels, chunk_sz) == -1)
						throw std::runtime_error("Error opening audio channels.");
				}

				~sdl_mixer() noexcept
				{
					Mix_CloseAudio();
					Mix_Quit();
				}
			};

			struct sdl_ttf final
			{
				explicit sdl_ttf()
				{
					if (TTF_Init() == -1)
						throw std::runtime_error("Error initializing SDL_ttf.");
				}

				~sdl_ttf() noexcept
				{
					TTF_Quit();
				}
			};

			void _init()
			{
				if (_cfg.core)
					_sdl.emplace(_cfg.core_flags);
				if (_cfg.image)
					_sdl_image.emplace(_cfg.image_flags);
				if (_cfg.mixer)
					_sdl_mixer.emplace(_cfg.mixer_flags, _cfg.mixer_freq, _cfg.mixer_format, _cfg.mixer_channels, _cfg.mixer_chunk_size);
				if (_cfg.ttf)
					_sdl_ttf.emplace();
			}

			config _cfg;
			std::optional<sdl_core> _sdl;
			std::optional<sdl_image> _sdl_image;
			std::optional<sdl_mixer> _sdl_mixer;
			std::optional<sdl_ttf> _sdl_ttf;
		};
	}
}
