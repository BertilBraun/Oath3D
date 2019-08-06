#pragma once

#include "System/ECS/Include.h"

namespace Oath {

	namespace Comp {

		class AssetListener : public ECS::Listener {
		public:
			AssetListener();

			void onRemoveEntity(ECS::Entity* handle) override;
			void onMakeEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

		};
	}
}