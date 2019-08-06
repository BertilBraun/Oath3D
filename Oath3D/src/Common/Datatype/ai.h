#pragma once

#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>

#include "String.h"

namespace Oath {

	namespace AIVEC3 {

		inline aiVector3D operator - (const aiVector3D& v1, const aiVector3D& v2) {
			return aiVector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
		}
		inline aiVector3D operator * (float f, const aiVector3D& v) {
			return aiVector3D(f*v.x, f*v.y, f*v.z);
		}
		inline aiVector3D operator + (const aiVector3D& v1, const aiVector3D& v2) {
			return aiVector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
		}
		
		inline String ToString(const aiVector3D& o) {
			std::stringstream ss;

			ss << o.x << "x " << o.y << "y " << o.z << "z ";

			return ss.str();
		}

		inline aiVector3D LinearInterpolation(const aiVector3D& s, const aiVector3D& e, float fac) {

			assert(fac >= 0 && fac <= 1);

			return s + fac * (e - s);
		}

		inline aiVector3D cubicInterpolate(const aiVector3D& s0, const aiVector3D& s1, const aiVector3D& s2, const aiVector3D& s3, float x) {

			assert(x >= 0 && x <= 1);

			return s1 + 0.5f * x*(s2 - s0 + x * (2.0f*s0 - 5.0f*s1 + 4.0f*s2 - s3 + x * (3.0f*(s1 - s2) + s3 - s0)));
		}
	}

	namespace AIQUAT {

		inline String ToString(const aiQuaternion& o) {
			std::stringstream ss;

			ss << o.x << "x " << o.y << "y " << o.z << "z " << o.w << "w";

			return ss.str();
		}

		inline aiQuaternion LinearInterpolation(const aiQuaternion& s, const aiQuaternion& e, float fac) {

			assert(fac >= 0 && fac <= 1);

			aiQuaternion quat;
			aiQuaternion::Interpolate(quat, s, e, fac);
			return quat.Normalize();
		}
	}
}