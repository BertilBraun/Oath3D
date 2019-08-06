#pragma once

#include "glm/gtc/quaternion.hpp"
#include "Bullet/LinearMath/btMotionState.h"

namespace Oath {
	namespace ECS {

		class Entity;

		struct Transform : public btMotionState {

			Transform(Entity* owner);

			void rotate(const quat& rot);
			void move(const vec3& val);

			Transform& rot(const quat& rot);
			Transform& rot(const vec3& rot);
			Transform& pos(const vec3& pos);
			Transform& scale(const vec3& scale);

			quat rot() const;
			vec3 pos() const;
			vec3 scale() const;

			const glm::mat4& modelMatrix();

			void getWorldTransform(btTransform& worldTrans) const override;
			void setWorldTransform(const btTransform& worldTrans) override;

		private:
			bool changed();

			Entity* owner = nullptr;

			vec3 m_scale = vec3(1);
			mat4 m_modelMatrix = glm::mat4(1);

			bool hasChanged = false;
			btTransform bulletTransform = btTransform(btQuaternion(0, 0, 0));
		};
	}
}