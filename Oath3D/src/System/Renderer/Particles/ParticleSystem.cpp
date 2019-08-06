#include "Common/Common.h"
#include "ParticleSystem.h"

#include "System/Renderer/Shader/Shader.h"
#include "System/Resource/ResourceManager.h"
#include "System/Renderer/MasterRenderer.h"

namespace Oath {
	namespace Render {

		void ParticleEmitter::Update(float dt, const vec3& attractorPosition) const
		{
			compute->start();
			compute->setUniform("dt", dt);
			compute->setUniform("attractorActive", true);
			compute->setUniform("attractorPosition", attractorPosition);

			glDispatchCompute(amtOfParticles >> 6, 1, 1);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			shader->start();
			shader->updateCamera(Comp::GCam);

			texture->Bind();

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glDisable(GL_DEPTH_TEST);

			glDrawArrays(GL_POINTS, 0, amtOfParticles);
		}

		ParticleSystem::ParticleSystem()
		{
			addComponent(ParticleEmitter::ID);
		}

		void ParticleSystem::updateComponents(float delta, ECS::Entity * entity, const Vector<ECS::BaseComponent*>& entityComp)
		{
			Render::GRenderer->process(entity->getComponent<ParticleEmitter>());
		}

		ParticleListener::ParticleListener()
		{
			addComponent(ParticleEmitter::ID);
		}

		void ParticleListener::onMakeEntity(ECS::Entity * handle)
		{
			add(handle);
		}

		void ParticleListener::onRemoveEntity(ECS::Entity * handle)
		{
			remove(handle);
		}

		void ParticleListener::onAddComponent(ECS::Entity * handle, uint32 id)
		{
			add(handle);
		}

		void ParticleListener::onRemoveComponent(ECS::Entity * handle, uint32 id)
		{
			remove(handle);
		}

		enum class Bindings { PositionBuffer = 0, VelocityBuffer = 1 };
		enum class AttrLoc { Position = 0, Velocity = 1 };

		void ParticleListener::add(ECS::Entity * e)
		{
			ParticleEmitter* em = e->getComponent<ParticleEmitter>();

			std::vector<vec4> positions;
			positions.reserve(em->amtOfParticles);
			for (float x = 0; x < (1u << 7); ++x)
				for (float y = 0; y < (1u << 7); ++y)
					for (float z = 0; -z < (1u << 7); --z)
						positions.push_back(vec4{ x, y, z, 1.f });
			std::vector<vec4> velocities(em->amtOfParticles, vec4(0));

			glCreateBuffers(1, &em->posBuffer);
			glCreateBuffers(1, &em->velBuffer);

			glNamedBufferStorage(em->posBuffer, positions.size() * sizeof(vec4), positions.data(), GL_DYNAMIC_STORAGE_BIT);
			glNamedBufferStorage(em->velBuffer, velocities.size() * sizeof(vec4), velocities.data(), GL_DYNAMIC_STORAGE_BIT);
			positions.clear();
			velocities.clear();

			em->compute = Render::Shader::GetShader("Particle/Particle");
			em->shader = Render::Shader::GetShader("Particle/Particle", "Particle/Particle", "Particle/Particle");
			em->shader->setUniform("DiffuseTexture", 0);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)Bindings::PositionBuffer, em->posBuffer);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, (GLuint)Bindings::VelocityBuffer, em->velBuffer);

			glGenVertexArrays(1, &em->vao);
			glBindVertexArray(em->vao);

			glBindBuffer(GL_ARRAY_BUFFER, em->posBuffer);
			glVertexAttribPointer((GLuint)AttrLoc::Position, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4::value_type), (void *)0);
			glEnableVertexAttribArray((GLuint)AttrLoc::Position);
			glBindBuffer(GL_ARRAY_BUFFER, em->velBuffer);
			glVertexAttribPointer((GLuint)AttrLoc::Velocity, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4::value_type), (void *)0);
			glEnableVertexAttribArray((GLuint)AttrLoc::Velocity);

			em->texture = Comp::ResManager::GetTexture(Settings::TEXTURE_PATH + "grey.png");
		}

		void ParticleListener::remove(ECS::Entity * e)
		{
			ParticleEmitter* em = e->getComponent<ParticleEmitter>();

			glDeleteBuffers(1, &em->posBuffer);
			glDeleteBuffers(1, &em->velBuffer);
			glDeleteVertexArrays(1, &em->vao);
		}
	}
}