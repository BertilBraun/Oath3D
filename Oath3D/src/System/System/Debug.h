#pragma once

#ifdef _WIN32

#include "windows.h"
#include "psapi.h"

#include "Common/Common.h"

namespace Oath {
	namespace System {

		class Debug {
		public:
			static void render(float delta) { static Debug d; d.draw(delta); }

		private:
			Debug();
			void draw(float delta);
			void getCurrentValues();

			std::list<float> CPUusage, virtualMemUsed, physMemUsed, virtualMemUsedByMe, physMemUsedByMe;
			float totalVirtualMem, totalPhysMem;

			ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
			int numProcessors;
			HANDLE self;
		};
	}
}

#endif
