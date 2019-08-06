#pragma once

#include "System/Renderer/PostProcessing/PostEffect.h"
#include "Component/Asset/Model/BasicMesh.h"

namespace Oath {

	namespace Render {

		class Shader;

		class PPEffectInverseColor : public PostEffect {
		public:

			PPEffectInverseColor();

			void render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec) override;

			Shader* shader;
			Comp::BasicMesh quad;
		};
	}
}