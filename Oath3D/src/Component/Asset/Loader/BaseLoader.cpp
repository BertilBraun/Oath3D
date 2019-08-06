#include "Common/Common.h"
#include "BaseLoader.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Oath {

	BaseLoader::BaseLoader(const String& filename) {

		LOGINFO("LOADER", "LOADING ASSET : " + filename);

		scene = aiImportFile(filename.c_str(), 
			aiProcess_Triangulate			|
			aiProcess_JoinIdenticalVertices	|
			aiProcess_FlipUVs				|
			aiProcess_GenNormals			|
			aiProcess_FixInfacingNormals	|
			aiProcess_GenSmoothNormals		|
			aiProcess_CalcTangentSpace		| 
			aiProcess_OptimizeGraph			|
			aiProcess_OptimizeMeshes);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LOGERROR("FAILED TO LOAD :", TRIMPATH(filename.c_str()), "ERROR :", aiGetErrorString());
			isValid = false;
		}

		folder = String::TrimTail(filename, (filename.find('\\') != String::npos) ? '\\' : '/');
	}

	template<typename T>
	T GetMat(aiMaterial* mat, const char* pKey, unsigned int type, unsigned int idx) {
		T instance;

		if (mat->Get(pKey, type, idx, instance) != aiReturn_SUCCESS)
			int i = 0;// LOG("FAILED TO LOAD MATERIAL:", pKey);

		return instance;
	}

	String GetTex(const String& folder, aiMaterial* mat, aiTextureType type) {

		aiString str("");

		if (mat->GetTextureCount(type)) {

			mat->GetTexture(type, 0, &str);

			return folder + str.C_Str();
		}

		return str.C_Str();
	}

	Comp::Material* BaseLoader::processMat(aiMaterial * aimat)
	{
		Comp::LoadMaterial mat;

		mat.SpecularExponent		= GetMat<float>(aimat, AI_MATKEY_SHININESS);
		mat.TwoSided				= GetMat<bool>(aimat, AI_MATKEY_TWOSIDED);

		if ((int)mat.TwoSided > 1)
			mat.TwoSided = false;

		mat.DiffuseTexturePath		= GetTex(folder, aimat, aiTextureType::aiTextureType_DIFFUSE);
		mat.DisplacementTexturePath = GetTex(folder, aimat, aiTextureType::aiTextureType_DISPLACEMENT);
		mat.NormalTexturePath		= GetTex(folder, aimat, aiTextureType::aiTextureType_NORMALS);
		mat.SpecularTexturePath		= GetTex(folder, aimat, aiTextureType::aiTextureType_SHININESS);
		mat.SpecularHightlightPath	= GetTex(folder, aimat, aiTextureType::aiTextureType_SPECULAR);
		mat.LightTexturePath		= GetTex(folder, aimat, aiTextureType::aiTextureType_LIGHTMAP);
		mat.HeightTexturePath		= GetTex(folder, aimat, aiTextureType::aiTextureType_HEIGHT);

		return new Comp::Material(mat);
	}
}