#include "Common/Common.h"
#include "CameraListener.h"

#include "FreeLook.h"
#include "FreeMove.h"
#include "Camera.h"

#include "Component/Physik/RigidBody.h"
#include "System/Application/Application.h"

namespace Oath {

	namespace Comp {

		CameraListener::CameraListener() {

			addComponent(Camera::ID);
			addComponent(FreeLook::ID, ECS::FLAG_OPTIONAL);
			addComponent(FreeMove::ID, ECS::FLAG_OPTIONAL);

		}

		void CameraListener::onRemoveEntity(ECS::Entity* handle) {

			if (handle->getComponent<Camera>()->IsMainCamera)
				Comp::GCam = nullptr;
			
			if (handle->hasComponent<FreeLook>())
				glfwSetInputMode(APP::GApp->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void CameraListener::onMakeEntity(ECS::Entity* handle) {

			Camera * cam = handle->getComponent<Camera>();
			FreeLook * freelook = handle->getComponent<FreeLook>();

			if (Comp::GCam != nullptr && cam->IsMainCamera)
				LOGWARN("MAIN CAMERA WAS ALLREADY DEFINED !!");

			if (cam->proj == mat4())
				cam->proj = (perspective(Settings::FOV, Settings::WIDTH / Settings::HEIGHT, Settings::ZNEAR, Settings::ZFAR));

			if (freelook) 
				freelook->updateGLFW();
			
			if (handle->hasComponent<FreeMove>())
				addRigidBody(handle);
		}

		void CameraListener::onAddComponent(ECS::Entity * handle, uint32 id)
		{
			if (id == Camera::ID)
				onMakeEntity(handle);

			else if (id == FreeLook::ID) 
				handle->getComponent<FreeLook>()->updateGLFW();

			else if (id == FreeMove::ID)
				addRigidBody(handle);
		}

		void CameraListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
		{
			if (id == Camera::ID)
				onRemoveEntity(handle);

			else if (id == FreeLook::ID)
				glfwSetInputMode(APP::GApp->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void CameraListener::addRigidBody(ECS::Entity * entity)
		{
			Physic::RigidBody* rb;

			if (!entity->hasComponent<Physic::RigidBody>()) {

				Physic::RigidBody body(1);
				rb = entity->addComponent<Physic::RigidBody>(body);
			}
			else
				rb = entity->getComponent<Physic::RigidBody>();

			if (rb && rb->body) {

				rb->body->setFlags(rb->body->getFlags()		| 
					btCollisionObject::CF_KINEMATIC_OBJECT	|
					btCollisionObject::CF_CHARACTER_OBJECT);

				if (entity->getComponent<FreeMove>()->useGravity)
					rb->body->setGravity(rb->body->getGravity() * 10);
				else
					rb->body->setGravity(btVector3(0, 0, 0));

				rb->body->setDamping(0.95f, -1.f);
				rb->body->setActivationState(DISABLE_DEACTIVATION);
			}
			else
				LOGWARN("FAILED TO ADD RIGIDBODY TO ENTITY");
		}
	}
}