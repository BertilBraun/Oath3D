#pragma once

#include "System/ECS/Include.h"

namespace Oath {

	namespace Comp {

		struct Camera;
		struct Transform;
		struct FreeLook;
		struct FreeMove;

		class CameraSystem : public ECS::System {
		public:
			CameraSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& components) override;

		private:
			bool FreeLookToggle(FreeLook* freelook);

			void UpdateFreeLook(Camera* camera, FreeLook* freelook);
			void UpdateFreeMove(float delta, ECS::Entity* entity, Camera* camera, FreeMove* freemove);

		};
	}
}