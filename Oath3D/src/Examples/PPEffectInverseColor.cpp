#include "Common/Common.h"
#include "PPEffectInverseColor.h"
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

		PPEffectInverseColor::PPEffectInverseColor()
		{
			shader = Shader::GetShader("PostProcessing/PostProcessing", "PostProcessing/PPEffectInverseColor");
			quad.InitWithTexCoord(quadVertices, 2);
		}

		void PPEffectInverseColor::render(const Comp::Texture& Final, const Comp::Texture& Depth, const Comp::Texture& Pos, const Comp::Texture& Diffuse, const Comp::Texture& Normal, const Comp::Texture& Spec)
		{
			shader->start();
			shader->setUniform("FinalTexture", FINAL);
			shader->setUniform("InverseColors", Key::isKeyHeld(Key::SPACE));

			Final.Bind();

			quad.bind();
			quad.Draw();
		}
	}
}