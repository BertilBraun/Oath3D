	#include "Common/Common.h"
#include "ToggleKey.h"

namespace Oath {
	namespace System {

		ToggleKey::ToggleKey(Key::KeyboardKey key, float TTC) :
			key(key),
			TTC(TTC)
		{ }

		bool ToggleKey::isKeyPressed() {

			if (timer.getElapsedS() > TTC)
				if (Key::isKeyHeld(key)) {
					timer.reset();
					return true;
				}

			return false;
		}

		void ToggleKey::resetTime() {
			timer.reset();
		}

		ToggleButton::ToggleButton(Key::MouseButton key, float TTC) :
			key(key),
			TTC(TTC)
		{ }

		bool ToggleButton::isKeyPressed() {

			if (timer.getElapsedS() > TTC)
				if (Key::isButtonPressed(key)) {
					timer.reset();
					return true;
				}

			return false;
		}

		void ToggleButton::resetTime() {
			timer.reset();
		}
	}
}