#pragma once

#include "Common/Common.h"

#include "SystemList.h"
#include "Listener.h"
#include "FLAGS.h"

namespace Oath {

	namespace ECS {

		struct BaseComponent;
		class Entity;
		class System;

		class Manager {
		public:
			Manager() {}
			~Manager();

			void addListener(Listener* listener);
			void removeEntity(Entity* handle);

			template<typename ...Args>
			inline Entity* makeEntity(Args&... args) {
				Vector<BaseComponent*> basecomponents;
				Vector<uint32> componentIDs;
				size_t numComponents = 0;

				return makeEntityInternal(basecomponents, componentIDs, numComponents, args...);
			}

			template<class Component>
			inline bool removeComponent(Entity* entity)
			{
				if (entity == nullptr)
					return;

				for (uint32 i = 0; i < listeners.size(); i++) {

					const Vector<uint32>& componentIDs = listeners[i]->getComponentIDs();
					const Vector<uint32>& componentFlags = listeners[i]->getComponentFlags();

					for (uint32 j = 0; j < componentIDs.size(); j++)
						if (componentIDs[j] == Component::ID || componentFlags[j] == FLAGS::FLAG_OPTIONAL) {
							listeners[i]->onRemoveComponent(entity, Component::ID);
							break;
						}
				}
				return removeComponentInternal(entity, Component::ID);
			}

			template<class Component>
			inline Component* addComponent(Entity* entity, Component* component)
			{
				if (entity == nullptr || component == nullptr)
					return nullptr;

				Component* instance = (Component*)addComponentInternal(entity, Component::ID, component);

				for (uint32 i = 0; i < listeners.size(); i++) {

					const Vector<uint32>& componentIDs = listeners[i]->getComponentIDs();
					const Vector<uint32>& componentFlags = listeners[i]->getComponentFlags();

					for (uint32 j = 0; j < componentIDs.size(); j++)
						if (componentIDs[j] == Component::ID || componentFlags[j] == FLAGS::FLAG_OPTIONAL) {
							listeners[i]->onAddComponent(entity, Component::ID);
							break;
						}
				}

				return instance;
			}

			template<typename Component>
			Component* getComponent(Entity* entity) {
				return(Component*)(getComponentByType(entity, Component::ID));
			}

			BaseComponent* getComponentByType(Entity* entity, uint32 componentID);

			Entity* getEntity(const String& tag);
			Vector<Entity*> getEntities(const String& tag);

			void updateSystems(SystemList& systems, float delta);
			void updateSystemsMT(SystemList& systems, float delta);

			void ScriptUpdate(float delta);
			void ScriptRender(float delta);

		private:
			std::map<uint32, std::pair<Vector<uint8>, bool>> components;
			Vector<Listener*> listeners;
			Vector<Entity*> entities;

			inline void deleteComponent(uint32 componentID, uint32 Index);
			inline bool removeComponentInternal(Entity* entity, uint32 componentID);
			inline BaseComponent* addComponentInternal(Entity* entity, uint32 componentID, BaseComponent* component);
			inline BaseComponent* getComponentInternal(Vector<std::pair<uint32, uint32> >& entityComponents, Vector<uint8>& array, uint32 componentID);

			inline void updateSystem(System* sys, float delta);
			inline void updateSystemWithMultipleComponents(System* system, float delta, const Vector<uint32>& componentTypes);
			inline uint32 findLeastCommonComponent(const Vector<uint32>& componentTypes, const Vector<uint32>& componentFlags);

			Entity* makeEntityInternal(Vector<BaseComponent*>& components, Vector<uint32>& componentIDs, size_t& numComponents);

			template<typename T, typename ...Args>
			inline Entity* makeEntityInternal(Vector<BaseComponent*>& components, Vector<uint32>& componentIDs, size_t& numComponents,
				T component, Args&... args) {
				components.push_back((BaseComponent*)&component);
				componentIDs.push_back(T::ID);
				numComponents++;
				return makeEntityInternal(components, componentIDs, numComponents, args...);
			}

			inline Entity* createEntityInternal(BaseComponent** components, const uint32* componentIDs, size_t numComponents);
		};
	}
}