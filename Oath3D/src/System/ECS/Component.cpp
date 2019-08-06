#include "Common/Common.h"
#include "Component.h"

namespace Oath {

	namespace ECS {

		Vector<std::tuple<CreateComponentFunc, FreeComponentFunc, size_t>>* BaseComponent::componentTypes;

		uint32 BaseComponent::RegisterComponent(CreateComponentFunc createfn,
			FreeComponentFunc freefn, size_t size)
		{
			if (componentTypes == nullptr)
				componentTypes = new Vector<std::tuple<CreateComponentFunc, FreeComponentFunc, size_t>>();

			componentTypes->push_back({ createfn, freefn, size });
			return (uint32)(componentTypes->size() - 1);
		}
	}
}