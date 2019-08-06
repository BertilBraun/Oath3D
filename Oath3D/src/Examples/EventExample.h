#pragma once

#include "ExampleBase.h"
#include "Component/Physik/Collider.h"

namespace Oath {

	class EventExample : public ExampleBase {
	public:
		EventExample(APP::Application* app, APP::StateBase* parent) :
			ExampleBase(app, parent)
		{
			Comp::Asset earth("Earth/earth.obj", Comp::Asset::DONT_RENDER_MESH1);
			Physic::Collider collider(35);
			ecs.makeEntity(earth, collider)->scale(vec3(0.1)).rot(vec3(180, 0, 0));
	   
			cameraEntity->pos(vec3(50, 0, 0));

			app->dispatcher.AddEventSubscriber("COLLISION", EVENT_CLASS_FUNC_CAST(EventExample::EventFunc));
		}

		static void EventFunc(void* sender, void* data) {

			LOG("COLLISSION ACCURED");

		}

		void update(float deltaTime) override {

			CheckForNewState();
		}

		void render(float deltaTime) override {

		}

	};
}