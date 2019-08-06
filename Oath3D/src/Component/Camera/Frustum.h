#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace Comp {

		class Frustum {
		public:
			void Update(const glm::mat4& mat);

			bool isInFrustum(const vec3& min, const vec3& max);

			vec3 getVN(const vec3& normal, const vec3& min, const vec3& dim) const;
			vec3 getVP(const vec3& normal, const vec3& min, const vec3& dim) const;

		private:
			struct plane {
				vec3 normal;
				float distance;
				float distanceToPoint(const vec3& point) const;
			};

			std::array<plane, 6> planes;
		};
	}
}