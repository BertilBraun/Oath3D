#pragma once

#include "Common/Common.h"

#include "BaseLoader.h"
#include "Component/Asset/Model/Asset/AssetMesh.h"
#include "Component/Asset/Model/Animation/AnimIndexModel.h"

struct aiMesh;

namespace Oath {

	namespace Comp {
		struct BoneInfo
		{
			mat4 BoneOffset = mat4(1);
			mat4 FinalTransformation = mat4(1);
		};
	}

	class AnimationLoader : public BaseLoader {
	public:
		AnimationLoader(const String& filename);

		Vector<Comp::MeshEntry> entries;
		Vector<Comp::Material*> mat;
		Vector<Comp::BoneInfo> boneInfo;

		std::map<String, uint> boneMapping;

		Comp::AnimIndexModel model;

		mat4 transform;
		uint numBones;

	private:
		void init();
		void processMesh(uint i, aiMesh* mesh);
		void loadBones(uint i, aiMesh* mesh);
	};
}
