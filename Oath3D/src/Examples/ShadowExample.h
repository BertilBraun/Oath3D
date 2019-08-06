#pragma once

#include "ExampleBase.h"

namespace Oath {

	class ShadowExample : public ExampleBase {
	public:
		ShadowExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Render::GRenderer->directionalLight->Base.shadowType = Render::ShadowMappingType::DYNAMIC;
			Render::GRenderer->directionalLight->direction = vec3(0.3, 0.3, 0.3);

			Comp::Asset asset("Small Tropical Island/Small Tropical Island.obj");
			ecs.makeEntity(asset)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}
	};
}