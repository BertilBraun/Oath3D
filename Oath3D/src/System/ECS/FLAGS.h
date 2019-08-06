#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace ECS {

		enum FLAGS {
			NONE,

			FLAG_OPTIONAL = BIT(0),
			//FLAG_AT_LEAST_ONE = BIT(1),

			NUM
		};

	}
}