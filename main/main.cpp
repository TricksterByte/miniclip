#include <stdexcept>

#include <puyo/common/log.hpp>
#include <puyo/wrapper/lib.hpp>
#include <puyo/game/core/engine.hpp>

int main(int argc, char *argv[])
{	
	puyo::log::init();
	puyo::log::logline(puyo::log::info, "Initialized log.");

	try
	{
		puyo::sdl::lib lib;
		puyo::log::logline(puyo::log::info, "Initialized SDL.");
	}
	catch (std::runtime_error &e)
	{
		puyo::log::logline(puyo::log::error, e.what());
	}

	try
	{
		puyo::engine engine;
		engine.run();
	}
	catch (std::exception &e)
	{
		puyo::log::logline(puyo::log::error, e.what());
	}

	return 0;
}
