#pragma once

#include "System/ECS/Include.h"
#include "Billboard.h"
#include "Component/Camera/Camera.h"

namespace Oath {
	namespace Comp {

		class BillboardSystem : public ECS::System {
		public:

			BillboardSystem() {
				addComponent(Billboard::ID);
			}

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override {

				Billboard* bb = (Billboard*)entityComp[0];
				ECS::Entity* toLookAt;

				if (bb->toLookAt != nullptr)
					toLookAt = bb->toLookAt;
				else
					toLookAt = GCam->entity;

				quat lookAt = glm::normalize(glm::quat_cast(glm::lookAt(entity->transform.pos(), toLookAt->transform.pos(), vec3(0, 1, 0))));
				
				if (bb->interpolationFactor <= 0)
					entity->transform.rot(lookAt);
				else
					entity->transform.rot(QUAT::LinearInterpolation(entity->transform.rot(), lookAt, bb->interpolationFactor * delta));
			}
		};

	}
}