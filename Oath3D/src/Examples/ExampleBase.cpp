#include "Common/Common.h"
#include "ExampleBase.h"

#include "Examples/SoundExample.h"
#include "Examples/PPExample.h"
#include "Examples/AnimationExample.h"
#include "Examples/DeferredExample.h"
#include "Examples/ParentingExample.h"
#include "Examples/EventExample.h"
#include "Examples/ShadowExample.h"

#include "System/Application/Application.h"
#include "System/Window/Window.h"

namespace Oath {

	ExampleBase::ExampleBase(APP::Application* app, APP::StateBase* parent) :
		APP::StateBase(app, parent)
	{
		Render::Skybox sky("Skybox/");
		ecs.makeEntity(sky);

		Render::AmbientLight alight(1.f);
		ecs.makeEntity(alight);

		Render::DirectionalLight dlight(vec3(1), 0.5f, 1.5f, vec3(0.2, -0.8, -0.2));
		dlight.Base.shadowType = Render::ShadowMappingType::NO;
		ecs.makeEntity(dlight);

		Comp::Camera cam(true);
		Comp::Reciever r;
		Comp::FreeMove move(5);
		Comp::FreeLook look;
		cameraEntity = ecs.makeEntity(cam, look, move, r);

		cameraEntity->pos(vec3(0, 1, 0));
	}

	void ExampleBase::CheckForNewState()
	{
		static String last = "";

		if (Key::isKeyPressed(Key::KEY_1) && last != "PP") {
			last = "PP";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<PPExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_2) && last != "Sound") {
			last = "Sound";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<SoundExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_3) && last != "Anim") {
			last = "Anim";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<AnimationExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_4) && last != "Deferred") {
			last = "Deferred";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<DeferredExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_5) && last != "Parent") {
			last = "Parent";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<ParentingExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_6) && last != "Event") {
			last = "Event";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<EventExample>(this);
		}
		if (Key::isKeyPressed(Key::KEY_7) && last != "Shadow") {
			last = "Shadow";
			LOG("NEW STATE SET: ", last);
			glfwSetWindowTitle(app->getWindow(), last.c_str());
			app->pushState<ShadowExample>(this);
		}
	}

}