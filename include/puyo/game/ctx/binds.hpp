#pragma once

#include "../../wrapper/input/scan_code.hpp"

namespace puyo
{
	namespace ctx
	{
		namespace binds
		{
			constexpr const sdl::scan_code left{ sdl::scancodes::a };
			constexpr const sdl::scan_code right{ sdl::scancodes::d };
			constexpr const sdl::scan_code up{ sdl::scancodes::w };
			constexpr const sdl::scan_code down{ sdl::scancodes::s };
			constexpr const sdl::scan_code rotl{ sdl::scancodes::q };
			constexpr const sdl::scan_code rotr{ sdl::scancodes::e };

			constexpr const sdl::scan_code pause{ sdl::scancodes::space };
			constexpr const sdl::scan_code restart{ sdl::scancodes::enter };
		}
	}
}
