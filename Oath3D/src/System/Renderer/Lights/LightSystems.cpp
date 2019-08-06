#include "Common/Common.h"
#include "LightSystems.h"
#include "System/Renderer/MasterRenderer.h"
#include "System/Application/Application.h"

Oath::Render::AmbientLightListener::AmbientLightListener()
{
	addComponent(AmbientLight::ID);
}

void Oath::Render::AmbientLightListener::onMakeEntity(ECS::Entity * handle)
{
	if (!GRenderer->process(handle->getComponent<AmbientLight>()))
		LOGWARN("AMBIENT LIGHT WAS ALLREADY DEFINED: CAN ONLY HAVE ONE AMBIENT LIGHT PER RENDERER");
}

void Oath::Render::AmbientLightListener::onRemoveEntity(ECS::Entity * handle)
{
	GRenderer->process((AmbientLight*)nullptr);
}

void Oath::Render::AmbientLightListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	if (!GRenderer->process(handle->getComponent<AmbientLight>()))
		LOGWARN("AMBIENT LIGHT WAS ALLREADY DEFINED: CAN ONLY HAVE ONE AMBIENT LIGHT PER RENDERER");
}

void Oath::Render::AmbientLightListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	GRenderer->process((AmbientLight*)nullptr);
}

Oath::Render::DirectionalLightListener::DirectionalLightListener()
{
	addComponent(DirectionalLight::ID);
}

void Oath::Render::DirectionalLightListener::onMakeEntity(ECS::Entity * handle)
{
	DirectionalLight* l = handle->getComponent<DirectionalLight>();
	for (int i : range(Settings::NUM_CASCADES))
		l->shadow.emplace_back(new ShadowRenderer(Settings::SHADOW_MAP_SIZE, Settings::SHADOW_MAP_SIZE));
	l->box = new ShadowBox();

	if (!GRenderer->process(l))
		LOGWARN("DIRECTIONAL LIGHT WAS ALLREADY DEFINED: CAN ONLY HAVE ONE DIRECTIONAL LIGHT PER RENDERER");
}

void Oath::Render::DirectionalLightListener::onRemoveEntity(ECS::Entity * handle)
{
	DirectionalLight* l = handle->getComponent<DirectionalLight>();
	for (auto s : l->shadow)
		SAVE_DELETE(s);
	SAVE_DELETE(l->box);

	GRenderer->process((DirectionalLight*)nullptr);
}

void Oath::Render::DirectionalLightListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	DirectionalLight* l = handle->getComponent<DirectionalLight>();
	for (int i : range(Settings::NUM_CASCADES))
		l->shadow.emplace_back(new ShadowRenderer(Settings::SHADOW_MAP_SIZE, Settings::SHADOW_MAP_SIZE));
	l->box = new ShadowBox();

	if (!GRenderer->process(l))
		LOGWARN("DIRECTIONAL LIGHT WAS ALLREADY DEFINED: CAN ONLY HAVE ONE DIRECTIONAL LIGHT PER RENDERER");
}

void Oath::Render::DirectionalLightListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	DirectionalLight* l = handle->getComponent<DirectionalLight>();
	for (auto s : l->shadow)
		SAVE_DELETE(s);
	SAVE_DELETE(l->box);

	GRenderer->process((DirectionalLight*)nullptr);
}

Oath::Render::PointLightSystem::PointLightSystem()
{
	addComponent(PointLight::ID);
}

void Oath::Render::PointLightSystem::updateComponents(float delta, ECS::Entity * entity, const Vector<ECS::BaseComponent*>& entityComp)
{
	((PointLight*)entityComp[0])->Update();
	GRenderer->process((PointLight*)entityComp[0]);
}

Oath::Render::SpotLightSystem::SpotLightSystem()
{
	addComponent(SpotLight::ID);
}

void Oath::Render::SpotLightSystem::updateComponents(float delta, ECS::Entity * entity, const Vector<ECS::BaseComponent*>& entityComp)
{
	((SpotLight*)entityComp[0])->Update();
	GRenderer->process((SpotLight*)entityComp[0]);
}

Oath::Render::PointLightListener::PointLightListener()
{
	addComponent(PointLight::ID);
}

void Oath::Render::PointLightListener::onMakeEntity(ECS::Entity * handle)
{
	PointLight* l = handle->getComponent<PointLight>();
	for (int i : range(6))
		l->shadow[i] = new ShadowRenderer(l->w, l->h);
}

void Oath::Render::PointLightListener::onRemoveEntity(ECS::Entity * handle)
{
	PointLight* l = handle->getComponent<PointLight>();
	for (int i : range(6))
		SAVE_DELETE(l->shadow[i]);
}

void Oath::Render::PointLightListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	PointLight* l = handle->getComponent<PointLight>();
	for (int i : range(6))
		l->shadow[i] = new ShadowRenderer(l->w, l->h);
}

void Oath::Render::PointLightListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	PointLight* l = handle->getComponent<PointLight>();
	for (int i : range(6))
		SAVE_DELETE(l->shadow[i]);
}

Oath::Render::SpotLightListener::SpotLightListener()
{
	addComponent(SpotLight::ID);
}

void Oath::Render::SpotLightListener::onMakeEntity(ECS::Entity * handle)
{
	SpotLight* l = handle->getComponent<SpotLight>();
	l->shadow = new ShadowRenderer(l->w, l->h);
}

void Oath::Render::SpotLightListener::onRemoveEntity(ECS::Entity * handle)
{
	SpotLight* l = handle->getComponent<SpotLight>();
	SAVE_DELETE(l->shadow);
}

void Oath::Render::SpotLightListener::onAddComponent(ECS::Entity * handle, uint32 id)
{
	SpotLight* l = handle->getComponent<SpotLight>();
	l->shadow = new ShadowRenderer(l->w, l->h);
}

void Oath::Render::SpotLightListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
{
	SpotLight* l = handle->getComponent<SpotLight>();
	SAVE_DELETE(l->shadow);
}
