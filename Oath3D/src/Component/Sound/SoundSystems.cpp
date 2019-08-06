#include "Common/Common.h"

#include "Sound.h"
#include "SoundSystems.h"

#include "Component/Camera/Camera.h"
#include "Component/Physik/RigidBody.h"

#include "System/Resource/ResourceManager.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"

namespace Oath {
	namespace Comp {
		
		EmitterSystem::EmitterSystem() {
			addComponent(Emitter::ID);
		}

		void EmitterSystem::updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) {
			auto e = entity->getComponent<Emitter>();
			auto r = entity->getComponent<Physic::RigidBody>();

			vec3 p = entity->pos();
			ALCall(alSource3f(e->source, AL_POSITION, p.x, p.y, p.z));

			if (r) {
				auto v = r->body->getLinearVelocity();
				ALCall(alSource3f(e->source, AL_VELOCITY, v.x(), v.y(), v.z()));
			}

			ALint source_state;
			alGetSourcei(e->source, AL_SOURCE_STATE, &source_state);

			if (source_state == AL_INITIAL)
				e->setState(AL_PLAYING);
		}

		EmitterListener::EmitterListener() {
			addComponent(Emitter::ID);
		}

		void EmitterListener::onMakeEntity(ECS::Entity* handle) {
			add(handle);
		}

		void EmitterListener::onRemoveEntity(ECS::Entity* handle) {
			remove(handle);
		}

		void EmitterListener::onAddComponent(ECS::Entity* handle, uint32 id) {
			add(handle);
		}

		void EmitterListener::onRemoveComponent(ECS::Entity* handle, uint32 id) {
			remove(handle);
		}

		void EmitterListener::remove(ECS::Entity* entity) {
			auto e = entity->getComponent<Emitter>();
			alDeleteSources(1, &e->source);

			if (e->path != "")
				ResManager::FreeAudio(e->path);
		}
		void EmitterListener::add(ECS::Entity* entity) {
			auto e = entity->getComponent<Emitter>();

			if (e->path == "") {
				LOGERROR("CAN NOT LOAD SOURCE WITH EMPTY PATH");
				return;
			}

			ALCall(alGenSources(1, &e->source));

			ALCall(alSourcef(e->source, AL_ROLLOFF_FACTOR, e->rolloff));
			
			ALCall(alSourcef(e->source, AL_PITCH, e->pitch));
			ALCall(alSourcef(e->source, AL_GAIN, e->gain));
			
			vec3 p = entity->pos();
			ALCall(alSource3f(e->source, AL_POSITION, p.x, p.y, p.z));
			
			ALCall(alSourcei(e->source, AL_LOOPING, e->looping));

			ALCall(alSourcei(e->source, AL_BUFFER, ResManager::GetAudio(e->path)));
		}

		RecieverSystem::RecieverSystem() {
			addComponent(Reciever::ID);
		}

		void RecieverSystem::updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) {
			auto c = entity->getComponent<Camera>();
			auto r = entity->getComponent<Physic::RigidBody>();

			vec3 p = entity->pos();
			ALCall(alListener3f(AL_POSITION, p.x, p.y, p.z));
			
			if (c) {
				ALfloat d[6]{ c->forward.x, c->forward.y, c->forward.z, c->up.x, c->up.y, c->up.z };
				ALCall(alListenerfv(AL_ORIENTATION, d));
			}
			else {
				ALfloat d[6]{ 0, 0, -1, 0, 1, 0 };
				ALCall(alListenerfv(AL_ORIENTATION, d));
			}

			if (r) {
				auto v = r->body->getLinearVelocity();
				ALCall(alListener3f(AL_VELOCITY, v.x(), v.y(), v.z()));
			}
			else 
				ALCall(alListener3f(AL_VELOCITY, 0, 0, 0));
		}

		RecieverListener::RecieverListener() {
			addComponent(Reciever::ID);
		}

		void RecieverListener::onMakeEntity(ECS::Entity* handle) {
			add(handle);
		}

		void RecieverListener::onRemoveEntity(ECS::Entity* handle) {
			remove(handle);
		}

		void RecieverListener::onAddComponent(ECS::Entity* handle, uint32 id) {
			add(handle);
		}

		void RecieverListener::onRemoveComponent(ECS::Entity* handle, uint32 id) {
			remove(handle);
		}

		void RecieverListener::remove(ECS::Entity* entity) {
			added = false;
		}
		void RecieverListener::add(ECS::Entity* entity) {
			if (!added)
				added = true;
			else
				LOGERROR("ALLREADY ADDED A RECIEVER!");
		}
	}
}