#include "Common/Common.h"
#include "RigidBody.h"

namespace Oath {
	namespace Physic {

		RigidBody::RigidBody(float massIn) : info(massIn, nullptr, nullptr) { }

		void RigidBody::applyForce(const vec3 & val, const vec3 & org) {
			ASSERT(body);
			body->applyForce({ val.x, val.y, val.z }, { org.x, org.y, org.z });
		}
	}
}