#include "Common/Common.h"
#include "AnimationRenderer.h"

#include "System/Renderer/MasterRenderer.h"
#include "Component/Asset/AnimatedAsset.h"

namespace Oath {

	namespace Comp {

		AnimationRenderer::AnimationRenderer() {

			addComponent(Comp::AnimatedAsset::ID);
		}

		void AnimationRenderer::updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& components) {

			Comp::AnimatedAsset* anim = (Comp::AnimatedAsset*)components[0];

			anim->mesh->Update(delta);

			Render::GRenderer->process(anim, &entity->transform);
		}
	}
}