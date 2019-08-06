#pragma once

#include "Common/Common.h"
#include "System/ECS/Include.h"
#include "Component/Asset/Model/Texture.h"

namespace Oath {
	namespace Render {
	
		class Shader;

		struct ParticleEmitter : public ECS::Component<ParticleEmitter> {

			Comp::Texture* texture;
			Render::Shader* compute, *shader;
			GLuint posBuffer, velBuffer, vao, amtOfParticles = 1u << 21;

			void Update(float dt, const vec3& attractorPosition) const;
		};

		class ParticleListener : public ECS::Listener {
		public:
			ParticleListener();

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

			void add(ECS::Entity* e);
			void remove(ECS::Entity* e);
		};

		class ParticleSystem : public ECS::System {
		public:
			ParticleSystem();

			void updateComponents(float delta, ECS::Entity* entity, const Vector<ECS::BaseComponent*>& entityComp) override;

		};

	}
}