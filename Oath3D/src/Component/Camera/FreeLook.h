#pragma once

#include "Common/Common.h"
#include "System/ECS/Component.h"
#include "System/System/ToggleKey.h"
#include "System/Application/Application.h"

namespace Oath {

	namespace Comp {

		struct FreeLook : public ECS::Component<FreeLook> {

			FreeLook(
				vec2 mouseSpeed = vec2(0.2f), 
				Key::KeyboardKey freeLookKey = Key::F,
				Key::KeyboardKey togglekey = Key::ESCAPE)
			:
				mouseSpeed(mouseSpeed),
				freeLookKey(freeLookKey),
				toggleKey(togglekey),
				useFreeLook(true) {}

			void updateGLFW() {
				if (useFreeLook) {
					glfwSetInputMode(APP::GApp->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

					glfwSetCursorPos(APP::GApp->getWindow(),
						Settings::WIDTH / 2,
						Settings::HEIGHT / 2);
				}
				else
					glfwSetInputMode(APP::GApp->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			vec2 rot = vec2(PI / 2, 0);
			vec2 mouseSpeed;

			bool useFreeLook, freeLooking = false;
			System::ToggleKey toggleKey;
			Key::KeyboardKey freeLookKey;
		};
	}
}