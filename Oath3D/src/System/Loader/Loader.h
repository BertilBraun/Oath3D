#pragma once

#include "Common/Common.h"

#include "Component/Asset/Model/Asset/AssetMesh.h"
#include "Component/Asset/Model/Animation/AnimatedMesh.h"

namespace Oath {

	namespace Comp {

		struct Asset;
		struct AssetMesh;
		struct AnimatedMesh;

		struct Texture;

		struct VertexData;

		struct IndexModel;
		struct AnimIndexModel;
	}

	namespace System {

		struct Image {
			int width, height, channels;
			unsigned char *data = nullptr;
		};

		class Loader {
		public:
			static Comp::AssetMesh*		LoadAsset(const String& path);
			static Comp::AnimatedMesh*	LoadAnim(const String& path);
			static Comp::Texture*		LoadTexture(const String& file);
			static uint					LoadAudio(const String& file);
			static Comp::Texture*		LoadCubeMap(const String& file, const String& ext = ".png");

			static void FreeAsset(Comp::AssetMesh* asset);
			static void FreeAnim(Comp::AnimatedMesh* anim);
			static void FreeTexture(Comp::Texture* texture);
			static void FreeAudio(uint buffer);

			static Comp::Asset*	LoadAsset(const Comp::IndexModel & mesh);

		private:
			static GLuint GenVao();

			static GLuint StoreIndicies(const Vector<GLuint>& data);

			static GLuint CreateInstanceBuffer();
			static GLuint StoreData(const Vector<Comp::VertexData>& data);
			static GLuint StoreBones(const Vector<Comp::VertexBoneData> & Bones);

			static Image		LoadImageFromFile(const String& path);

			static Comp::AssetMesh*	LoadModel(const Comp::IndexModel & mesh);
			static Comp::AnimatedMesh*LoadAnimModel(const Comp::AnimIndexModel& anim);

			Loader() { }
		};
	}
}