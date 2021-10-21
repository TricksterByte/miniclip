#pragma once

#include <cassert>
#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "component.hpp"
#include "component_array.hpp"
#include "entity.hpp"

namespace puyo
{
	class component_manager final
	{
	public:
		template <typename T>
		void register_component()
		{
			const char *type_name = typeid(T).name();

			assert(_component_types.find(type_name) == _component_types.end() && "Registering component type more than once.");

			_component_types.insert({ type_name, _next_component_type });
			_component_arrays.insert({ type_name, std::make_shared<component_array<T>>() });

			++_next_component_type;
		}

		template <typename T>
		[[nodiscard]] component_type get_component_type()
		{
			const char *type_name = typeid(T).name();
			assert(_component_types.find(type_name) != _component_types.end() && "Component not registered before use.");
			return _component_types[type_name];
		}

		template <typename T>
		void add_component(entity id, T component)
		{
			_get_component_array<T>()->insert_data(id, component);
		}

		template <typename T>
		void remove_component(entity id)
		{
			_get_component_array<T>()->remove_data(id);
		}

		template <typename T>
		[[nodiscard]] T &get_component(entity id)
		{
			return _get_component_array<T>()->get_data(id);
		}

		void entity_destroyed(entity id)
		{
			for (auto const &pair : _component_arrays)
			{
				auto const &component = pair.second;
				component->entity_destroyed(id);
			}
		}

	private:
		std::unordered_map<const char *, component_type> _component_types{};
		std::unordered_map<const char *, std::shared_ptr<component_array_base>> _component_arrays{};

		component_type _next_component_type{};

		template <typename T>
		std::shared_ptr<component_array<T>> _get_component_array()
		{
			const char *type_name = typeid(T).name();
			assert(_component_types.find(type_name) != _component_types.end() && "Component not registered before use.");
			
			return std::static_pointer_cast<component_array<T>>(_component_arrays[type_name]);
		}
	};
}
