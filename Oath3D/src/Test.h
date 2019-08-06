#pragma once

#include "System/Application/StateBase.h"
#include "System/Application/Application.h"

#include "System/Renderer/Lights/Lights.h"

#include "Component/Camera/Camera.h"
#include "Component/Camera/FreeLook.h"
#include "Component/Camera/FreeMove.h"

#include "Component/Asset/Asset.h"
#include "Component/Asset/AnimatedAsset.h"

#include "ImGui/Include.h"

#include "System/Events/Dispatcher.h"
#include "System/Events/Subscriber.h"

#include "Component/Physik/Collider.h"
#include "Component/Physik/RigidBody.h"

#include "System/System/Debug.h"

#include "Component/Sound/Sound.h"

#include "System/Renderer/Skybox/Skybox.h"

#include "System/Renderer/Particles/ParticleSystem.h"
#include "System/Renderer/PostProcessing/SSR.h"

namespace Oath {

	using namespace Comp;
	using namespace Physic;
	using namespace Render; 

	class TestState : public APP::StateBase {
	public:
		TestState(APP::Application* app, APP::StateBase* parent = nullptr) : 
			APP::StateBase(app, parent)
		{
			//Asset groundasset("cube.obj");
			//Asset asset("Earth/earth.obj");
			//Collider groundcoll(vec3(50, 5, 50));
		
			//ECS::Entity* entity = ecs.makeEntity(groundasset, groundcoll)->transform.setScale(vec3(100, 10, 100)).setPos(vec3(0, -10, 0));
			//ECS::Entity* entity2 = ecs.makeEntity(asset)->transform.setPos(vec3(500)).setScale(vec3(0.1)).setRot(angleAxis(90.0f, vec3(0, 0, 1)));
			//entity->addChild(entity2);

			//EVENT::Subscriber* sub = new EVENT::Subscriber(this, EVENT_FUNC_CAST(&TestFunc));
			//dispatcher.AddEventSubscriber("TEST", sub);
			//dispatcher.AddEventSubscriber("COLLISION", this, &TestState::EventTestFunc);

			//Emitter e("piano2.wav", true, 0.1f);
			//ecs.makeEntity(e)->pos(vec3(8));

			Skybox sky("Skybox/");
			ecs.makeEntity(sky);

			//ParticleEmitter em { };
			//ecs.makeEntity(em);

			AmbientLight alight(0.1f);
			ecs.makeEntity(alight);
			
			DirectionalLight dlight(vec3(1), 0.1f, 0.9f, vec3(0.2, -0.8, -0.2));
			//dlight.Base.shadowType = Render::ShadowMappingType::NO;
			ecs.makeEntity(dlight);
			
			PointLight pLight(vec3(1, 0, 0), 1, 1.2f, 0, 0, 0.3f);
			ecs.makeEntity(pLight)->pos(vec3(0, 5, 0));
			
			SpotLight sLight(vec3(0, 0, 1), 2, 2.2f, 0.5, 0.5, 0.5, vec3(0, 0, 1), 20);
			ecs.makeEntity(sLight);
			
			Asset asset("Earth/earth.obj", Asset::DONT_RENDER_MESH1);
			//ecs.makeEntity(asset)->pos(vec3(10)).scale(vec3(0.01f));

			Asset sponza("Sponza/sponza.obj");
			ecs.makeEntity(sponza)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

			AnimatedAsset guard("Guard/boblampclean.md5mesh");
			//ecs.makeEntity(guard)->scale(vec3(0.1)).rot(vec3(0, 0, 180));

			AnimatedAsset anim("Runner/CharacterRunning.dae");
			ecs.makeEntity(anim)/*->pos(vec3(10, 0, 0)).rot(vec3(0, 0, 90))*/;

			//Render::GRenderer->process(new SSR());

			Camera cam(true);
			Reciever r;
			Comp::FreeMove move(5);
			FreeLook look;
			look.rot = vec2(50, 30);
			ecs.makeEntity(cam, look, move, r);
		}

		static void EventTestFunc(void* sender, void* data) {

			//INTERACTION WORLD COLLITION TEST
			auto objects = (Vector<const btCollisionObject*>*)data;
			
		}

		void update(float deltaTime) override {

			//dispatcher.DispatchEvent("TEST", this);
		}

		vec3 p, f, u;

		void render(float deltaTime) override {

			ImGui::Begin("WINDOW");

			u = Comp::GCam->up;
			ImGui::SliderFloat3("UP", glm::value_ptr(u), 0, 1);

			p = Comp::GCam->pos();
			ImGui::SliderFloat3("POS", glm::value_ptr(p), -50, 50);

			f = Comp::GCam->forward;
			ImGui::SliderFloat3("TARGET", glm::value_ptr(f), 0, 1);

			ImGui::End();

			System::Debug::render(deltaTime);
		}

	};
}