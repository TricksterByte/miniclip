#pragma once

#include "../../core/input.hpp"

#include "../../ctx/binds.hpp"

namespace puyo
{
	namespace sys
	{
		bool handle_general_input(bool &paused, const input &input)
		{
			if (input.keyboard.just_pressed(ctx::binds::restart))
				return true;
			else if (input.keyboard.just_pressed(ctx::binds::pause))
			{
				paused = !paused;
			}

			return false;
		}
	}
}
