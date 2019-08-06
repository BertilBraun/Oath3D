#include "Common/Common.h"
#include "Debug.h"

#include "ImGui/Include.h"

namespace Oath {
	namespace System {

		void plot(const String& n, const std::list<float>& d, float min, float max) {

			Vector<float> data;
			for (float c : d)
				data.emplace_back(c);

			ImGui::PlotLines(n.c_str(), data.data(), (int)data.size(), 0, NULL, min, max, ImVec2(200, 100));
		}

		Debug::Debug() {
			SYSTEM_INFO sysInfo;
			FILETIME ftime, fsys, fuser;

			GetSystemInfo(&sysInfo);
			numProcessors = sysInfo.dwNumberOfProcessors;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&lastCPU, &ftime, sizeof(FILETIME));

			self = GetCurrentProcess();
			GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
			memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));


			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			virtualMemUsedByMe.emplace_back((float)pmc.PagefileUsage);

			totalVirtualMem = (float)memInfo.ullTotalPageFile;
			totalPhysMem = (float)memInfo.ullTotalPhys;

			getCurrentValues();
		}

		void Debug::getCurrentValues() {
			FILETIME ftime, fsys, fuser;
			ULARGE_INTEGER now, sys, user;
			float percent;

			GetSystemTimeAsFileTime(&ftime);
			memcpy(&now, &ftime, sizeof(FILETIME));

			GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
			memcpy(&sys, &fsys, sizeof(FILETIME));
			memcpy(&user, &fuser, sizeof(FILETIME));
			percent = (float)(sys.QuadPart - lastSysCPU.QuadPart) + (float)(user.QuadPart - lastUserCPU.QuadPart);
			percent /= (now.QuadPart - lastCPU.QuadPart);
			percent /= numProcessors;
			lastCPU = now;
			lastUserCPU = user;
			lastSysCPU = sys;

			CPUusage.emplace_back(percent * 100);

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);

			virtualMemUsed.emplace_back((float)memInfo.ullTotalPageFile - memInfo.ullAvailPageFile);

			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			virtualMemUsedByMe.emplace_back((float)pmc.PagefileUsage);

			physMemUsed.emplace_back((float)memInfo.ullTotalPhys - memInfo.ullAvailPhys);
			physMemUsedByMe.emplace_back((float)pmc.WorkingSetSize);

			int popRate = 50;

			while (virtualMemUsed.size() > popRate)
				virtualMemUsed.pop_front();

			while (physMemUsed.size() > popRate)
				physMemUsed.pop_front();

			while (virtualMemUsedByMe.size() > popRate)
				virtualMemUsedByMe.pop_front();

			while (physMemUsedByMe.size() > popRate)
				physMemUsedByMe.pop_front();

			while (CPUusage.size() > popRate)
				CPUusage.pop_front();
		}

		void Debug::draw(float delta) {
			static float elapsed = 0;
			elapsed += delta;

			if (elapsed > 0.15f) {
				elapsed -= 0.15f;
				getCurrentValues();
			}

			ImGui::Begin("Debug Information");

			plot("", CPUusage, 0, 100);
			ImGui::SameLine();
			ImGui::Text("CPU usage: %.0f %%", *std::next(CPUusage.begin(), CPUusage.size() - 1));

			ImGui::NewLine();

			ImGui::Text("Total VM  : %.0f mb", totalVirtualMem / (1024 * 1024));

			plot("", virtualMemUsed, 0, totalVirtualMem);
			ImGui::SameLine();
			ImGui::Text("Used VM:\n%.0f mb", *std::next(virtualMemUsed.begin(), virtualMemUsed.size() - 1) / (1024 * 1024));
			ImGui::SameLine();
			plot("", virtualMemUsedByMe, 0, totalVirtualMem);
			ImGui::SameLine();
			ImGui::Text("VM Used by me:\n%.0f mb", *std::next(virtualMemUsedByMe.begin(), virtualMemUsedByMe.size() - 1) / (1024 * 1024));

			ImGui::NewLine();

			ImGui::Text("Total RAM : %.0f mb", totalPhysMem / (1024 * 1024));

			plot("", physMemUsed, 0, totalVirtualMem);
			ImGui::SameLine();
			ImGui::Text("Used RAM:\n%.0f mb", *std::next(physMemUsed.begin(), physMemUsed.size() - 1) / (1024 * 1024));
			ImGui::SameLine();
			plot("", physMemUsedByMe, 0, totalVirtualMem);
			ImGui::SameLine();
			ImGui::Text("RAM Used by me:\n%.0f mb", *std::next(physMemUsedByMe.begin(), physMemUsedByMe.size() - 1) / (1024 * 1024));

			ImGui::End();
		}
	}
}
