#include "Common/Common.h"
#include "Manager.h"

#include "System.h"
#include "Entity.h"
#include "Script.h"
#include "Component.h"

namespace Oath {

	namespace ECS {

		Manager::~Manager()
		{
			for (auto it = components.begin(); it != components.end(); ++it) {
				uint32 typeSize = (uint32)BaseComponent::getTypeSize(it->first);
				FreeComponentFunc freefn = BaseComponent::getTypeFreeFunction(it->first);

				for (uint32 i = 0; i < (uint32)it->second.first.size(); i += typeSize)
					if (((BaseComponent*)&it->second.first[i])->entity)
						freefn((BaseComponent*)&it->second.first[i]);
			}

			for (auto entity : entities)
				SAVE_DELETE(entity);
		}

		void Manager::addListener(Listener * listener)
		{
			listeners.push_back(listener);
		}

		Entity * Manager::makeEntityInternal(Vector<BaseComponent*>& components, Vector<uint32>& componentIDs, size_t & numComponents) {
			return createEntityInternal(components.data(), componentIDs.data(), numComponents);
		}

		Entity* Manager::createEntityInternal(BaseComponent** entityComponents, const uint32* componentIDs, size_t numComponents)
		{
			Entity* newEntity = new Entity(this);

			for (uint32 i = 0; i < numComponents; i++) {

				if (!BaseComponent::isTypeValid(componentIDs[i])) {
					LOG(componentIDs[i], "is not a valid component type.");
					SAVE_DELETE(newEntity);
					return nullptr;
				}

				addComponentInternal(newEntity, componentIDs[i], entityComponents[i]);
			}

			entities.push_back(newEntity);

			for (Listener* listener : listeners) {

				bool isValid = true;

				const Vector<uint32>& listenerComponents = listener->getComponentIDs();
				const Vector<uint32>& componentFlags = listener->getComponentFlags();

				for (uint32 j = 0; j < listenerComponents.size(); j++) {

					bool hasComponent = false;

					for (uint32 k = 0; k < numComponents; k++)
						if (listenerComponents[j] == componentIDs[k] || componentFlags[j] == FLAGS::FLAG_OPTIONAL) {
							hasComponent = true;
							break;
						}

					if (!hasComponent) {
						isValid = false;
						break;
					}
				}
				if (isValid)
					listener->onMakeEntity(newEntity);
				
			}

			return newEntity;
		}

		void Manager::removeEntity(Entity* entity)
		{
			if (entity == nullptr)
				return;

			Vector<std::pair<uint32, uint32>>& Components = entity->components;

			for (Listener* listener : listeners) {

				bool isValid = true;

				const Vector<uint32>& componentIDs = listener->getComponentIDs();
				const Vector<uint32>& componentFlags = listener->getComponentFlags();

				for (uint32 j = 0; j < componentIDs.size(); j++) {

					bool hasComponent = false;
					for (uint32 k = 0; k < Components.size(); k++) 
						if (componentIDs[j] == Components[k].first || componentFlags[j] == FLAGS::FLAG_OPTIONAL) {
							hasComponent = true;
							break;
						}
						
					if (!hasComponent) {
						isValid = false;
						break;
					}
				}
				if (isValid)
					listener->onRemoveEntity(entity);
				
			}

			for (auto comps : entity->components)
				memset(&components[comps.first].first[comps.second], 0, BaseComponent::getTypeSize(comps.first));			

			entities.erase(std::find(entities.begin(), entities.end(), entity));
			SAVE_DELETE(entity);
		}

		inline void Manager::deleteComponent(uint32 componentID, uint32 Index)
		{
			FreeComponentFunc freefn = BaseComponent::getTypeFreeFunction(componentID);
			components[componentID].second = true;

			BaseComponent* destComponent = (BaseComponent*)&components[componentID].first[Index];
			freefn(destComponent);
		}

		inline bool Manager::removeComponentInternal(Entity * entity, uint32 componentID)
		{
			if (entity == nullptr)
				return false;

			Vector<std::pair<uint32, uint32> >& entityComponents = entity->components;

			for (uint32 i = 0; i < entityComponents.size(); i++)
				if (componentID == entityComponents[i].first) {
					deleteComponent(entityComponents[i].first, entityComponents[i].second);
					entityComponents.swapRemove(i);
					return true;
				}

			return false;
		}

		inline BaseComponent* Manager::addComponentInternal(Entity* entity, uint32 componentID, BaseComponent* component)
		{
			for (auto pair : entity->components)
				if (pair.first == componentID) {
					LOGERROR("COMPONENT WAS ALLREADY ADDED TO THIS ENTITY :", componentID);
					return nullptr;
				}

			CreateComponentFunc createfn = BaseComponent::getTypeCreateFunction(componentID);
			entity->components.push_back({ componentID, createfn(components[componentID].first, entity, component, components[componentID].second) });
		
			return (BaseComponent*)&components[componentID].first[entity->components.back().second];
		}

		BaseComponent * Manager::getComponentByType(Entity* entity, uint32 componentID)
		{
			if (entity == nullptr)
				return nullptr;

			return getComponentInternal(entity->components, components[componentID].first, componentID);
		}

		Entity * Manager::getEntity(const String & tag)
		{
			for (Entity* e : entities)
				if (e->tag == tag)
					return e;

			return nullptr;
		}

		Vector<Entity*> Manager::getEntities(const String & tag)
		{
			Vector<Entity*> out;

			for (Entity* e : entities)
				if (e->tag == tag)
					out.emplace_back(e);

			return out;
		}

		inline BaseComponent* Manager::getComponentInternal(Vector<std::pair<uint32,uint32>>& entity, Vector<uint8>& array, uint32 componentID)
		{
			for (uint32 i = 0; i < entity.size(); i++)
				if (componentID == entity[i].first)
					return (BaseComponent*)&array[entity[i].second];

			return nullptr;
		}

		void Manager::updateSystems(SystemList& systems, float delta)
		{
			for (System* system : systems.Systems())
				updateSystem(system, delta);
		}

		void Manager::updateSystemsMT(SystemList & systems, float delta)
		{
			Vector<std::thread> threads;

			for (System* system : systems.Systems())
				threads.emplace_back([&] { updateSystem(system, delta); });

			for (auto& t : threads)
				t.join();
		}

		void Manager::ScriptUpdate(float delta) {
			for (Entity* e : entities)
				for (Script* s : e->scripts) {
					try
					{
						s->Update(delta);
					}
					catch (const std::exception& e)
					{
						LOGERROR(e.what());
					}
				}
		}

		void Manager::ScriptRender(float delta) {
			for (Entity* e : entities)
				for (Script* s : e->scripts) {
					try
					{
						s->Render(delta);
					}
					catch (const std::exception& e)
					{
						LOGERROR(e.what());
					}
				}
		}

		inline uint32 Manager::findLeastCommonComponent(const Vector<uint32>& componentTypes, const Vector<uint32>& componentFlags)
		{
			uint32 minSize = (uint32)-1;
			uint32 minIndex = (uint32)-1;
			for (uint32 i = 0; i < componentTypes.size(); i++) {
				if ((componentFlags[i] & FLAGS::FLAG_OPTIONAL) != 0)
					continue;

				size_t typeSize = BaseComponent::getTypeSize(componentTypes[i]);
				uint32 size = (uint32)(components[componentTypes[i]].first.size() / typeSize);
				if (size <= minSize) {
					minSize = size;
					minIndex = i;
				}
			}

			return minIndex;
		}

		inline void Manager::updateSystem(System * sys, float delta)
		{
			const Vector<uint32>& componentTypes = sys->getComponentTypes();

			if (componentTypes.size() == 1) {
				uint32 typeSize = (uint32)BaseComponent::getTypeSize(componentTypes[0]);
				Vector<uint8>& array = components[componentTypes[0]].first;

				for (uint32 j = 0; j < (uint32)array.size(); j += typeSize) {

					BaseComponent* component = (BaseComponent*)&array[j];
					
					if (!component->entity || !component->entity->enabled)
						continue;

					component->mutex->lock();

					try
					{
						sys->updateComponents(delta, component->entity, { component });
					}
					catch (const std::exception& e)
					{
						LOGERROR("Updating System threw exception", e.what());
					}

					component->mutex->unlock();
				}
			}
			else
				updateSystemWithMultipleComponents(sys, delta, componentTypes);
		}

		inline void Manager::updateSystemWithMultipleComponents(System* system, float delta, const Vector<uint32>& componentTypes)
		{
			static Vector<BaseComponent*> componentParam;
			static Vector<Vector<uint8>*> componentVectors;

			const Vector<uint32>& componentFlags = system->getComponentFlags();

			componentParam.resize(std::max(componentParam.size(), componentTypes.size()));
			componentVectors.resize(std::max(componentVectors.size(), componentTypes.size()));

			for (uint32 i = 0; i < componentTypes.size(); i++)
				componentVectors[i] = &components[componentTypes[i]].first;

			uint32 minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

			uint32 typeSize = (uint32)BaseComponent::getTypeSize(componentTypes[minSizeIndex]);
			Vector<uint8>& array = *componentVectors[minSizeIndex];

			for (uint32 i = 0; i < (uint32)array.size(); i += typeSize) {

				componentParam[minSizeIndex] = (BaseComponent*)&array[i];

				if (componentParam[minSizeIndex]->entity == nullptr || !componentParam[minSizeIndex]->entity->enabled)
					continue;

				Vector<std::pair<uint32, uint32>>& entityComponents = componentParam[minSizeIndex]->entity->components;

				bool isValid = true;
				for (uint32 j = 0; j < componentTypes.size(); j++) {
					if (j == minSizeIndex)
						continue;

					componentParam[j] = getComponentInternal(entityComponents, *componentVectors[j], componentTypes[j]);
					if (componentParam[j] == nullptr && (componentFlags[j] & FLAGS::FLAG_OPTIONAL) == 0) {
						isValid = false;
						break;
					}
				}

				if (isValid) {
					for (uint32 j = 0; j < componentTypes.size(); j++)
						if (componentParam[j] != nullptr)
							componentParam[j]->mutex->lock();

					try
					{
						system->updateComponents(delta, componentParam[0]->entity, componentParam);
					}
					catch (const std::exception& e)
					{
						LOGERROR("Updating System threw exception", e.what());
					}

					for (uint32 j = 0; j < componentTypes.size(); j++)
						if (componentParam[j] != nullptr)
							componentParam[j]->mutex->unlock();
				}
			}
		}
	}
}