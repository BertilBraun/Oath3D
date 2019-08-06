#include "Common/Common.h"
#include "IndexModel.h"

namespace Oath {

	void Comp::IndexModel::CalcAABB(vec3& min, vec3& max) const {

		for (const auto& p : data) {

			if (p.position.x < min.x)
				min.x = p.position.x;
			else if (p.position.x > max.x)
				max.x = p.position.x;
			
			if (p.position.y < min.y)
				min.y = p.position.y;
			else if (p.position.y > max.y)
				max.y = p.position.y;
			
			if (p.position.z < min.z)
				min.z = p.position.z;
			else if (p.position.z > max.z)
				max.z = p.position.z;
		}
	}

}