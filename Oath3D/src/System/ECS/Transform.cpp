#include "Common/Common.h"
#include "Transform.h"

#include "Entity.h"
#include "Component/Physik/RigidBody.h"

namespace Oath {

	namespace ECS {

		Transform::Transform(ECS::Entity* owner) : owner(owner) { }

		void Transform::rotate(const quat & rotIn)
		{
			bulletTransform.setRotation(btQuaternion(rotIn.x, rotIn.y, rotIn.z, rotIn.w) * bulletTransform.getRotation());
			hasChanged = true;

			auto rb = owner->getComponent<Physic::RigidBody>();

			if (rb)
				rb->body->setWorldTransform(bulletTransform);
		}

		void Transform::move(const vec3 & posIn)
		{
			bulletTransform.setOrigin(bulletTransform.getOrigin() + btVector3(posIn.x, posIn.y, posIn.z));
			hasChanged = true;

			auto rb = owner->getComponent<Physic::RigidBody>();

			if (rb)
				rb->body->setWorldTransform(bulletTransform);
		}

		Transform& Transform::rot(const quat & rotIn)
		{
			bulletTransform.setRotation({ rotIn.x, rotIn.y, rotIn.z, rotIn.w });
			hasChanged = true;

			auto rb = owner->getComponent<Physic::RigidBody>();
			
			if (rb)
				rb->body->setWorldTransform(bulletTransform);

			return *this;
		}

		Transform & Transform::rot(const vec3 & p_rot)
		{
			return rot(quat(radians(p_rot)));
		}

		Transform& Transform::pos(const vec3 & posIn)
		{
			bulletTransform.setOrigin({ posIn.x, posIn.y, posIn.z });
			hasChanged = true;

			auto rb = owner->getComponent<Physic::RigidBody>();

			if (rb)
				rb->body->setWorldTransform(bulletTransform);
			
			return *this;
		}

		Transform& Transform::scale(const vec3 & scaleIn)
		{
			m_scale = scaleIn;
			hasChanged = true;

			return *this;
		}

		quat Transform::rot() const
		{
			const btQuaternion& bulletRot = bulletTransform.getRotation();
			quat rot = quat(bulletRot.x(), bulletRot.y(), bulletRot.z(), bulletRot.w());

			if (!owner->parent)
				return rot;

			return owner->parent->transform.rot() * rot;
		}

		vec3 Transform::pos() const
		{
			const btVector3& bulletPos = bulletTransform.getOrigin();
			vec3 pos(bulletPos.x(), bulletPos.y(), bulletPos.z());

			if (!owner->parent)
				return pos;

			return vec3(owner->parent->transform.modelMatrix() * vec4(pos, 1));
		}

		vec3 Transform::scale() const
		{
			if (!owner->parent)
				return m_scale;

			return m_scale * owner->parent->transform.scale();
		}

		const mat4& Transform::modelMatrix() {

			if (!changed())
				return m_modelMatrix;

			btVector3 bulletPos = bulletTransform.getOrigin();
			vec3 pos(bulletPos.x(), bulletPos.y(), bulletPos.z());

			btQuaternion bulletRot = bulletTransform.getRotation();
			quat rot(bulletRot.x(), bulletRot.y(), bulletRot.z(), bulletRot.w());

			m_modelMatrix = glm::translate(pos) * glm::toMat4(rot) * glm::scale(m_scale);

			if (owner->parent)
				m_modelMatrix = owner->parent->transform.modelMatrix() * m_modelMatrix;

			return m_modelMatrix;
		}

		void Transform::getWorldTransform(btTransform & worldTrans) const
		{
			worldTrans = bulletTransform;
		}

		void Transform::setWorldTransform(const btTransform & worldTrans)
		{
			bulletTransform = worldTrans;
			hasChanged = true;
		}

		bool Transform::changed()
		{
			if (!owner->parent)
				return hasChanged;

			return hasChanged || owner->parent->transform.changed();
		}
	}
}