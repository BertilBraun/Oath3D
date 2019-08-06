#pragma once

#include "Common/Common.h"

#include "ResourceHolder.h"

namespace Oath {
	namespace Comp {

		struct AssetMesh;
		struct AnimatedMesh;
		struct Texture;

		class ResManager {
		public:
			static AssetMesh*		GetAsset(const String& path);
			static AnimatedMesh*	GetAnim(const String& path);
			static Texture*			GetTexture(const String& path);
			static uint				GetAudio(const String& path);

			static void FreeAsset(const String & path);
			static void FreeAnim(const String & path);
			static void FreeTexture(const String & path);
			static void FreeAudio(const String & path);

		private:
			static ResManager& get();

			ResManager();

			ResourceHolder<AssetMesh*> assets;
			ResourceHolder<AnimatedMesh*> animations;
			ResourceHolder<Texture*> textures;
			ResourceHolder<uint> audios;
		};
	}
}