#include "Common/Common.h"
#include "AssetLoader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Oath {

	AssetLoader::AssetLoader(const String& filename) : BaseLoader(filename) {

		if (isValid) 
			init();
	}

	AssetLoader::~AssetLoader()
	{
		aiReleaseImport(scene);
	}

	void AssetLoader::init()
	{
		uint NumVertices = 0, NumIndices = 0;

		mat.resize(scene->mNumMaterials);
		entries.resize(scene->mNumMeshes);

		for (uint i : range(scene->mNumMeshes)) {
			entries[i].MatIndex		= scene->mMeshes[i]->mMaterialIndex;
			entries[i].NumIndices	= scene->mMeshes[i]->mNumFaces * 3;
			entries[i].BaseVertex	= NumVertices;
			entries[i].BaseIndex	= NumIndices;

			NumVertices += scene->mMeshes[i]->mNumVertices;
			NumIndices	+= entries[i].NumIndices;
		}

		model.data.reserve(NumVertices);
		model.indices.reserve(NumVertices);

		for (uint i : range(scene->mNumMeshes))
			processMesh(scene->mMeshes[i]);

		for (uint i : range(scene->mNumMaterials))
			mat[i] = processMat(scene->mMaterials[i]);
	}

	void AssetLoader::processMesh(aiMesh * mesh)
	{
		for (auto i : range(mesh->mNumVertices)) {

			Comp::VertexData tmp;

			tmp.position	= vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			
			if (mesh->mNormals)
				tmp.normal	= vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

			if (mesh->mTangents)
				tmp.tangent = vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

			if (mesh->mColors[0])
				tmp.color	= vec3(mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b);

			if (mesh->mTextureCoords[0])
				tmp.texture = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

			model.data.push_back(tmp);
		}

		for (auto i : range(mesh->mNumFaces)) 
			for (auto j : range(mesh->mFaces[i].mNumIndices))
				model.indices.push_back(mesh->mFaces[i].mIndices[j]);
	}

}