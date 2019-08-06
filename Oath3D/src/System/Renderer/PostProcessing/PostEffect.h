#pragma once

#include "Component/Asset/Model/Texture.h"
#include "System/Renderer/Deferred/FBO.h"

namespace Oath {

	namespace Render {

		class PostEffect {
		public:
			virtual void render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec) = 0;
		};
	}
}