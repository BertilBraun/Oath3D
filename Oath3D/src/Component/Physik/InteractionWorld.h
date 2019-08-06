#pragma once

#include "System/ECS/Listener.h"

#include "Bullet/btBulletDynamicsCommon.h"

#include "System/Events/Dispatcher.h"
#include "System/Renderer/Debug/BulletDebugDraw.h"

namespace Oath {
	namespace Physic {

		class World : public ECS::Listener {
		public:
			World(Event::Dispatcher& events);
			~World();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

			void processInteractions(float delta);
			void debugDraw();

		private:
			void addEntity(ECS::Entity* entity);

			btDefaultCollisionConfiguration		* configuration;
			btCollisionDispatcher				* dispatcher;
			btBroadphaseInterface				* broadphase;
			btSequentialImpulseConstraintSolver	* solver;
			btDiscreteDynamicsWorld				* dynamicsWorld;

			Render::BulletDebugDraw* draw;
			Event::Dispatcher& events;
		};

	}
}