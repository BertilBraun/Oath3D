#pragma once

#include "Common/Common.h"

#include "System/ECS/Include.h"
#include "Component/Asset/Model/BasicMesh.h"

namespace Oath {

	namespace Comp {
		struct Texture;
	}

	namespace Render {

		class Shader;

		struct SkyboxComp {
		public:
			SkyboxComp(const String& path);
			~SkyboxComp();

			void Draw() const;

		private:
			Shader* shader;
			Comp::Texture* tex;
			Comp::BasicMesh box;
		};

		struct Skybox : public ECS::Component<Skybox> {
			Skybox(const String& p) : path(p) {}
			String path;
		};

		class SkyboxListener : public ECS::Listener {
		public:
			SkyboxListener() { addComponent(Skybox::ID); }

			void onMakeEntity(ECS::Entity* handle) override;
			void onRemoveEntity(ECS::Entity* handle) override;
			void onAddComponent(ECS::Entity* handle, uint32 id) override;
			void onRemoveComponent(ECS::Entity* handle, uint32 id) override;

		};
	}
}