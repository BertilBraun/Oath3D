#pragma once

#include "ExampleBase.h"
#include "PPDisplayDeferred.h"

#include "ImGui/Include.h"

namespace Oath {

	class DeferredExample : public ExampleBase {
	public:
		Render::PPDisplayDeferred* ppdd;

		DeferredExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset sponza("Sponza/sponza.obj");
			ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

			ppdd = new Render::PPDisplayDeferred();
			Render::GRenderer->process(ppdd);
		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

			ImGui::Begin("ALBEDO");
			ImGui::End();

			ImGui::Begin("POSITION");
			ImGui::End();

			ImGui::Begin("NORMAL");
			ImGui::End();

			ImGui::Begin("SPECULAR");
			ImGui::End();
		}

	};
}
