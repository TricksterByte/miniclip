#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <queue>

#include "entity.hpp"
#include "signature.hpp"

namespace puyo
{
	class entity_manager final
	{
	public:
		entity_manager()
		{
			for (entity e = 1; e < max_entities; ++e)
				_available_entities.push(e);
		}

		entity_manager(const entity_manager &) = delete;
		entity_manager &operator=(const entity_manager &) = delete;

		entity_manager(entity_manager &&) = delete;
		entity_manager &operator=(entity_manager &&) = delete;

		[[nodiscard]] entity create_entity()
		{
			assert(_entity_count < max_entities && "Too many entities in existence.");

			entity id = _available_entities.front();
			_available_entities.pop();
			++_entity_count;

			return id;
		}

		void destroy_entity(entity id)
		{
			assert(id < max_entities && "Entity out of range.");

			_signatures[id].reset();

			_available_entities.push(id);
			--_entity_count;
		}

		void set_signature(entity id, signature sig)
		{
			assert(id < max_entities && "Entity out of range.");
			_signatures[id] = sig;
		}

		[[nodiscard]] signature get_signature(entity id)
		{
			assert(id < max_entities && "Entity out of range.");
			return _signatures[id];
		}

	private:
		std::queue<entity> _available_entities{};
		std::array<signature, max_entities> _signatures{};
		std::uint32_t _entity_count{ 0 };
	};
}
