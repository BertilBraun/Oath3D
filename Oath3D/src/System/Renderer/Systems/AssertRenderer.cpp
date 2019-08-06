#include "Common/Common.h"
#include "AssertRenderer.h"

#include "System/Renderer/MasterRenderer.h"
#include "Component/Asset/Asset.h"

namespace Oath {

	namespace Comp {

		AssertRenderer::AssertRenderer() {

			addComponent(Asset::Asset::ID);
		}

		void AssertRenderer::updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& components) {

			Render::GRenderer->process((Comp::Asset*)components[0], &entity->transform);
		}
	}
}