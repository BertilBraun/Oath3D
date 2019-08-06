#pragma once

#include "Common/Common.h"

#include "Transform.h"
#include "Manager.h"
#include "Script.h"

namespace Oath {

	namespace ECS {
		struct BaseComponent;

		typedef uint32 IndexInData;
		typedef uint32 ComponentID;

		class Entity {
		public:
			Entity(Manager* ecs);
			~Entity();

			void add(Entity* e);
			void add(Script* script);

			inline Transform& rot(const glm::quat& rot)		{ return transform.rot(rot);		}
			inline Transform& rot(const glm::vec3& rot)		{ return transform.rot(rot);		}
			inline Transform& pos(const glm::vec3& pos)		{ return transform.pos(pos);		}
			inline Transform& scale(const glm::vec3& scale)	{ return transform.scale(scale);	}

			inline glm::quat rot()		const { return transform.rot();		}
			inline glm::vec3 pos()		const { return transform.pos();		}
			inline glm::vec3 scale()	const { return transform.scale();	}

			template<class Component>
			inline bool hasComponent();
			inline bool hasComponent(ComponentID componentID);

			template<class Component>
			inline bool removeComponent();
			template<class Component>
			inline Component* addComponent(Component& component);

			template<typename Component>
			inline Component* getComponent();
			BaseComponent* getComponentByType(ComponentID componentID);

			bool enabled = true;
			Entity* parent = nullptr;
			String tag = "";
			Vector<Entity*> childs;
			Vector<Script*> scripts;
			Vector<std::pair<ComponentID, IndexInData>> components;
			Transform transform;
		private:
			Manager* ecs;
		};

		template<class Component>
		inline bool Entity::hasComponent() {
			return hasComponent(Component::ID);
		}

		inline bool ECS::Entity::hasComponent(ComponentID componentID) {
			for (auto pair : components)
				if (pair.first == componentID)
					return true;

			return false;
		}

		template<class Component>
		inline bool Entity::removeComponent() {
			return ecs->removeComponent<Component>(this);
		}

		template<class Component>
		inline Component * Entity::addComponent(Component & component) {
			return ecs->addComponent(this, &component);
		}

		template<typename Component>
		inline Component * Entity::getComponent() {
			return(Component*)(getComponentByType(Component::ID));
		}

	}
}