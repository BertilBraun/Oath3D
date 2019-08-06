#pragma once

#include "System/ECS/Include.h"

namespace Oath {
	namespace Comp {

		class EmitterSystem : public ECS::System {
		public:
			EmitterSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override;
		};

		class EmitterListener : public ECS::Listener {
		public:
			EmitterListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

			void remove(ECS::Entity* entity);
			void add(ECS::Entity* entity);
		};

		class RecieverSystem : public ECS::System {
		public:
			RecieverSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override;
		};

		class RecieverListener : public ECS::Listener {
		public:
			RecieverListener();
			bool added = false;

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

			void remove(ECS::Entity* entity);
			void add(ECS::Entity* entity);
		};
	}
}