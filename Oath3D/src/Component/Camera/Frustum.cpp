#include "Common/Common.h"
#include "Frustum.h"

namespace Oath {
	namespace Comp {

		void Frustum::Update(const glm::mat4 & mat) {
			//left
			planes[0].normal.x = mat[0][3] + mat[0][0];
			planes[0].normal.y = mat[1][3] + mat[1][0];
			planes[0].normal.z = mat[2][3] + mat[2][0];
			planes[0].distance = mat[3][3] + mat[3][0];

			// right
			planes[1].normal.x = mat[0][3] - mat[0][0];
			planes[1].normal.y = mat[1][3] - mat[1][0];
			planes[1].normal.z = mat[2][3] - mat[2][0];
			planes[1].distance = mat[3][3] - mat[3][0];

			// bottom
			planes[2].normal.x = mat[0][3] + mat[0][1];
			planes[2].normal.y = mat[1][3] + mat[1][1];
			planes[2].normal.z = mat[2][3] + mat[2][1];
			planes[2].distance = mat[3][3] + mat[3][1];

			// top
			planes[3].normal.x = mat[0][3] - mat[0][1];
			planes[3].normal.y = mat[1][3] - mat[1][1];
			planes[3].normal.z = mat[2][3] - mat[2][1];
			planes[3].distance = mat[3][3] - mat[3][1];

			// near
			planes[4].normal.x = mat[0][3] + mat[0][2];
			planes[4].normal.y = mat[1][3] + mat[1][2];
			planes[4].normal.z = mat[2][3] + mat[2][2];
			planes[4].distance = mat[3][3] + mat[3][2];

			// far
			planes[5].normal.x = mat[0][3] - mat[0][2];
			planes[5].normal.y = mat[1][3] - mat[1][2];
			planes[5].normal.z = mat[2][3] - mat[2][2];
			planes[5].distance = mat[3][3] - mat[3][2];


			for (plane& plane : planes) {
				float length = glm::length(plane.normal);
				plane.normal /= length;
				plane.distance /= length;
			}
		}

		bool Frustum::isInFrustum(const vec3 & min, const vec3 & max) {

			bool result = true;
			for (auto& plane : planes) {
				if (plane.distanceToPoint(getVP(plane.normal, min, max)) < 0)
					return false;
				else if (plane.distanceToPoint(getVN(plane.normal, min, max)) < 0)
					result = true;
			}
			return result;
		}
		vec3 Frustum::getVN(const vec3 & normal, const vec3 & min, const vec3 & max) const {

			glm::vec3 res = min;

			if (normal.x < 0)
				res.x = max.x;
			if (normal.y < 0)
				res.y = max.y;
			if (normal.z < 0)
				res.z = max.z;

			return res;
		}
		vec3 Frustum::getVP(const vec3 & normal, const vec3 & min, const vec3 & max) const {

			glm::vec3 res = min;

			if (normal.x > 0)
				res.x = max.x;
			if (normal.y > 0)
				res.y = max.y;
			if (normal.z > 0)
				res.z = max.z;

			return res;
		}
		float Frustum::plane::distanceToPoint(const vec3 & point) const {
			return glm::dot(point, normal) + distance;
		}
	}
}