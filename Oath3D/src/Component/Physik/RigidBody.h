#pragma once

#include "System/ECS/Component.h"

#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"

namespace Oath {
	namespace Physic {

		struct RigidBody : public ECS::Component<RigidBody> {

			RigidBody(float massIn);

			void applyForce(const vec3& val, const vec3& org = vec3(0));

			bool addedFromWorld = false;
			btRigidBody::btRigidBodyConstructionInfo info;
			btRigidBody* body = nullptr;
		};
	}
}
