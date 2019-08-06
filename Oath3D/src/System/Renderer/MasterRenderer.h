#pragma once

#include "Component/Asset/Model/BasicMesh.h"

namespace Oath {

	namespace ECS {
		struct Transform;
	}
	namespace Comp {
		class Frustum;
		struct Asset;
		struct AnimatedAsset;
		struct AssetMesh;
		struct AnimatedMesh;
	}

	namespace Render {

		struct Skybox;
		struct SkyboxComp;

		class Shader;
		class ShadowRenderer;
		class DeferredRenderer;
		class PostEffect;

		struct ParticleEmitter;
		struct DirectionalLight;
		struct AmbientLight;
		struct PointLight;
		struct SpotLight;
		struct BaseLight;

		class MasterRenderer {
		public:
			MasterRenderer();
			~MasterRenderer();

			void process(const Comp::Asset* model, ECS::Transform* transform);
			void process(const Comp::AnimatedAsset* model, ECS::Transform* transform);

			bool process(DirectionalLight* l);
			void process(const ParticleEmitter* l);
			bool process(const AmbientLight* l);
			void process(const PointLight* l);
			void process(const SpotLight* l);
			void process(const Skybox* s);
			void process(PostEffect* pe);

			void remove(PostEffect* pe);

			void render();

			void resize(int w, int h);

			DirectionalLight* directionalLight = nullptr;
			const AmbientLight* ambientLight = nullptr;
			const SkyboxComp* skybox = nullptr;

		private:
			void prepare();
			void clear();

			void instancedAssets(const Comp::AssetMesh* model, const Vector<ECS::Transform*>& transforms, Shader* s, const Comp::Frustum& cam);
			void renderAssets(const Comp::AssetMesh* model, const Vector<ECS::Transform*>& transforms, Shader* s, const Comp::Frustum& cam);
			void renderAnimations(const Comp::AnimatedMesh* mesh, ECS::Transform* t, Shader* s, const Comp::Frustum& cam);
			void renderParticles();

			bool renderMesh(int flag, size_t i);

			void finalPass();
			void shadowPass();
			void shadowRenderPass();
			void postEffectPass();
			void shadowRenderPass(BaseLight& light, const Vector<ShadowRenderer*>& shadow);
			void geometryPass();
			void spotLightPass(const SpotLight* light);
			void pointLightPass(const PointLight* light);
			void directionalLightPass();

			std::map<const Comp::AssetMesh*, Vector<ECS::Transform*>> renderModels;
			std::map<const Comp::AnimatedAsset*, ECS::Transform*> animModels;

			Vector<const SpotLight*> spotLights;
			Vector<const PointLight*> pointLights;
			Vector<const ParticleEmitter*> particles;

			Vector<PostEffect*> postEffects;

			Shader* animShader;
			Shader* assetShader;
			Shader* instancedShader;
			Shader* animShadow;
			Shader* assetShadow;
			Shader* instancedShadow;
			Shader* nullShader;
			Shader* pointLightShader;
			Shader* spotLightShader;
			Shader* direcLightShader;
			Shader* shadowShader;
			Shader* finalShader;

			Comp::BasicMesh quad;
			Comp::AssetMesh* sphere, *cone;

			DeferredRenderer* deferred = nullptr;
		};
	}
}