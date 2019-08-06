#pragma once

#include "Common/Common.h"

#include "Component/Asset/Model/Texture.h"

namespace Oath {

	namespace Render {

		class FBO {
		public:
			FBO();
			~FBO();

			void resize(int w, int h);

			void bind() const;
			void unBind() const;
			void bindDraw();
			void unBindDraw();
			void bindRead();
			void unBindRead();

			void bindDrawBuffers();
			void bindTextures();
			void bindDepth(int offset = 0);

			void addAttachment(uint att, GLsizei width, GLsizei height, Comp::Texture* texIn = nullptr);
			void addDepthStencil(GLsizei width, GLsizei height);
			void addDepth(GLsizei width, GLsizei height);
			void complete();

			Comp::Texture depth;
			Vector<Comp::Texture> buffers;
		private:
			GLuint framebuffer;
			bool stencil = false;
		};
	}
}