#pragma once

#include "Component/Asset/Model/Material.h"
#include "Component/Asset/Model/Asset/AssetMesh.h"
#include "Component/Asset/Loader/AnimationLoader.h"

#include <assimp/vector3.h>
#include <assimp/quaternion.h>

struct aiScene;
struct aiAnimation;
struct aiNodeAnim;
struct aiAnimation;

namespace Oath {

	namespace Comp {

		enum class ANIM_VB_TYPES : uint {
			BONEIDs = 5,
			WEIGHTs,
			NUM
		};
		
		struct AnimatedMesh : public AssetMesh {

			AnimatedMesh(GLuint vao, GLuint vbo, GLuint bbo, GLuint ibo, const String & name);
			void Update(float deltaTime);			
			void Delete();

			void SetAnimation(const String& key, float TTC);
			void SetAnimation(const String& key);

			void SetAnimation(uint key, float TTC);
			void SetAnimation(uint key);

			std::map<String, uint> boneMapping;
			Vector<BoneInfo> boneInfo;

			uint numBones;
			mat4 transform;

			const aiScene* scene;

		private:
			void ReadNodeHeirarchy(const aiNode* pNode, const mat4& ParentTransform);
			
			aiVector3D	 CalcInterpolatedScaling (const aiNodeAnim* AnimNode);
			aiQuaternion CalcInterpolatedRotation(const aiNodeAnim* AnimNode);
			aiVector3D	 CalcInterpolatedPosition(const aiNodeAnim* AnimNode);
			
			uint FindScaling (const aiNodeAnim* AnimNode);
			uint FindRotation(const aiNodeAnim* AnimNode);
			uint FindPosition(const aiNodeAnim* AnimNode);
			
			const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const String& NodeName);

			GLuint bbo;
			float AnimationTime = 0, TTC = 0, TSS = 0;
			aiAnimation* animation = nullptr, *toGoToAnimation = nullptr;
		};
	}
}
