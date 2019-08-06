#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace Comp {

		struct VertexData {
			vec3 position	= vec3(0);
			vec3 normal		= vec3(0, 1, 0);
			vec3 tangent	= vec3(0, 0, 0);
			vec3 color		= vec3(0);
			vec2 texture	= vec2(0);
		};

		struct IndexModel {
			String name;

			Vector<VertexData> data;
			Vector<GLuint> indices;

			void CalcAABB(vec3& min, vec3& max) const;
		};
	}
}