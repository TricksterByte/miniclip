#pragma once

#include <map>
#include <vector>

#include "../../core/ecs/entity.hpp"

namespace puyo
{
	struct chains final
	{
		std::map<entity, std::vector<entity>> blob_chains;
	};
}
