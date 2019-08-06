#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace ECS {
		class System;

		class SystemList {
		public:
			~SystemList();

			inline Vector<System*>& Systems() {
				return systems;
			}

			bool addSystem(System* system);
			bool removeSystem(System* system);
		private:
			Vector<System*> systems;
		};
	}
}