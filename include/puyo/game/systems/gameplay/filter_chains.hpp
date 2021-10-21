#pragma once

#include <algorithm>

#include "../../core/ecs/coordinator.hpp"
#include "../../core/ecs/entity.hpp"

#include "../../components/gameplay/belonging_chain.hpp"
#include "../../components/gameplay/chains.hpp"

namespace puyo
{
	namespace sys
	{
		bool filter_chains(coordinator &coord, entity &ch)
		{
			auto &c = coord.get_component<chains>(ch);

			for (auto it = c.blob_chains.begin(); it != c.blob_chains.end();)
			{
				if (it->second.size() < 4)
				{
					std::for_each(it->second.begin(), it->second.end(),
						[&](entity &e)
						{
							auto &b = coord.get_component<belonging_chain>(e);
							b.chain = 0u;
						}
					);
					coord.destroy_entity(it->first);
					c.blob_chains.erase(it++);
				}
				else
					++it;
			}

			return c.blob_chains.size() > 0;
		}
	}
}
