#pragma once

#include "System/System/Clock.h"
#include "Common/Datatype/String.h"

namespace Oath {

	namespace LOGGER {

		class Stopwatch {
		public:
			Stopwatch();
			Stopwatch(int line, const String& file);
			~Stopwatch();

			float getElapsedTime();
			void Log(String prepend = "");

		private:
			System::Clock clock;
			String file;
			int line;
		};
	}
}