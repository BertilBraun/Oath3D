#pragma once

#include "ExampleBase.h"

namespace Oath {

	class ParentingExample : public ExampleBase {
	public:
		ECS::Entity* center;
		ECS::Entity* orbit;

		ParentingExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset earth("Earth/earth.obj", Comp::Asset::DONT_RENDER_MESH1);
			Comp::AnimatedAsset anim("CharacterRunning.dae");

			center = ecs.makeEntity(earth);
			orbit = ecs.makeEntity(anim);

			center->scale(vec3(0.1f));
			orbit->scale(vec3(20)).pos(vec3(350, 0, 0)).rot(vec3(-90, 0, 0));

			center->add(orbit);

			cameraEntity->pos(vec3(50, 0, 0));
		}

		void update(float deltaTime) override {

			center->transform.rotate(quat(radians(vec3(20 * deltaTime))));

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}

	};
}