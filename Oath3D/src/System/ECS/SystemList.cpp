#include "Common/Common.h"
#include "SystemList.h"

#include "System.h"

namespace Oath {

	namespace ECS {
		SystemList::~SystemList() {

			for (System* sys : systems)
				SAVE_DELETE(sys)
		}

		bool SystemList::addSystem(System * system) {
			if (!system->isValid())
				return false;

			systems.push_back(system);
			return true;
		}

		bool SystemList::removeSystem(System* system)
		{
			for (uint32 i = 0; i < systems.size(); i++)
				if (system == systems[i]) {
					systems.erase(systems.begin() + i);
					return true;
				}

			return false;
		}

	}
}
