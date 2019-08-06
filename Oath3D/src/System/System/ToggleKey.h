#pragma once

#include "GLFW/glfw3.h"

#include "Clock.h"

namespace Oath {
	namespace System {

		class ToggleKey {
		public:
			ToggleKey(Key::KeyboardKey key, float TTC = 0.2f);

			bool isKeyPressed();
			void resetTime();

		private:
			Key::KeyboardKey key;
			Clock timer;
			float TTC;
		};

		class ToggleButton {
		public:
			ToggleButton(Key::MouseButton key, float TTC = 0.2f);

			bool isKeyPressed();
			void resetTime();

		private:
			Key::MouseButton key;
			Clock timer;
			float TTC;
		};
	}	
}