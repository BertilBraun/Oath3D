#pragma once

#include "ExampleBase.h"

namespace Oath {

	class SoundExample : public ExampleBase {
	public:
		ECS::Entity* emitter;

		SoundExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset asset("Hi-fi/hifi.obj");
			Comp::Emitter e("piano2.wav", true, 0.3, 0.8, 500);

			emitter = ecs.makeEntity(e, asset);
			emitter->pos(vec3(5));

			Comp::Asset sponza("Sponza/sponza.obj");
			ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}

		void onLeave() {

			emitter->getComponent<Comp::Emitter>()->setState(AL_STOPPED);

		}
	};
}