#include "Common/Common.h"
#include "DeferredRenderer.h"

#include "System/Application/Application.h"

#include "System/Renderer/Shader/Shader.h"

#include "System/Renderer/RenderEnums.h"

namespace Oath {
	namespace Render {

		DeferredRenderer::DeferredRenderer()
		{
			GLsizei width = (GLsizei)Settings::WIDTH;
			GLsizei height = (GLsizei)Settings::HEIGHT;

			for (uint i : range((uint)NUM))
				addAttachment(i, width, height);

			addDepthStencil(width, height);

			finalTexture.offset = FINAL;
			glGenTextures(1, &finalTexture.texID);
			glBindTexture(GL_TEXTURE_2D, finalTexture.texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + FINAL, GL_TEXTURE_2D, finalTexture.texID, 0);

			complete();
		}

		DeferredRenderer::~DeferredRenderer()
		{
			finalTexture.Delete();
		}

		void DeferredRenderer::startFrame()
		{
			bindDraw();
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + FINAL);
			glClearColor(0, 0, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void DeferredRenderer::bindForGeomPass()
		{
			bindDraw();
			bindDrawBuffers();
		}

		void DeferredRenderer::bindForStencilPass()
		{
			bindDraw();
			glDrawBuffer(GL_NONE);
		}

		void DeferredRenderer::bindForLightPass()
		{
			bindDraw();
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + FINAL);
			bindTextures();
		}

		void DeferredRenderer::bindForShadowPass()
		{
			bindDraw();
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + FINAL);
			bindTextures();
			finalTexture.Bind();
		}

		void DeferredRenderer::bindForPostEffectPass()
		{
			bindDraw();
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + FINAL);
			bindTextures();
			finalTexture.Bind();
		}

		void DeferredRenderer::bindForFinalPass()
		{
			unBindDraw();
			unBind();
		}

		void DeferredRenderer::resize(int w, int h)
		{
			FBO::resize(w, h);

			finalTexture.Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_FLOAT, NULL);
		}
	}
}