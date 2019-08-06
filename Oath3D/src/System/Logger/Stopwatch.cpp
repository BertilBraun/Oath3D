#include "Common/Common.h"
#include "Stopwatch.h"

#include "Logger.h"

namespace Oath {

	namespace LOGGER {

		Stopwatch::Stopwatch() :
			line(0), file("")
		{ }

		Stopwatch::Stopwatch(int line, const String & file) :
			line(line), file(file)
		{ }

		Stopwatch::~Stopwatch()
		{
			Log("Total");
		}

		float Stopwatch::getElapsedTime()
		{
			return (float)clock.getElapsedS();
		}

		void Stopwatch::Log(String prepend)
		{
			Logger(false, "TIME", COL_LIGHTGREEN, file, line, prepend + " Updating took : ", clock.resetYS(), "micro sec to finish.");
		}
	}
}