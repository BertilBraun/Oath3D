#pragma once

#include "Common/Common.h"

#include "System/ECS/System.h"

namespace Oath {

	namespace Comp {

		class AssertRenderer : public ECS::System {
		public:
			AssertRenderer();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& components) override;

		};
	}
}