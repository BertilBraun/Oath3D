#pragma once

#include "System/ECS/Component.h"

#include "Component/Asset/Model/Animation/AnimatedMesh.h"

namespace Oath {
	namespace Comp {

		struct AnimatedAsset : public ECS::Component<AnimatedAsset> {

			AnimatedAsset() { }
			AnimatedAsset(const String& path, const int& flagsIn = 0) : name(path), flags(flagsIn) {}

			AnimatedMesh* mesh = nullptr;
			String name;
			int flags = 0;
		};
	
	}
}