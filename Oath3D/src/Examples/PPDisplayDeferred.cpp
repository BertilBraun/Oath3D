#include "Common/Common.h"
#include "PPDisplayDeferred.h"
#include "System/Renderer/RenderEnums.h"
#include "System/Renderer/Shader/Shader.h"

#include "System/System/Keyboard.h"

namespace Oath {
	namespace Render {

		const Vector<GLfloat> quadVertices = {
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
		};

		PPDisplayDeferred::PPDisplayDeferred()
		{
			shader = Shader::GetShader("PostProcessing/PostProcessing", "PostProcessing/PPDisplayDeferred");
			quad.InitWithTexCoord(quadVertices, 2);
		}

		void PPDisplayDeferred::render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec)
		{
			shader->start();
			shader->setUniform("NormalTexture", NORMAL);
			shader->setUniform("PositionTexture", POSITION);
			shader->setUniform("AlbedoTexture", DIFFUSE);
			shader->setUniform("SpecularTexture", SPECULAR);

			shader->setUniform("ScreenSize", vec2(Settings::WIDTH, Settings::HEIGHT));

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