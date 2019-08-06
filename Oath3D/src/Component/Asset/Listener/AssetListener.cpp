#include "Common/Common.h"
#include "AssetListener.h"

#include "Component/Asset/Asset.h"
#include "System/Resource/ResourceManager.h"

Oath::Comp::AssetListener::AssetListener()
{
	addComponent(Asset::ID);
}

void Oath::Comp::AssetListener::onRemoveEntity(ECS::Entity * handle)
{
	Asset* asset = handle->getComponent<Asset>();
	for (auto name : asset->names)
		if (name != "")
			ResManager::FreeAsset(name);
}

void Oath::Comp::AssetListener::onMakeEntity(ECS::Entity * handle)
{
	Asset* asset = handle->getComponent<Asset>();
	ASSERT(asset->names[0] != "");

	for (int i = 0; i < 4; i++)
		if (asset->names[i] != "") {
			asset->meshs[i] = ResManager::GetAsset(asset->names[i]);
			asset->meshs[i]->flags |= asset->flags;
		}
}

void Oath::Comp::AssetListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	Asset* asset = handle->getComponent<Asset>();
	ASSERT(asset->names[0] != "");

	for (int i = 0; i < 4; i++)
		if (asset->names[i] != "") {
			asset->meshs[i] = ResManager::GetAsset(asset->names[i]);
			asset->meshs[i]->flags |= asset->flags;
		}
}

void Oath::Comp::AssetListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	Asset* asset = handle->getComponent<Asset>();
	for (auto name : asset->names)
		if (name != "")
			ResManager::FreeAsset(name);
}
