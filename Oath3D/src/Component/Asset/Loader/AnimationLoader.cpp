#include "Common/Common.h"
#include "AnimationLoader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Oath {

	using namespace Comp;

	AnimationLoader::AnimationLoader(const String& filename) : BaseLoader(filename) {

		if (isValid)
			init();
	}

	void AnimationLoader::init()
	{
		transform = MAT4::toGLM(scene->mRootNode->mTransformation);
		transform = glm::inverse(transform);

		mat		.resize(scene->mNumMaterials);
		entries	.resize(scene->mNumMeshes);

		uint NumVertices = 0, NumIndices = 0;
		
		for (uint i : range(scene->mNumMeshes)) {
			entries[i].MatIndex		= scene->mMeshes[i]->mMaterialIndex;
			entries[i].NumIndices	= scene->mMeshes[i]->mNumFaces * 3;
			entries[i].BaseVertex	= NumVertices;
			entries[i].BaseIndex	= NumIndices;

			NumVertices += scene->mMeshes[i]->mNumVertices;
			NumIndices	+= entries[i].NumIndices;
		}

		model.bones  .resize(NumVertices);
		model.data	 .reserve(NumVertices);
		model.indices.reserve(NumVertices);

		for (uint i : range(scene->mNumMeshes))
			processMesh(i, scene->mMeshes[i]);

		for (uint i : range(scene->mNumMaterials))
			mat[i] = processMat(scene->mMaterials[i]);
	}

	void AnimationLoader::processMesh(uint i, aiMesh * mesh)
	{
		for (auto i : range(mesh->mNumVertices)) {

			VertexData tmp;

			tmp.position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			
			if (mesh->mNormals)
				tmp.normal	 = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTangents)
				tmp.tangent = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

			if (mesh->mColors[0])
				tmp.color = vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);

			if (mesh->mTextureCoords[0])
				tmp.texture = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			model.data.push_back(tmp);
		}

		loadBones(i, mesh);

		for (auto i : range(mesh->mNumFaces))
			for (auto j : range(mesh->mFaces[i].mNumIndices))
				model.indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

	void AnimationLoader::loadBones(uint index, aiMesh * mesh)
	{
		for (uint i = 0; i < mesh->mNumBones; i++) {
			uint BoneIndex = 0;
			String BoneName(mesh->mBones[i]->mName.data);

			if (boneMapping.find(BoneName) == boneMapping.end()) {
				BoneIndex = numBones++;
				boneInfo.push_back(BoneInfo());
			}
			else 
				BoneIndex = boneMapping[BoneName];
			
			boneMapping[BoneName] = BoneIndex;
			boneInfo[BoneIndex].BoneOffset = MAT4::toGLM(mesh->mBones[i]->mOffsetMatrix);

			for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
				uint VertexID = entries[index].BaseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = mesh->mBones[i]->mWeights[j].mWeight;
				model.bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
	}
}