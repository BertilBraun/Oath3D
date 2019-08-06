#include "Common/Common.h"
#include "AnimIndexModel.h"

namespace Oath {
	namespace Comp {

		void VertexBoneData::AddBoneData(uint BoneID, float Weight) {

			for (uint i = 0; i < MAX_EFFECTING_BONES; i++)
				if (Weights[i] == 0.0) {
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}

			//ASSERT(0);
		}
	}
}
