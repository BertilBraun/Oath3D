#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace ECS {
		class Entity;

		class Listener {
		public:
			virtual void onMakeEntity(ECS::Entity* handle) {}
			virtual void onRemoveEntity(ECS::Entity* handle) {}
			virtual void onAddComponent(ECS::Entity* handle, uint32 id) {}
			virtual void onRemoveComponent(ECS::Entity* handle, uint32 id) {}

			const Vector<uint32>& getComponentIDs();
			const Vector<uint32>& getComponentFlags();

		protected:
			void addComponent(uint32 id, uint32 flag = 0);

		private:
			Vector<uint32> componentIDs;
			Vector<uint32> componentFlags;
		};

	}
}