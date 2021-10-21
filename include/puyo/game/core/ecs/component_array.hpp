#pragma once

#include <array>
#include <cassert>
#include <unordered_map>

#include "entity.hpp"

namespace puyo
{
	class component_array_base
	{
	public:
		virtual ~component_array_base() = default;
		virtual void entity_destroyed(entity) = 0;
	};

	template <typename T>
	class component_array : public component_array_base
	{
	public:
		void insert_data(entity id, T component)
		{
			// assert(_entity_to_index.find(id) == _entity_to_index.end() && "Component added to same entity more than once.");

			std::size_t new_index = _size;
			_entity_to_index[id] = new_index;
			_index_to_entity[new_index] = id;
			_component_array[new_index] = component;
			++_size;
		}

		void remove_data(entity id)
		{
			// assert(_entity_to_index.find(id) != _entity_to_index.end() && "Removing non-existent component.");

			std::size_t removed = _entity_to_index[id];
			std::size_t last = _size - 1;
			_component_array[removed] = _component_array[last];

			entity entity_last = _index_to_entity[last];
			_entity_to_index[entity_last] = removed;
			_index_to_entity[removed] = entity_last;

			_entity_to_index.erase(id);
			_index_to_entity.erase(last);

			--_size;
		}

		[[nodiscard]] T &get_data(entity id)
		{
			// assert(_entity_to_index.find(id) != _entity_to_index.end() && "Retrieving non-existent component.");

			return _component_array[_entity_to_index[id]];
		}

		void entity_destroyed(entity id)
		{
			if (_entity_to_index.find(id) != _entity_to_index.end())
				remove_data(id);
		}

	protected:
		std::array<T, max_entities> _component_array{};
		std::unordered_map<entity, std::size_t> _entity_to_index;
		std::unordered_map<std::size_t, entity> _index_to_entity;
		std::size_t _size;
	};
}
