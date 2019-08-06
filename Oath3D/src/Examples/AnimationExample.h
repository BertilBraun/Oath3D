#pragma once

#include "ExampleBase.h"

namespace Oath {

	class AnimationExample : public ExampleBase {
	public:
		AnimationExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset sponza("Sponza/sponza.obj");
			ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

			Comp::AnimatedAsset anim("CharacterRunning.dae");
			ecs.makeEntity(anim)->scale(vec3(3)).rot(vec3(0, 90, 180)).pos(vec3(20, 0, 0));

		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}

	};
}