#pragma once

#include "Common/Common.h"

namespace Oath {
	namespace Render {

		class ShadowBox {
		public:
			Vector<vec3> lbn, rtf;

			ShadowBox();

			void update(const vec3& direc);
		};

	}
}
