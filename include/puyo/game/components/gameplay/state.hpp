#pragma once

namespace puyo
{
	enum state_t
	{
		dropping,
		placed
	};

	struct state final
	{
		state_t blob_state;
	};
}
