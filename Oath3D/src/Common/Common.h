#pragma once

#include <math.h>
#include <fstream>
#include <stdexcept>

#include <map>
#include <set>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "GL/glew.h"

#include "System/Logger/Logger.h"

#include "Datatype/Vector.h"
#include "Datatype/String.h"

#include "Datatype/glm.h"
#include "Datatype/ai.h"

#include "Settings.h"

#include "MathDef.h"
#include "Util.h"

#include "System/System/Keyboard.h"
#include "System/System/FileSystem.h"

namespace Oath {
	namespace APP {
		class Application;

		extern Application* GApp;
	}
	namespace Render {
		class MasterRenderer;

		extern MasterRenderer* GRenderer;
	}
	namespace Comp {
		struct Camera;

		extern Camera* GCam;
	}
}

#define BITSET(var, val)	((var) & BIT(val))
#define SETBIT(var, val)	(var |= BIT(val))
#define BIT(val)			(1 << val)

#define SAVE_DELETE(x)	{ if (x) { delete x; x = nullptr; } } 

#define ELEMENTS_IN_ARRAY(x) (sizeof(x)/sizeof(x[0]))

#if defined(_DEBUG) || defined(_RELEASE)

#define ASSERT(x)	if (!(x)) { LOGINFOCOL(false, "ASSERT", COL_RED, #x); __debugbreak(); }

#define GLCall(x)	{\
						for (int NoOneShouldEverComeToUseSuchANameGL = 0; NoOneShouldEverComeToUseSuchANameGL < 20 && glGetError() != GL_NO_ERROR; NoOneShouldEverComeToUseSuchANameGL++);\
						(x);\
						LOGGER::LogGLErrors(#x, FILENAME, __LINE__);\
					}

#define ALCall(x)	{\
						for (int NoOneShouldEverComeToUseSuchANameAL = 0; NoOneShouldEverComeToUseSuchANameAL < 20 && alGetError() != AL_NO_ERROR; NoOneShouldEverComeToUseSuchANameAL++);\
						(x);\
						LOGGER::LogALErrors(#x, FILENAME, __LINE__);\
					}

#define ONLY_DEBUG(x)	{ x; }

#else

#define ASSERT(x)

#define GLCall(x)	x;
#define ALCall(x)	x;

#define ONLY_DEBUG(x)

#endif