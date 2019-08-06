#pragma once

#include <GLFW/glfw3.h>
#include "Common/Common.h"

#include "AL/al.h"
#include "AL/alc.h"

namespace Oath {

	namespace WINDOW {

		class Window {
		public:
			Window(const String& Name, float width, float height);
			~Window();

			int getWidth();
			int getHeight();

			GLFWwindow* window = nullptr;
		private:
			ALCdevice* device;
			ALCcontext* context;
		};
	}
}