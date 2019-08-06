#include "Common/Common.h"
#include "AnimatedMesh.h"

#include <assimp/mesh.h>
#include <assimp/anim.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

namespace Oath {

	namespace Comp {

		AnimatedMesh::AnimatedMesh(GLuint vao, GLuint vbo, GLuint bbo, GLuint ibo, const String & name) :
			AssetMesh(vao, vbo, ibo, name)
		{
			this->bbo = bbo;
		}

		void AnimatedMesh::Delete()
		{
			AssetMesh::Delete();
			glDeleteBuffers(1, &bbo);
			
			bbo = 0;
		}

		void AnimatedMesh::SetAnimation(const String & key, float TTC)
		{
			for (uint i : range(scene->mNumAnimations))
				if (scene->mAnimations[i]->mName.data == key) {
					SetAnimation(i, TTC);
					return;
				}
		}

		void AnimatedMesh::SetAnimation(uint key, float TTC)
		{
			this->toGoToAnimation = scene->mAnimations[key];
			this->AnimationTime = 0;
			this->TTC = TTC;
			this->TSS = 0;
		}

		void AnimatedMesh::SetAnimation(const String & key)
		{
			for (uint i : range(scene->mNumAnimations))
				if (scene->mAnimations[i]->mName.data == key) {
					SetAnimation(i);
					return;
				}
		}

		void AnimatedMesh::SetAnimation(uint key)
		{
			this->toGoToAnimation = nullptr;
			this->animation = scene->mAnimations[key];
			this->AnimationTime = 0;
		}

		uint AnimatedMesh::FindPosition(const aiNodeAnim* AnimNode)
		{
			for (uint i = 0; i < AnimNode->mNumPositionKeys - 1; i++)
				if (AnimationTime < (float)AnimNode->mPositionKeys[i + 1].mTime)
					return i;

			ASSERT(0);
			return 0;
		}

		uint AnimatedMesh::FindRotation(const aiNodeAnim* AnimNode)
		{
			ASSERT(AnimNode->mNumRotationKeys > 0);

			for (uint i = 0; i < AnimNode->mNumRotationKeys - 1; i++)
				if (AnimationTime < (float)AnimNode->mRotationKeys[i + 1].mTime)
					return i;

			ASSERT(0);
			return 0;
		}

		uint AnimatedMesh::FindScaling(const aiNodeAnim* AnimNode)
		{
			ASSERT(AnimNode->mNumScalingKeys > 0);

			for (uint i = 0; i < AnimNode->mNumScalingKeys - 1; i++)
				if (AnimationTime < (float)AnimNode->mScalingKeys[i + 1].mTime)
					return i;

			ASSERT(0);
			return 0;
		}

		aiVector3D AnimatedMesh::CalcInterpolatedPosition(const aiNodeAnim* AnimNode)
		{
			if (AnimNode->mNumPositionKeys == 1) 
				return AnimNode->mPositionKeys[0].mValue;

			uint PositionIndex = FindPosition(AnimNode);
			uint NextPositionIndex = (PositionIndex + 1);
			
			ASSERT(NextPositionIndex < AnimNode->mNumPositionKeys);
			
			float DeltaTime = (float)(AnimNode->mPositionKeys[NextPositionIndex].mTime - AnimNode->mPositionKeys[PositionIndex].mTime);
			float Factor = (AnimationTime - (float)AnimNode->mPositionKeys[PositionIndex].mTime) / DeltaTime;

			return AIVEC3::LinearInterpolation(	AnimNode->mPositionKeys[PositionIndex].mValue,
												AnimNode->mPositionKeys[NextPositionIndex].mValue,
												Factor);
		}

		aiQuaternion AnimatedMesh::CalcInterpolatedRotation(const aiNodeAnim* AnimNode)
		{
			if (AnimNode->mNumRotationKeys == 1) 
				return AnimNode->mRotationKeys[0].mValue;

			uint RotationIndex = FindRotation(AnimNode);
			uint NextRotationIndex = (RotationIndex + 1);
			
			ASSERT(NextRotationIndex < AnimNode->mNumRotationKeys);
			
			float DeltaTime = (float)(AnimNode->mRotationKeys[NextRotationIndex].mTime - AnimNode->mRotationKeys[RotationIndex].mTime);
			float Factor = (AnimationTime - (float)AnimNode->mRotationKeys[RotationIndex].mTime) / DeltaTime;
			
			return AIQUAT::LinearInterpolation(	AnimNode->mRotationKeys[RotationIndex].mValue,
												AnimNode->mRotationKeys[NextRotationIndex].mValue,
												Factor);
		}

		aiVector3D AnimatedMesh::CalcInterpolatedScaling(const aiNodeAnim* AnimNode)
		{
			if (AnimNode->mNumScalingKeys == 1) 
				return AnimNode->mScalingKeys[0].mValue;

			uint ScalingIndex = FindScaling(AnimNode);
			uint NextScalingIndex = (ScalingIndex + 1);
			
			ASSERT(NextScalingIndex < AnimNode->mNumScalingKeys);
			
			float DeltaTime = (float)(AnimNode->mScalingKeys[NextScalingIndex].mTime - AnimNode->mScalingKeys[ScalingIndex].mTime);
			float Factor = (AnimationTime - (float)AnimNode->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
			
			return AIVEC3::LinearInterpolation(	AnimNode->mScalingKeys[ScalingIndex].mValue,
												AnimNode->mScalingKeys[NextScalingIndex].mValue,
												Factor);
		}

		void AnimatedMesh::ReadNodeHeirarchy(const aiNode* pNode, const mat4& ParentTransform)
		{
			String NodeName(pNode->mName.data);
			mat4 NodeTransformation = MAT4::toGLM(pNode->mTransformation);

			const aiNodeAnim* ToGoToAnimNode = nullptr;
			if (toGoToAnimation)
				ToGoToAnimNode = FindNodeAnim(toGoToAnimation, NodeName);
			const aiNodeAnim* AnimNode = FindNodeAnim(animation, NodeName);

			if (AnimNode) {
				aiVector3D	 s = CalcInterpolatedScaling(AnimNode);
				aiQuaternion r = CalcInterpolatedRotation(AnimNode);
				aiVector3D	 t = CalcInterpolatedPosition(AnimNode);

				if (ToGoToAnimNode) {
					aiVector3D	 ts = CalcInterpolatedScaling(ToGoToAnimNode);
					aiQuaternion tr = CalcInterpolatedRotation(ToGoToAnimNode);
					aiVector3D	 tt = CalcInterpolatedPosition(ToGoToAnimNode);
			
					ASSERT(TTC > 0);

					aiVector3D	 es = AIVEC3::LinearInterpolation(s, ts, glm::min(TSS / TTC, 1.f));
					aiQuaternion er = AIQUAT::LinearInterpolation(r, tr, glm::min(TSS / TTC, 1.f));
					aiVector3D	 et = AIVEC3::LinearInterpolation(t, tt, glm::min(TSS / TTC, 1.f));

					NodeTransformation = MAT4::ModelMat(es, er, et);
				}
				else
					NodeTransformation = MAT4::ModelMat(s, r, t);
			}

			mat4 GlobalTransformation = ParentTransform * NodeTransformation;

			if (boneMapping.find(NodeName) != boneMapping.end())
				boneInfo[boneMapping[NodeName]].FinalTransformation = transform * GlobalTransformation * boneInfo[boneMapping[NodeName]].BoneOffset;

			for (uint i : range(pNode->mNumChildren))
				ReadNodeHeirarchy(pNode->mChildren[i], GlobalTransformation);
		}

		void AnimatedMesh::Update(float deltaTime)
		{
			float TicksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
			AnimationTime = fmod(AnimationTime + deltaTime * TicksPerSecond, (float)animation->mDuration);

			TSS += deltaTime;
			if (TSS > TTC && toGoToAnimation) {
				animation = toGoToAnimation;
				toGoToAnimation = nullptr;
			}

			ReadNodeHeirarchy(scene->mRootNode, mat4());
		}

		const aiNodeAnim* AnimatedMesh::FindNodeAnim(const aiAnimation* pAnimation, const String& NodeName)
		{
			for (uint i = 0; i < pAnimation->mNumChannels; i++)
				if (pAnimation->mChannels[i]->mNodeName.data == NodeName)
					return pAnimation->mChannels[i];

			return NULL;
		}
	}
}