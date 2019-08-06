#include "Common/Common.h"
#include "SSR.h"
#include "System/Renderer/RenderEnums.h"
#include "System/Renderer/Shader/Shader.h"

#include "Component/Camera/Camera.h"

namespace Oath {

	namespace Render {

		static Vector<GLfloat> quadVertices = {
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
		};

		SSR::SSR()
		{
			shader = Shader::GetShader("PostProcessing/PostProcessing", "PostProcessing/SSR");
			quad.InitWithTexCoord(quadVertices, 2);
		}

		void SSR::render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec)
		{
			shader->start();
			shader->setUniform("FinalTexture", FINAL);
			shader->setUniform("NormalTexture", NORMAL);
			shader->setUniform("PositionTexture", POSITION);
			shader->setUniform("DepthTexture", DEPTH);
			shader->setUniform("SpecularTexture", SPECULAR);

			shader->setUniform("view", Comp::GCam->view);
			shader->setUniform("invView", glm::inverse(Comp::GCam->view));

			shader->setUniform("proj", Comp::GCam->proj);
			shader->setUniform("invProj", glm::inverse(Comp::GCam->proj));

			Final.Bind();
			Pos.Bind();
			Depth.Bind();
			Normal.Bind();
			Spec.Bind();

			quad.bind();
			quad.Draw();
		}
	}
}