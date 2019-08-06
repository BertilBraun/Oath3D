#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace ECS {

		struct BaseComponent;
		class Entity;

		class System {
		public:
			System() {}

			virtual void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) = 0;

			const Vector<uint32>& getComponentTypes();
			const Vector<uint32>& getComponentFlags();

			bool isValid();
		protected:
			void addComponent(uint32 componentType, uint32 componentFlag = 0);

		private:
			Vector<uint32> componentTypes;
			Vector<uint32> componentFlags;
		};

	}
}