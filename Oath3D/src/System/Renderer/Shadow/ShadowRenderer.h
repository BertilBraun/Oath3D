#pragma once

#include "System/Renderer/Deferred/FBO.h"
#include "Component/Camera/Camera.h"

namespace Oath {

	namespace Render {

		class ShadowRenderer : public FBO {
		public:
			ShadowRenderer(GLsizei w, GLsizei h);

			void bind() const;
			void unBind() const;

			Comp::Frustum frustum;
			mat4 view, proj, viewProj;
			GLsizei w, h;
		};
	}
}