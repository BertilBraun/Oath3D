#include "Common/Common.h"
#include "ResourceManager.h"

#include "System/Loader/Loader.h"

namespace Oath {
	namespace Comp {

		AssetMesh * ResManager::GetAsset(const String & path)
		{
			return get().assets.getData(path);
		}

		AnimatedMesh * ResManager::GetAnim(const String & path)
		{
			return get().animations.getData(path);
		}

		Texture * ResManager::GetTexture(const String & path)
		{
			return get().textures.getData(path);
		}

		uint ResManager::GetAudio(const String & path)
		{
			return get().audios.getData(path);
		}

		void ResManager::FreeAsset(const String & path)
		{
			get().assets.freeData(path);
		}

		void ResManager::FreeAnim(const String & path)
		{
			get().animations.freeData(path);
		}

		void ResManager::FreeTexture(const String & path)
		{
			//get().textures.freeData(path);
		}

		void ResManager::FreeAudio(const String & path)
		{
			get().audios.freeData(path);
		}

		ResManager & ResManager::get() {
			static ResManager instance;
			return instance;
		}

		ResManager::ResManager() :
			assets(&System::Loader::LoadAsset, &System::Loader::FreeAsset),
			animations(&System::Loader::LoadAnim, &System::Loader::FreeAnim),
			textures(&System::Loader::LoadTexture, &System::Loader::FreeTexture),
			audios(&System::Loader::LoadAudio, &System::Loader::FreeAudio)
		{ }
	}
}
