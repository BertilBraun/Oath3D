#include "Common/Common.h"
#include "ShadowRenderer.h"

namespace Oath {

	namespace Render {

		ShadowRenderer::ShadowRenderer(GLsizei w, GLsizei h) : w(w), h(h)
		{
			addDepth(w, h);

			complete();
		}

		void ShadowRenderer::bind() const
		{
			GLCall(glViewport(0, 0, w, h));

			FBO::bind();

			GLCall(glDepthMask(GL_TRUE));
			GLCall(glEnable(GL_DEPTH_TEST));
			GLCall(glClear(GL_DEPTH_BUFFER_BIT));

			//glCullFace(GL_FRONT);

			GLCall(glDrawBuffer(GL_NONE));
			GLCall(glReadBuffer(GL_NONE));
		}

		void ShadowRenderer::unBind() const
		{
			FBO::unBind();
			//glCullFace(GL_BACK);

			GLCall(glDepthMask(GL_FALSE));
			GLCall(glViewport(0, 0, (GLsizei)Settings::WIDTH, (GLsizei)Settings::HEIGHT));
		}
	}
}