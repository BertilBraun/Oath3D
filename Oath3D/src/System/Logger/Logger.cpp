#include "Common/Common.h"
#include "Logger.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"

namespace Oath {

	namespace LOGGER {

		std::mutex Logger::m_Mutex;

		void indent(std::ostringstream& of, int s, int e) {
			for (size_t i = s; i < e; i++)
				of << " ";
		}

		void Logger::Log(const String& level, const String& file, int line, std::ostringstream& msg)
		{
			std::ostringstream of;

			of << " [ " << level;

			indent(of, (int)level.size(), 10);

			of << "] [ " << file;

			indent(of, (int)file.size(), 20);

			of << " : ( line : " << line;

			int s = 0;
			while (line /= 10) s++;

			indent(of, s, 5);

			of << ") ] : " << msg.str();

			m_Mutex.lock();

			if (!LogToFile)
#ifdef _WIN32
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
				std::cout << of.str() << std::endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)COL_WHITE);
			}
#else
				std::cout << of.str() << std::endl;
#endif 
			else 
				File::Append(Settings::LOG_FILE, of.str());

			m_Mutex.unlock();
		}

		void LogGLErrors(const String & func, const String & file, int line) {

			bool found = false;
			GLenum error, lastError = 0;

			for (int i = 0; i < 20 && (error = glGetError()) != GL_NO_ERROR && error != lastError; i++) {
				auto str = glewGetErrorString(error);
				LOGGER::Logger(false, "GLError", COL_RED, file, line, "FAILED WITH ERROR CODE [", error, "] (", str, ") IN FUNCTION :", func);
				LOGGER::Logger(true, "GLError", COL_RED, file, line, "FAILED WITH ERROR CODE [", error, "] (", str, ") IN FUNCTION :", func);
				found = true;
				lastError = error;
			}
			ASSERT(!found);
		}

		void LogALErrors(const String & func, const String & file, int line) {

			bool found = false;
			ALenum error, lastError = 0;

			for (int i = 0; i < 20 && (error = alGetError()) != AL_NO_ERROR && error != lastError; i++) {
				auto str = String(alutGetErrorString(error));
				LOGGER::Logger(false, "ALError", COL_GREEN, file, line, "FAILED WITH ERROR CODE [", error, "] (" + str + ") IN FUNCTION :", func);
				LOGGER::Logger(true, "ALError", COL_GREEN, file, line, "FAILED WITH ERROR CODE [", error, "] (" + str + ") IN FUNCTION :", func);
				found = true;
				lastError = error;
			}
			ASSERT(!found);
		}
	}
}