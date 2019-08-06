#pragma once

#include "ai.h"

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "String.h"

namespace Oath {

	using namespace glm;

	static vec3 operator * (const vec3& v, const mat4& m) {
		return vec3(vec4(v, 1) * m);
	}
	static vec3 operator * (const mat4& m, const vec3& v) {
		return vec3(m * vec4(v, 1));
	}

	namespace QUAT {


		inline quat LinearInterpolation(const quat& s, const quat& e, float fac) {

			assert(fac >= 0 && fac <= 1);

			return glm::normalize(glm::lerp(s, e, fac));
		}
	}

	namespace MAT4 {

		static mat4 toGLM(const aiMatrix4x4& m) {
			return mat4(m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4);
		}

		static mat4 toGLM(const aiMatrix3x3& m) {
			return mat4(m.a1, m.b1, m.c1, 0,
						m.a2, m.b2, m.c2, 0,
						m.a3, m.b3, m.c3, 0,
						0, 0, 0, 1);
		}

		static aiMatrix4x4 toAI(const mat4& m) {
			return aiMatrix4x4(m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3]);
		}

		inline mat4 ModelMat(const aiVector3D& s, const aiQuaternion& r, const aiVector3D& t) {
			return translate(vec3(t.x, t.y, t.z)) * toGLM(r.GetMatrix()) * scale(vec3(s.x, s.y, s.z));
		}

		inline mat4 ModelMat(const vec3& s, const quat& r, const vec3& t) {
			return translate(t) * mat4_cast(r) * scale(s);
		}

		inline String ToString(const mat4& m) {
			std::stringstream ss;

			ss << "\n";
			ss << "[0][0] " << m[0][0] << "\t ,[0][1] " << m[0][1] << "\t ,[0][2] " << m[0][2] << "\t ,[0][3] " << m[0][3] << "\n";
			ss << "[1][0] " << m[1][0] << "\t ,[1][1] " << m[1][1] << "\t ,[1][2] " << m[1][2] << "\t ,[1][3] " << m[1][3] << "\n";
			ss << "[2][0] " << m[2][0] << "\t ,[2][1] " << m[2][1] << "\t ,[2][2] " << m[2][2] << "\t ,[2][3] " << m[2][3] << "\n";
			ss << "[3][0] " << m[3][0] << "\t ,[3][1] " << m[3][1] << "\t ,[3][2] " << m[3][2] << "\t ,[3][3] " << m[3][3] << "\n";

			return ss.str();
		}

	}

	namespace VEC3 {

		inline String ToString(const vec3& o) {
			std::stringstream ss;

			ss << o.x << "x " << o.y << "y " << o.z << "z ";

			return ss.str();
		}
	};
}