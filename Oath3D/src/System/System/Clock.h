#pragma once

#include "GLFW/glfw3.h"

namespace Oath {
	namespace System {

		class Clock {
		public:
			Clock() {
				start = glfwGetTime();
			}

			double getElapsedS() {
				return glfwGetTime() - start;
			}

			double getElapsedMS() {
				return getElapsedS() * 1000;
			}

			double getElapsedYS() {
				return getElapsedMS() * 1000;
			}

			void reset() {
				start = glfwGetTime();
			}

			double resetS() {

				double time = glfwGetTime() - start;
				reset();
				return time;
			}

			double resetMS() {
				return resetS() * 1000;
			}

			double resetYS() {
				return resetMS() * 1000;
			}

		private:
			double start;
		};
	}

}