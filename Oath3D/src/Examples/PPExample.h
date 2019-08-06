#pragma once

#include "ExampleBase.h"
#include "PPEffectInverseColor.h"

namespace Oath {

	class PPExample : public ExampleBase {
	public:
		Render::PPEffectInverseColor* ppe;

		PPExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset sponza("Sponza/sponza.obj");
			ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

			ppe = new Render::PPEffectInverseColor();
			Render::GRenderer->process(ppe);
		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}
	};
}