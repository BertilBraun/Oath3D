#pragma once

#include "Component/Asset/Model/Asset/IndexModel.h"

#define MAX_EFFECTING_BONES 4
#define MAX_NUM_BONES		100

namespace Oath {
	namespace Comp {

		struct VertexBoneData
		{
			uint IDs[MAX_EFFECTING_BONES]{ 0 };
			float Weights[MAX_EFFECTING_BONES]{ 0 };

			void AddBoneData(uint BoneID, float Weight);
		};

		struct AnimIndexModel : public IndexModel {

			Vector<VertexBoneData> bones;
		};
	}
}