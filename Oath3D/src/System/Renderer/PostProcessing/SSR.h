#pragma once

#include "PostEffect.h"
#include "Component/Asset/Model/BasicMesh.h"

namespace Oath {

	namespace Render {

		class Shader;

		class SSR : public PostEffect {
		public:
			SSR();

			void render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec) override;

			Shader* shader;
			Comp::BasicMesh quad;
		};
	}
}