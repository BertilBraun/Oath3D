#pragma once

#include "System/ECS/Include.h"

#include "System/Renderer/MasterRenderer.h"
#include "Component/Physik/InteractionWorld.h"

namespace Oath {
	namespace APP {
		class Application;

		class StateBase {
		public:
			StateBase(APP::Application* app, APP::StateBase* parent = nullptr);

			virtual ~StateBase() = default;

			virtual void update(float deltaTime) = 0;
			virtual void render(float deltaTime) = 0;

			virtual void onOpen() {};
			virtual void onClose() {};
			virtual void onLeave() {};
			virtual void onResume() {};

		public:
			Physic::World* world;
			ECS::Manager ecs;
			ECS::SystemList rendering, updatelist;
			Render::MasterRenderer renderer;
			APP::Application* app;
			APP::StateBase* parent;
		};

	}
}