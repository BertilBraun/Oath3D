#pragma once

#include "Stopwatch.h"
#include "Common/Settings.h"
#include "GL/glew.h"
#include <mutex>

#ifdef _WIN32
#include <Windows.h>
#endif 

#define COL_BLACK		  0   
#define	COL_BLUE		  1
#define	COL_GREEN		  2
#define	COL_CYAN		  3
#define	COL_RED			  4
#define	COL_MAGENTA		  5
#define	COL_BROWN		  6
#define	COL_LIGHTGRAY	  7
#define	COL_DARKGRAY	  8
#define	COL_LIGHTBLUE	  9
#define	COL_LIGHTGREEN	  10
#define	COL_LIGHTCYAN	  11
#define	COL_LIGHTRED	  12
#define	COL_LIGHTMAGENTA  13
#define	COL_YELLOW		  14
#define	COL_WHITE		  15

#define TRIMPATHBS(x)		(strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#define TRIMPATH(x)			(strrchr(x, '/') ? strrchr(x, '/') + 1 : x)

#ifdef _WIN32

#define FILENAME		TRIMPATHBS(__FILE__)

#else

#define FILENAME		TRIMPATH(__FILE__)

#endif 

#if defined(_DEBUG) || defined(_RELEASE)

#define LOGERROR(...)	  { LOGINFOCOL(false, "ERROR", COL_LIGHTRED, __VA_ARGS__); LOGINFOCOL(true, "ERROR", COL_LIGHTRED, __VA_ARGS__); __debugbreak(); }
							
#define LOGWARN(...)		LOGINFOCOL(false, "WARN", COL_YELLOW, __VA_ARGS__)
							
#define LOG(...)			LOGINFOCOL(false, "INFO", COL_CYAN, __VA_ARGS__)

#define LOGINFO(txt, ...)	LOGINFOCOL(false, txt, COL_WHITE, __VA_ARGS__)

#define LOGINFOCOL(file, txt, col, ...)		Oath::LOGGER::Logger(file, txt, col, FILENAME, __LINE__, __VA_ARGS__)

#define TIMEBLOCK									Oath::LOGGER::TimeLogger timeLogger(__LINE__, FILENAME)
#define TIMEBLOCKINFO(call)	LOGINFO("TIME", call);  Oath::LOGGER::TimeLogger timeLogger(__LINE__, FILENAME)

#else

#define LOGERROR(...)

#define LOGWARN(...)

#define LOG(...)

#define LOGINFO(txt, ...)

#define LOGINFOCOL(txt, col, ...)

#define TIMEBLOCK

#endif

namespace Oath {

	namespace LOGGER {

		class Logger {
		public:
			template<typename... Args>
			Logger(bool LogToFile, const String& level, int color, const String& file, int line, const Args&... args) :
				LogToFile(LogToFile)
#ifdef _WIN32
				, color((WORD)color)
#endif 
			{
				std::ostringstream msg;
				Log(level, file, line, msg, args...);
			}

		private:
			template<class T, typename... Args>
			void Log(const String& level, const String& file, int line, std::ostringstream& msg, T value, const Args&... args)
			{
				msg << " " << value;
				Log(level, file, line, msg, args...);
			}

			void Log(const String& level, const String& file, int line, std::ostringstream& msg);

			static std::mutex m_Mutex;

#ifdef _WIN32
			WORD color;
#endif 
			bool LogToFile = false;
		};

		void LogGLErrors(const String& func, const String& file, int line);
		void LogALErrors(const String& func, const String& file, int line);
	}
}