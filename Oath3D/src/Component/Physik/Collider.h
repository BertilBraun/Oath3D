#pragma once

#include "System/ECS/Component.h"

#include "Bullet/btBulletCollisionCommon.h"

namespace Oath {
	namespace Physic {

		struct Collider : public ECS::Component<Collider> {

			Collider(const vec3& halfExtends) {
				collider = new btBoxShape(btVector3(btScalar(halfExtends.x), btScalar(halfExtends.y), btScalar(halfExtends.z)));
			}

			Collider(float radius) {
				collider = new btSphereShape((btScalar)radius);
			}

			Collider(float radius, float height) {
				collider = new btCapsuleShape((btScalar)radius, (btScalar)height);
			}

			bool addedFromWorld = false;
			btCollisionShape* collider = nullptr;
		};
	}
}
