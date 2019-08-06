#include "Common/Common.h"
#include "FBO.h"

#include "System/Renderer/Shader/Shader.h"
#include "System/Application/Application.h"
#include "System/Renderer/RenderEnums.h"

namespace Oath {

	namespace Render {

		FBO::FBO() {

			glGenFramebuffers(1, &framebuffer);
			bind();
		}
		
		FBO::~FBO() {
			for (auto& b : buffers)
				b.Delete();

			depth.Delete();
			glDeleteFramebuffers(1, &framebuffer);
		}

		void FBO::addAttachment(uint att, GLsizei width, GLsizei height, Comp::Texture* texIn) {
			Comp::Texture tex(0, att);

			GLCall(glGenTextures(1, &tex.texID));
			GLCall(glBindTexture(GL_TEXTURE_2D, tex.texID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + att, GL_TEXTURE_2D, tex.texID, 0));

			if (texIn)
				*texIn = tex;
			else
				buffers.emplace_back(tex);
		}

		void FBO::addDepthStencil(GLsizei width, GLsizei height)
		{
			depth.offset = DEPTH;
			stencil = true;
			GLCall(glGenTextures(1, &depth.texID));
			GLCall(glBindTexture(GL_TEXTURE_2D, depth.texID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, NULL));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth.texID, 0));
		}

		void FBO::addDepth(GLsizei width, GLsizei height)
		{
			depth.offset = DEPTH;
			GLCall(glGenTextures(1, &depth.texID));
			GLCall(glBindTexture(GL_TEXTURE_2D, depth.texID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth.texID, 0));
		}
		
		void FBO::complete()
		{
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				LOG("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

			unBind();
		}

		void FBO::bind() const {
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		}

		void FBO::unBind() const {
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}

		void FBO::bindDraw()
		{
			GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer));
		}

		void FBO::unBindDraw()
		{
			GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		}

		void FBO::bindRead()
		{
			GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer));
		}

		void FBO::unBindRead()
		{
			GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
		}

		void FBO::bindDrawBuffers()
		{
			Vector<GLenum> DrawBuffers;
			for (auto b : buffers)
				DrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + b.offset);

			GLCall(glDrawBuffers((GLsizei)DrawBuffers.size(), DrawBuffers.data()));
		}

		void FBO::bindTextures()
		{
			for (auto& b : buffers)
				b.Bind();
		}

		void FBO::bindDepth(int offset)
		{
			depth.Bind(offset);
		}

		void FBO::resize(int w, int h)
		{
			bind();

			for (auto& b : buffers) {
				b.Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
			}

			if (depth.texID)
				if (stencil)
					addDepthStencil(w, h);
				else
					addDepth(w, h);

			unBind();
		}
	}
}