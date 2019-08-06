#pragma once

#include "Lights.h"
#include "System/ECS/Include.h"
#include "Common/Common.h"

namespace Oath {
	namespace Render {

		struct AmbientLightListener : ECS::Listener
		{
			AmbientLightListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;
		};

		struct DirectionalLightListener : ECS::Listener
		{
			DirectionalLightListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;
		};

		struct PointLightListener : ECS::Listener
		{
			PointLightListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;
		};

		struct SpotLightListener : ECS::Listener
		{
			SpotLightListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;
		};

		struct PointLightSystem : ECS::System
		{
			PointLightSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override;
		};

		struct SpotLightSystem : ECS::System
		{
			SpotLightSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override;
		};
	}
}