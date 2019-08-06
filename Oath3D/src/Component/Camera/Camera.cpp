#include "Common/Common.h"
#include "Camera.h"

Oath::Comp::Camera* Oath::Comp::GCam = nullptr;

Oath::Comp::Camera::Camera(bool IsMainCamera, float dist) : 
	IsMainCamera(IsMainCamera), thirdPersonDistance(dist) { }

glm::vec3 Oath::Comp::Camera::pos() {
	return entity->pos();
}
