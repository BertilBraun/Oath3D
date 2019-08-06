#include "Common/Common.h"
#include "InteractionWorld.h"

#include "System/ECS/FLAGS.h"
#include "Component/Camera/Camera.h"
#include "System/System/ToggleKey.h"

#include "Collider.h"
#include "RigidBody.h"

namespace Oath {

	namespace Physic {

		World::World(Event::Dispatcher& events) : events(events)
		{
			addComponent(Collider::ID, ECS::FLAG_OPTIONAL);
			addComponent(RigidBody::ID, ECS::FLAG_OPTIONAL);

			configuration	= new btDefaultCollisionConfiguration();
			dispatcher		= new btCollisionDispatcher(configuration);
			broadphase		= new btDbvtBroadphase();
			solver			= new btSequentialImpulseConstraintSolver();
			dynamicsWorld	= new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, configuration);

			draw = new Render::BulletDebugDraw();
			draw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

			dynamicsWorld->setDebugDrawer(draw);
			dynamicsWorld->setGravity(btVector3(0, -Settings::GRAVITY, 0)); 
		}

		World::~World()
		{
			delete draw;
			delete dynamicsWorld;
			delete solver;
			delete broadphase;
			delete dispatcher;
			delete configuration;
		}

		void World::onMakeEntity(ECS::Entity* entity)
		{
			addEntity(entity);
		}

		void World::onRemoveEntity(ECS::Entity* entity)
		{
			Collider*	collider = entity->getComponent<Collider>();
			RigidBody*	rigidBody = entity->getComponent<RigidBody>();

			if (!collider || !rigidBody)
				return;

			dynamicsWorld->removeRigidBody(rigidBody->body);

			SAVE_DELETE(collider->collider);
			SAVE_DELETE(rigidBody->body);
		}

		void World::onAddComponent(ECS::Entity* entity, uint32 id)
		{
			if (id != Collider::ID && id != RigidBody::ID)
				return;

			addEntity(entity);
		}

		void World::onRemoveComponent(ECS::Entity* entity, uint32 id)
		{
			if (id == Collider::ID) {
				if (entity->getComponent<RigidBody>()->addedFromWorld)
					onRemoveEntity(entity);
				else {
					Collider coll(0.0f);
					coll.addedFromWorld = true;
					entity->addComponent(coll);
					return;
				}
			}

			else if (id == RigidBody::ID) {
				if (entity->getComponent<Collider>()->addedFromWorld)
					onRemoveEntity(entity);
				else {
					RigidBody body(0.0f);
					body.addedFromWorld = true;
					entity->addComponent(body);
					return;
				}
			}
		}
		
		void World::addEntity(ECS::Entity * entity)
		{
			Collider*	collider = entity->getComponent<Collider>();
			RigidBody*	rigidBody = entity->getComponent<RigidBody>();

			if (!collider && !rigidBody)
				return;

			if (!collider) {
				Collider coll(0.0f);
				coll.addedFromWorld = true;
				entity->addComponent(coll);
				return;
			}

			if (!rigidBody) {
				RigidBody body(0.0f);
				body.addedFromWorld = true;
				entity->addComponent(body);
				return;
			}

			if (collider && rigidBody && !rigidBody->body) {

				btVector3 localInertia(0, 0, 0);
				if (rigidBody->info.m_mass != 0.f)
					collider->collider->calculateLocalInertia(rigidBody->info.m_mass, localInertia);

				rigidBody->info.m_collisionShape = collider->collider;
				rigidBody->info.m_localInertia = localInertia;
				rigidBody->info.m_motionState = &entity->transform;

				rigidBody->body = new btRigidBody(rigidBody->info);
				rigidBody->body->setUserPointer(entity);

				dynamicsWorld->addRigidBody(rigidBody->body);
			}
			else if (!rigidBody->body)
				LOGERROR("FAILED TO ADD ENTITY TO PHYSIK WORLD");
		}

		void World::processInteractions(float delta)
		{
			dynamicsWorld->stepSimulation(delta);
			
			for (int i : range(dispatcher->getNumManifolds())) {
				const btPersistentManifold *mf = dispatcher->getManifoldByIndexInternal(i);
				Vector<const btCollisionObject*> data { mf->getBody0(), mf->getBody1() };

				if (!(data[0]->isStaticObject() && data[1]->isStaticObject()))
					events.DispatchEvent("COLLISION", &data);
			}
		}

		void World::debugDraw()
		{
			dynamicsWorld->debugDrawWorld();
		}
	}
}