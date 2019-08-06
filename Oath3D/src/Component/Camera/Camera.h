#pragma once

#include "System/ECS/Component.h"
#include "Frustum.h"

namespace Oath {

	namespace Comp {

		struct Camera : public ECS::Component<Camera> {

			Camera(bool IsMainCamera = false, float dist = 0);

			vec3 pos();

			vec3 forward = vec3(0, 0, -1);
			vec3 going = forward;
			vec3 right = vec3(0, 0, 0);
			vec3 up = vec3(0, 0, 0);

			mat4 view;
			mat4 proj;

			Frustum frustum;

			float thirdPersonDistance = 15.f;
			bool IsMainCamera = false;
		};
	}
}