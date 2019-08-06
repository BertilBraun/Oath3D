#include "Common/Common.h"
#include "Loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>

#include "Component/Asset/Loader/AssetLoader.h"
#include "Component/Asset/Loader/AnimationLoader.h"

#include "Component/Asset/Asset.h"
#include "Component/Asset/AnimatedAsset.h"

#include "Component/Asset/Model/Texture.h"
#include "Component/Asset/Model/Material.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"

namespace Oath {
	namespace System {

		Comp::AssetMesh* Loader::LoadAsset(const String & path)
		{
			AssetLoader loader(Settings::MODEL_PATH + path);

			if (!loader.isValid)
				return nullptr;

			Comp::AssetMesh* asset	= LoadModel(loader.model);
			asset->entries			= std::move(loader.entries);
			asset->mat				= std::move(loader.mat);

			loader.model.CalcAABB(asset->minIn, asset->maxIn);

			return asset;
		}

		Comp::AnimatedMesh * Loader::LoadAnim(const String & path)
		{
			AnimationLoader loader(Settings::MODEL_PATH + path);

			if (!loader.isValid)
				return nullptr;

			Comp::AnimatedMesh* asset	= LoadAnimModel(loader.model);
			
			asset->entries		= std::move(loader.entries);
			asset->mat			= std::move(loader.mat);

			asset->boneMapping	= std::move(loader.boneMapping);
			asset->boneInfo		= std::move(loader.boneInfo);

			asset->scene		= std::move(loader.scene);
			asset->transform	= std::move(loader.transform);
			asset->numBones		= std::move(loader.numBones);

			asset->SetAnimation(0);

			loader.model.CalcAABB(asset->minIn, asset->maxIn);

			return asset;
		}

		Comp::Texture * Loader::LoadTexture(const String & file)
		{
			LOGINFO("LOADER", "LOADING TEXTURE : " + file);

			Image image = LoadImageFromFile(file);

			unsigned int ID = 0;

			if (image.data) {
				GLCall(glGenTextures(1, &ID));
				GLCall(glBindTexture(GL_TEXTURE_2D, ID));

				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
				GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
				GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -1));

				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data));
				GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			}

			stbi_image_free(image.data);

			return new Comp::Texture(ID);
		}

		uint Loader::LoadAudio(const String & file)
		{
			String path = Settings::AUDIO_PATH + file;
			LOGINFO("LOADER", "LOADING AUDIO : " + path);

			ALvoid *data;
			ALsizei size, freq;
			ALenum format;
			ALboolean loop;

			ALCall(alutLoadWAVFile((ALbyte*)path.c_str(), &format, &data, &size, &freq, &loop));

			uint buffer = 0;
			ALCall(alGenBuffers(1, &buffer));
			ALCall(alBufferData(buffer, format, data, size, freq));

			ALCall(alutUnloadWAV(format, data, size, freq));

			return buffer;
		}

		Comp::Texture * Loader::LoadCubeMap(const String & file, const String& ext)
		{
			static const std::string pathadd[6] = { "right", "left", "top", "bottom", "back", "front" };
			LOGINFO("LOADER", "LOADING CUBEMAP : " + Settings::TEXTURE_PATH + file);

			GLuint ID = 0;
			GLCall(glGenTextures(1, &ID));
			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, ID));

			for (int i = 0; i < 6; i++) {
				Image image = LoadImageFromFile(Settings::TEXTURE_PATH + file + pathadd[i] + ext);

				if (image.data) 
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);

				stbi_image_free(image.data);
			}

			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
			GLCall(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, -1));
			GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

			return new Comp::Texture(ID, 0, GL_TEXTURE_CUBE_MAP);
		}

		void Loader::FreeAsset(Comp::AssetMesh * asset)
		{
			asset->Delete();
			SAVE_DELETE(asset);
		}

		void Loader::FreeAnim(Comp::AnimatedMesh* anim)
		{
			anim->Delete();
			aiReleaseImport(anim->scene);
			SAVE_DELETE(anim);
		}

		void Loader::FreeTexture(Comp::Texture * texture)
		{
			texture->Delete();
			SAVE_DELETE(texture);
		}

		void Loader::FreeAudio(uint buffer)
		{
			alDeleteBuffers(1, &buffer);
		}

		Comp::Asset* Loader::LoadAsset(const Comp::IndexModel & mesh)
		{
			Comp::Asset* asset = new Comp::Asset();
			asset->names = { "", "", "", "" };

			asset->meshs[0] = LoadModel(mesh);
			asset->meshs[0]->instanceID = 0;

			Comp::MeshEntry entry;
			entry.NumIndices = (uint)mesh.indices.size();
			asset->meshs[0]->entries.push_back(entry);

			if (!asset->meshs[0]->mat.size())
				asset->meshs[0]->mat.push_back(new Comp::Material());

			mesh.CalcAABB(asset->meshs[0]->minIn, asset->meshs[0]->maxIn);

			return asset;
		}

		GLuint Loader::GenVao() {

			GLuint VaoID;
			GLCall(glGenVertexArrays(1, &VaoID));
			GLCall(glBindVertexArray(VaoID));
			return VaoID;
		}

		GLuint Loader::StoreIndicies(const Vector<GLuint>& data) {

			GLuint VboID;
			GLCall(glGenBuffers(1, &VboID));
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboID));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW));
			return VboID;
		}

		GLuint Loader::CreateInstanceBuffer()
		{
			GLuint InstanceBufferID;
			GLCall(glGenBuffers(1, &InstanceBufferID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, InstanceBufferID));

			for (unsigned int i = 0; i < 4; i++) {
				glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::WORLDMAT + i);
				glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::WORLDMAT + i, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(mat4), (void*)(sizeof(GLfloat) * i * 4));
				glVertexAttribDivisor((uint)Comp::MESH_VB_TYPES::WORLDMAT + i, 1);

				glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::INVWORLDMAT + i);
				glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::INVWORLDMAT + i, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(mat4), (void*)(sizeof(GLfloat) * i * 4 + sizeof(mat4)));
				glVertexAttribDivisor((uint)Comp::MESH_VB_TYPES::INVWORLDMAT + i, 1);
			}

			return InstanceBufferID;
		}

		GLuint Loader::StoreData(const Vector<Comp::VertexData>& data)
		{
			GLuint VboID;
			GLCall(glGenBuffers(1, &VboID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Comp::VertexData), data.data(), GL_STATIC_DRAW));

			GLCall(glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::VERTEX));
			GLCall(glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexData), 0));

			GLCall(glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::NORMAL));
			GLCall(glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexData), (void*)(1 * sizeof(vec3))));

			GLCall(glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::TANGENT));
			GLCall(glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexData), (void*)(2 * sizeof(vec3))));

			GLCall(glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::COLOR));
			GLCall(glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexData), (void*)(3 * sizeof(vec3))));

			GLCall(glEnableVertexAttribArray((uint)Comp::MESH_VB_TYPES::TEXTURE));
			GLCall(glVertexAttribPointer((uint)Comp::MESH_VB_TYPES::TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexData), (void*)(4 * sizeof(vec3))));

			return VboID;
		}

		GLuint Loader::StoreBones(const Vector<Comp::VertexBoneData>& Bones)
		{
			GLuint VboID;
			GLCall(glGenBuffers(1, &VboID));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboID));
			GLCall(glBufferData(GL_ARRAY_BUFFER, Bones.size() * sizeof(Comp::VertexBoneData), Bones.data(), GL_STATIC_DRAW));

			GLCall(glEnableVertexAttribArray((uint)Comp::ANIM_VB_TYPES::BONEIDs));
			GLCall(glVertexAttribIPointer((uint)Comp::ANIM_VB_TYPES::BONEIDs, MAX_EFFECTING_BONES, GL_UNSIGNED_INT, sizeof(Comp::VertexBoneData), 0));
			
			GLCall(glEnableVertexAttribArray((uint)Comp::ANIM_VB_TYPES::WEIGHTs));
			GLCall(glVertexAttribPointer((uint)Comp::ANIM_VB_TYPES::WEIGHTs, MAX_EFFECTING_BONES, GL_FLOAT, GL_FALSE, sizeof(Comp::VertexBoneData), (void*)(sizeof(GLuint) * MAX_EFFECTING_BONES)));

			return VboID;
		}

		Image Loader::LoadImageFromFile(const String & path)
		{
			int width, height, channels;
			unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

			if (!data) {
				LOGERROR("TEXTURE LOADING FAILED : ", path);
				return Image();
			}
			else
				return { width, height, channels, data };
		}

		Comp::AssetMesh* Loader::LoadModel(const Comp::IndexModel & model)
		{
			GLuint VaoID = GenVao();
			GLuint VboID = StoreData(model.data);
			GLuint IboID = StoreIndicies(model.indices);
			GLuint InstanceID = CreateInstanceBuffer();
			
			Comp::AssetMesh* mesh = new Comp::AssetMesh(VaoID, VboID, IboID, model.name);
			mesh->instanceID = InstanceID;

			GLCall(glBindVertexArray(0));
			return mesh;
		}

		Comp::AnimatedMesh* Loader::LoadAnimModel(const Comp::AnimIndexModel & anim)
		{
			GLuint VaoID = GenVao();
			GLuint VboID = StoreData(anim.data);
			GLuint BboID = StoreBones(anim.bones);
			GLuint IboID = StoreIndicies(anim.indices);

			Comp::AnimatedMesh* mesh = new Comp::AnimatedMesh(VaoID, VboID, BboID, IboID, anim.name);

			GLCall(glBindVertexArray(0));
			return mesh;
		}
	}
}