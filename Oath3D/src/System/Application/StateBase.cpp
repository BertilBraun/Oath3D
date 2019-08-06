#include "Common/Common.h"
#include "StateBase.h"
#include "Application.h"

#include "Component/Sound/SoundSystems.h"

#include "Component/Camera/CameraSystem.h"
#include "Component/Camera/CameraListener.h"

#include "Component/Asset/Listener/AssetListener.h"
#include "Component/Asset/Listener/AnimationListener.h"

#include "System/Renderer/Skybox/Skybox.h"
#include "System/Renderer/Lights/LightSystems.h"
#include "System/Renderer/Systems/AssertRenderer.h"
#include "System/Renderer/Systems/AnimationRenderer.h"
#include "System/Renderer/Particles/ParticleSystem.h"

#include "Component/Control/BillboardSystem.h"

namespace Oath {
	namespace APP {

		StateBase::StateBase(APP::Application * app, APP::StateBase * parent)
			: app(app), parent(parent)
		{
			Render::GRenderer = &renderer;

			updatelist.addSystem(new Comp::CameraSystem());
			updatelist.addSystem(new Comp::BillboardSystem());
			updatelist.addSystem(new Comp::EmitterSystem());
			updatelist.addSystem(new Comp::RecieverSystem());

			rendering.addSystem(new Comp::AssertRenderer());
			rendering.addSystem(new Comp::AnimationRenderer());

			rendering.addSystem(new Render::ParticleSystem());
			rendering.addSystem(new Render::PointLightSystem());
			rendering.addSystem(new Render::SpotLightSystem());

			world = new Physic::World(app->dispatcher);

			ecs.addListener(world);

			ecs.addListener(new Render::ParticleListener());
			ecs.addListener(new Render::AmbientLightListener());
			ecs.addListener(new Render::DirectionalLightListener());
			ecs.addListener(new Render::PointLightListener());
			ecs.addListener(new Render::SpotLightListener());
			ecs.addListener(new Render::SkyboxListener());

			ecs.addListener(new Comp::CameraListener());
			ecs.addListener(new Comp::EmitterListener());
			ecs.addListener(new Comp::RecieverListener());

			ecs.addListener(new Comp::AssetListener());
			ecs.addListener(new Comp::AnimationListener());
		}
	}
}