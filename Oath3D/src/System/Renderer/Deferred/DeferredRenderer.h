#pragma once

#include "FBO.h"

namespace Oath {

	namespace Render {

		class DeferredRenderer : public FBO {
		public:
			DeferredRenderer();
			~DeferredRenderer();

			void startFrame();
			void bindForGeomPass();
			void bindForStencilPass();
			void bindForLightPass();
			void bindForShadowPass();
			void bindForPostEffectPass();
			void bindForFinalPass();

			void resize(int w, int h);

			Comp::Texture finalTexture;
		};
	}
}