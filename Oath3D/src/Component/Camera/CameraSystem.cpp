#include "Common/Common.h"
#include "CameraSystem.h"

#include "System/Application/Application.h"

#include "FreeLook.h"
#include "FreeMove.h"
#include "Camera.h"
#include "Component/Physik/RigidBody.h"

namespace Oath {

	namespace Comp {

		CameraSystem::CameraSystem() {

			addComponent(Camera::ID);
			addComponent(FreeLook::ID, ECS::FLAG_OPTIONAL);
			addComponent(FreeMove::ID, ECS::FLAG_OPTIONAL);
		}

		void CameraSystem::updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& components)
		{
			Camera* camera = (Camera*)components[0];
			FreeLook* freeLook = (FreeLook*)components[1];
			FreeMove* freeMove = (FreeMove*)components[2];

			if (camera->IsMainCamera && GCam != camera)
				GCam = camera;

			if (freeLook)
				UpdateFreeLook(camera, freeLook);

			if (freeMove) 
				UpdateFreeMove(delta, entity, camera, freeMove);
		}

		bool CameraSystem::FreeLookToggle(FreeLook * freelook)
		{
			if (freelook->toggleKey.isKeyPressed()) {
				freelook->useFreeLook = !freelook->useFreeLook;

				freelook->updateGLFW();
			}

			freelook->freeLooking = false;
			if (Key::isKeyHeld(freelook->freeLookKey))
				freelook->freeLooking = true;

			return freelook->useFreeLook;
		}

		void CameraSystem::UpdateFreeLook(Camera * camera, FreeLook * freelook)
		{
			vec2& rot = freelook->rot;

			if (FreeLookToggle(freelook)) {
				double x, y;
				double width = Settings::WIDTH / 2;
				double height = Settings::HEIGHT / 2;

				glfwGetCursorPos(APP::GApp->getWindow(), &x, &y);
				glfwSetCursorPos(APP::GApp->getWindow(), width, height);

				rot.x += (float)freelook->mouseSpeed.x * (float)(width - x);
				rot.y += (float)freelook->mouseSpeed.y * (float)(height - y);

				if (rot.y > 89.9f)
					rot.y = 89.9f;

				else if (rot.y < -89.9f)
					rot.y = -89.9f;
			}

			float rx = radians(rot.x);
			float ry = radians(rot.y);

			camera->forward = normalize(vec3(cosf(ry) * sinf(rx), sinf(ry), cosf(ry) * cosf(rx)));
			camera->right = normalize(cross(camera->forward, vec3(0, 1, 0)));
			camera->up = normalize(cross(camera->right, camera->forward));

			camera->view = lookAt(camera->pos() - (camera->forward * camera->thirdPersonDistance), camera->pos() + camera->forward, camera->up);
			camera->frustum.Update(camera->proj * camera->view);

			if (!freelook->freeLooking)
				camera->going = camera->forward;
		}

		void CameraSystem::UpdateFreeMove(float delta, ECS::Entity* entity, Camera * camera, FreeMove * freemove)
		{
			auto rb = entity->getComponent<Physic::RigidBody>();

			if (!rb)
				return;

			bool wasUpdated = false;
			float speed = freemove->Speed * 100;

			if (Key::isKeyHeld(freemove->sprintKey))
				speed *= 10;

			vec3 going = camera->going * delta * speed;
			vec3 right = normalize(cross(going, vec3(0, 1, 0))) * delta * speed;
			vec3 up = normalize(cross(right, going)) * delta * speed;

			if (Key::isKeyHeld(Key::W)) {
				rb->applyForce(going);
				wasUpdated = true;
			}

			if (Key::isKeyHeld(Key::S)) {
				rb->applyForce(-going);
				wasUpdated = true;
			}

			if (Key::isKeyHeld(Key::D)) {
				rb->applyForce(right);
				wasUpdated = true;
			}

			if (Key::isKeyHeld(Key::A)) {
				rb->applyForce(-right);
				wasUpdated = true;
			}

			if (Key::isKeyHeld(Key::LEFT_CONTROL) || Key::isKeyHeld(Key::RIGHT_CONTROL)) {
				rb->applyForce(-up);
				wasUpdated = true;
			}

			if (Key::isKeyHeld(Key::SPACE)) {
				rb->applyForce(up);
				wasUpdated = true;
			}

			if (wasUpdated)
				rb->body->activate(true);
		}
	}
}