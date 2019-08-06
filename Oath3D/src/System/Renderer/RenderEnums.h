#pragma once

namespace Oath {
	namespace Render {

		enum TEXTURE_TYPE {
			POSITION,
			DIFFUSE,
			NORMAL,
			SPECULAR,
			NUM,
			FINAL,
			DEPTH
		};

		enum class ShadowMappingType {
			DYNAMIC,
			STATIC,
			NO,
			NUM
		};

	}
}