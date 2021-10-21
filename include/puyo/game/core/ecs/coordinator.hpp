#pragma once

#include <memory>

#include "component_manager.hpp"
#include "entity_manager.hpp"

namespace puyo
{
	class coordinator final
	{
	public:
		coordinator()
		{
			_component_manager = std::make_unique<component_manager>();
			_entity_manager = std::make_unique<entity_manager>();
		}

		~coordinator() = default;

		coordinator(const coordinator &) = delete;
		coordinator &operator=(const coordinator &) = delete;

		coordinator(coordinator &&) = delete;
		coordinator &operator=(coordinator &&) = delete;

		[[nodiscard]] entity create_entity()
		{
			return _entity_manager->create_entity();
		}

		void destroy_entity(entity id)
		{
			_entity_manager->destroy_entity(id);
			_component_manager->entity_destroyed(id);
		}

		template <typename T>
		void register_component()
		{
			_component_manager->register_component<T>();
		}

		template <typename T>
		void add_component(entity id, T component)
		{
			_component_manager->add_component<T>(id, component);

			auto sig = _entity_manager->get_signature(id);
			sig.set(_component_manager->get_component_type<T>(), true);
			_entity_manager->set_signature(id, sig);
		}

		template <typename T>
		void remove_component(entity id)
		{
			_component_manager->remove_component<T>(id);

			auto sig = _entity_manager->get_signature(id);
			sig.set(_component_manager->get_component_type<T>(), false);
			_entity_manager->set_signature(id, sig);
		}

		template <typename T>
		[[nodiscard]] T &get_component(entity id)
		{
			return _component_manager->get_component<T>(id);
		}

	private:
		std::unique_ptr<component_manager> _component_manager;
		std::unique_ptr<entity_manager> _entity_manager;
	};
}
