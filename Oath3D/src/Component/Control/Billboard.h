#pragma once

#include "System/ECS/Include.h"

namespace Oath {
	namespace Comp {

		struct Billboard : public ECS::Component<Billboard> {

			Billboard(ECS::Entity* e) : toLookAt(e) {}

			ECS::Entity* toLookAt = nullptr;
			float interpolationFactor = -1;
		};

	}
}