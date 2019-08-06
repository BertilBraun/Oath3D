#pragma once

#include "Common/Common.h"

#include <tuple>
#include <mutex>

#include "Entity.h"

namespace Oath {

	namespace ECS {

		struct BaseComponent;

		typedef uint32(*CreateComponentFunc)(Vector<uint8>& memory, Entity* entity, BaseComponent* comp, bool& hasDelPos);
		typedef void(*FreeComponentFunc)(BaseComponent* comp);

		struct BaseComponent {

			static ComponentID RegisterComponent(CreateComponentFunc createfn, FreeComponentFunc freefn, size_t size);
			Entity* entity = nullptr;
			std::mutex* mutex = nullptr;

			inline static CreateComponentFunc getTypeCreateFunction(ComponentID id)
			{
				return std::get<0>((*componentTypes)[id]);
			}

			inline static FreeComponentFunc getTypeFreeFunction(ComponentID id)
			{
				return std::get<1>((*componentTypes)[id]);
			}

			inline static size_t getTypeSize(ComponentID id)
			{
				return std::get<2>((*componentTypes)[id]);
			}

			inline static bool isTypeValid(ComponentID id)
			{
				return id < componentTypes->size();
			}
		private:
			static Vector<std::tuple<CreateComponentFunc, FreeComponentFunc, size_t>>* componentTypes;
		};

		template<typename T>
		struct Component : public BaseComponent
		{
			static const CreateComponentFunc CREATE_FUNCTION;
			static const FreeComponentFunc FREE_FUNCTION;
			static const ComponentID ID;
			static const size_t SIZE;
		};

		template<typename Component>
		uint32 CreateComponent(Vector<uint8>& memory, Entity* entity, BaseComponent* comp, bool& hasDelPos)
		{
			uint32 index = (uint32)-1;
			uint32 size = (uint32)memory.size();

			if (hasDelPos)
				for (uint32 i = 0; i < size; i += (uint32)Component::SIZE)
					if ((Component*)memory[i] == nullptr) {
						index = i;
						break;
					}

			if (index == (uint32)-1) {
				hasDelPos = false;

				if (size < 16 * Component::SIZE)
					memory.resize(size + 16 * Component::SIZE);
				else
					memory.resize(size + 128 * Component::SIZE);

				index = size;
			}

			Component* component = new(&memory[index])Component(*(Component*)comp);

			component->entity = entity;
			component->mutex = new std::mutex();

			return index;
		}

		template<typename Component>
		void FreeComponent(BaseComponent* comp)
		{
			SAVE_DELETE(comp->mutex)
			((Component*)comp)->~Component();
		}

		template<typename T>
		const ComponentID Component<T>::ID(BaseComponent::RegisterComponent(CreateComponent<T>, FreeComponent<T>, sizeof(T)));

		template<typename T>
		const size_t Component<T>::SIZE(sizeof(T));

		template<typename T>
		const CreateComponentFunc Component<T>::CREATE_FUNCTION(CreateComponent<T>);

		template<typename T>
		const FreeComponentFunc Component<T>::FREE_FUNCTION(FreeComponent<T>);
	}
}