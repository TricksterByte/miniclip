#pragma once

#include <SDL.h>

#include <cstdint>

namespace puyo
{
	namespace sdl
	{
		enum class pixel_format : std::uint32_t
		{
			unknown = SDL_PIXELFORMAT_UNKNOWN,

			index1lsb = SDL_PIXELFORMAT_INDEX1LSB,
			index1msb = SDL_PIXELFORMAT_INDEX1MSB,
			index4lsb = SDL_PIXELFORMAT_INDEX4LSB,
			index4msb = SDL_PIXELFORMAT_INDEX4MSB,
			index8 = SDL_PIXELFORMAT_INDEX8,

			xrgb4444 = SDL_PIXELFORMAT_XRGB4444,
			xbgr4444 = SDL_PIXELFORMAT_XBGR4444,

			xrgb1555 = SDL_PIXELFORMAT_XRGB1555,
			xbgr1555 = SDL_PIXELFORMAT_XBGR1555,

			xrgb8888 = SDL_PIXELFORMAT_XRGB8888,
			xbgr8888 = SDL_PIXELFORMAT_XBGR8888,

			rgb332 = SDL_PIXELFORMAT_RGB332,
			rgb444 = SDL_PIXELFORMAT_RGB444,
			bgr444 = SDL_PIXELFORMAT_BGR444,

			rgb555 = SDL_PIXELFORMAT_RGB555,
			bgr555 = SDL_PIXELFORMAT_BGR555,

			argb4444 = SDL_PIXELFORMAT_ARGB4444,
			rgba4444 = SDL_PIXELFORMAT_RGBA4444,
			abgr4444 = SDL_PIXELFORMAT_ABGR4444,
			bgra4444 = SDL_PIXELFORMAT_BGRA4444,

			argb1555 = SDL_PIXELFORMAT_ARGB1555,
			rgba5551 = SDL_PIXELFORMAT_RGBA5551,
			abgr1555 = SDL_PIXELFORMAT_ABGR1555,
			bgra5551 = SDL_PIXELFORMAT_BGRA5551,

			rgb565 = SDL_PIXELFORMAT_RGB565,
			bgr565 = SDL_PIXELFORMAT_BGR565,

			rgb24 = SDL_PIXELFORMAT_RGB24,
			bgr24 = SDL_PIXELFORMAT_BGR24,

			rgb888 = SDL_PIXELFORMAT_RGB888,
			rgbx8888 = SDL_PIXELFORMAT_RGBX8888,
			bgr888 = SDL_PIXELFORMAT_BGR888,
			bgrx8888 = SDL_PIXELFORMAT_BGRX8888,

			argb8888 = SDL_PIXELFORMAT_ARGB8888,
			rgba8888 = SDL_PIXELFORMAT_RGBA8888,
			abgr8888 = SDL_PIXELFORMAT_ABGR8888,
			bgra8888 = SDL_PIXELFORMAT_BGRA8888,

			argb2101010 = SDL_PIXELFORMAT_ARGB2101010,

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rgba32 = SDL_PIXELFORMAT_RGBA8888,
			argb32 = SDL_PIXELFORMAT_ARGB8888,
			bgra32 = SDL_PIXELFORMAT_BGRA8888,
			abgr32 = SDL_PIXELFORMAT_ABGR8888,
#else
			rgba32 = SDL_PIXELFORMAT_ABGR8888,
			argb32 = SDL_PIXELFORMAT_BGRA8888,
			bgra32 = SDL_PIXELFORMAT_ARGB8888,
			abgr32 = SDL_PIXELFORMAT_RGBA8888,
#endif

			yv12 = SDL_PIXELFORMAT_YV12,
			iyuv = SDL_PIXELFORMAT_IYUV,
			yuy2 = SDL_PIXELFORMAT_YUY2,
			uyvy = SDL_PIXELFORMAT_UYVY,
			yvyu = SDL_PIXELFORMAT_YVYU,
			nv12 = SDL_PIXELFORMAT_NV12,
			nv21 = SDL_PIXELFORMAT_NV21,
			external_oes = SDL_PIXELFORMAT_EXTERNAL_OES
		};

		enum class blend_op : std::uint32_t
		{
			add = SDL_BLENDOPERATION_ADD,
			sub = SDL_BLENDOPERATION_SUBTRACT,
			reverse_sub = SDL_BLENDOPERATION_REV_SUBTRACT,
			min = SDL_BLENDOPERATION_MINIMUM,
			max = SDL_BLENDOPERATION_MAXIMUM
		};

		enum class blend_factor
		{
			zero = SDL_BLENDFACTOR_ZERO,
			one = SDL_BLENDFACTOR_ONE,

			src_color = SDL_BLENDFACTOR_SRC_COLOR,
			one_minus_src_color = SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR,

			src_alpha = SDL_BLENDFACTOR_SRC_ALPHA,
			one_minus_src_alpha = SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,

			dst_color = SDL_BLENDFACTOR_DST_COLOR,
			one_minus_dst_color = SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,

			dst_alpha = SDL_BLENDFACTOR_DST_ALPHA,
			one_minus_dst_alpha = SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA
		};

		enum class blend_mode
		{
			none = SDL_BLENDMODE_NONE,
			blend = SDL_BLENDMODE_BLEND,
			add = SDL_BLENDMODE_ADD,
			mode = SDL_BLENDMODE_MOD,
			mul = SDL_BLENDMODE_MUL,
			invalid = SDL_BLENDMODE_INVALID
		};

		struct blend_task final
		{
			blend_factor src;
			blend_factor dst;
			blend_op op;
		};

		[[nodiscard]] inline blend_mode compose_blend_mode(const blend_task &color, const blend_task &alpha)
		{
			const auto res = SDL_ComposeCustomBlendMode(
				static_cast<SDL_BlendFactor>(color.src),
				static_cast<SDL_BlendFactor>(color.dst),
				static_cast<SDL_BlendOperation>(color.op),
				static_cast<SDL_BlendFactor>(alpha.src),
				static_cast<SDL_BlendFactor>(alpha.dst),
				static_cast<SDL_BlendOperation>(alpha.op));

			return static_cast<blend_mode>(res);
		}

		enum class texture_access : int
		{
			no_lock = SDL_TEXTUREACCESS_STATIC,
			streaming = SDL_TEXTUREACCESS_STREAMING,
			target = SDL_TEXTUREACCESS_TARGET
		};

		enum class scale_mode
		{
			nearest = SDL_ScaleModeNearest,
			linear = SDL_ScaleModeLinear,
			best = SDL_ScaleModeBest
		};
	}
}
