#include "Common/Common.h"
#include "AnimationListener.h"

#include "Component/Asset/AnimatedAsset.h"
#include "System/Resource/ResourceManager.h"

Oath::Comp::AnimationListener::AnimationListener()
{
	addComponent(AnimatedAsset::ID);
}

void Oath::Comp::AnimationListener::onRemoveEntity(ECS::Entity * handle)
{
	ResManager::FreeAnim(handle->getComponent<AnimatedAsset>()->name);
}

void Oath::Comp::AnimationListener::onMakeEntity(ECS::Entity * handle)
{
	AnimatedAsset* asset = handle->getComponent<AnimatedAsset>();
	ASSERT(asset->name != "");
	asset->mesh = ResManager::GetAnim(asset->name);
	asset->mesh->flags |= asset->flags;
}

void Oath::Comp::AnimationListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	AnimatedAsset* asset = handle->getComponent<AnimatedAsset>();
	ASSERT(asset->name != "");
	asset->mesh = ResManager::GetAnim(asset->name);
	asset->mesh->flags |= asset->flags;
}

void Oath::Comp::AnimationListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	ResManager::FreeAnim(handle->getComponent<AnimatedAsset>()->name);
}
