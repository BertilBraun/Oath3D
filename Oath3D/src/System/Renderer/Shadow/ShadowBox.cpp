#include "Common/Common.h"
#include "ShadowBox.h"
#include "Component/Camera/Camera.h"

#include "glm/matrix.hpp"

namespace Oath {
	namespace Render {

		ShadowBox::ShadowBox() {

			lbn.resize(Settings::NUM_CASCADES);
			rtf.resize(Settings::NUM_CASCADES);
		}

		void ShadowBox::update(const vec3& direc) {

			vec3 pos = Comp::GCam->pos();
			mat4 CamInv = glm::inverse(glm::lookAt(pos, pos - normalize(Comp::GCam->forward), vec3(0, 1, 0)));

			mat4 LightMat = glm::lookAt(vec3(0, 0, 0), normalize(direc), vec3(0, 1, 0));

			float Htan = tanf(radians(Settings::FOV / 2.0f));
			float Vtan = tanf(radians((Settings::FOV * (Settings::HEIGHT / Settings::WIDTH)) / 2.0f));

			for (int i : range(Settings::NUM_CASCADES)) {

				float xn = Settings::CASCADES[i] * Htan;
				float xf = Settings::CASCADES[i + 1] * Htan;
				float yn = Settings::CASCADES[i] * Vtan;
				float yf = Settings::CASCADES[i + 1] * Vtan;

				vec4 frustumCorners[8] = {
					// near face
					vec4(xn,  yn, Settings::CASCADES[i], 1.0),
					vec4(-xn,  yn, Settings::CASCADES[i], 1.0),
					vec4(xn, -yn, Settings::CASCADES[i], 1.0),
					vec4(-xn, -yn, Settings::CASCADES[i], 1.0),

					// far face
					vec4(xf,  yf, Settings::CASCADES[i + 1], 1.0),
					vec4(-xf,  yf, Settings::CASCADES[i + 1], 1.0),
					vec4(xf, -yf, Settings::CASCADES[i + 1], 1.0),
					vec4(-xf, -yf, Settings::CASCADES[i + 1], 1.0)
				};

				lbn[i] = vec3(std::numeric_limits<float>::max());
				rtf[i] = vec3(std::numeric_limits<float>::min());

				for (int j : range(8)) {

					vec4 fc = LightMat * CamInv * frustumCorners[j];

					lbn[i].x = min(lbn[i].x, fc.x);
					rtf[i].x = max(rtf[i].x, fc.x);
					lbn[i].y = min(lbn[i].y, fc.y);
					rtf[i].y = max(rtf[i].y, fc.y);
					lbn[i].z = min(lbn[i].z, fc.z);
					rtf[i].z = max(rtf[i].z, fc.z);
				}

				lbn[i].z -= Settings::SHADOW_OFFSET;
			}
		}
	}
}
