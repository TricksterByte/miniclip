#pragma once

namespace puyo
{
	enum color_t
	{
		red = 0x1,
		yellow = 0x2,
		green = 0x3,
		blue = 0x4
	};

	struct color final
	{
		color_t blob_color;
	};
}
