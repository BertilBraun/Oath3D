#include "Common/Common.h"
#include "MasterRenderer.h"
#include "RenderEnums.h"

#include "System/Application/Application.h"
#include "System/ECS/Transform.h"

#include "Skybox/Skybox.h"
#include "Lights/Lights.h"
#include "Particles/ParticleSystem.h"
#include "PostProcessing/PostEffect.h"

#include "Shadow/ShadowRenderer.h"

#include "Shader/Shader.h"

#include "Component/Asset/Asset.h"
#include "Component/Asset/AnimatedAsset.h"
#include "Component/Camera/Camera.h"

#include "System/System/Clock.h"
#include "System/Resource/ResourceManager.h"

#include "ImGui/Include.h"

namespace Oath {
	namespace Render {

		float CalcPointLightBSphere(const PointLightBase& Light)
		{
			float MaxChannel = max(max(Light.Base.Color.x, Light.Base.Color.y), Light.Base.Color.z);
			return (-Light.Attenuation.Linear + sqrtf(Light.Attenuation.Linear * Light.Attenuation.Linear - 4 * Light.Attenuation.Exp * (Light.Attenuation.Exp - 256 * MaxChannel * Light.Base.DiffuseIntensity))) / (2 * Light.Attenuation.Exp);
		}

		static Vector<GLfloat> quadVertices = {
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
		};

		MasterRenderer* GRenderer = nullptr;

		MasterRenderer::MasterRenderer() :
			animShader			(Shader::GetShader("Asset/Animated"		, "Asset/Asset")),
			assetShader			(Shader::GetShader("Asset/Asset"		, "Asset/Asset")),
			instancedShader		(Shader::GetShader("Asset/Instanced"	, "Asset/Asset")),

			animShadow			(Shader::GetShader("Shadow/Animated"	, "Shadow/Null")),
			assetShadow			(Shader::GetShader("Shadow/Asset"		, "Shadow/Null")),
			instancedShadow		(Shader::GetShader("Shadow/Instanced"	, "Shadow/Null")),

			nullShader			(Shader::GetShader("Null"				, "Null")),

			direcLightShader	(Shader::GetShader("Final"				, "Light/DirecLight")),
			pointLightShader	(Shader::GetShader("Light/Light"		, "Light/PointLight")),
			spotLightShader		(Shader::GetShader("Light/Light"		, "Light/SpotLight")),

			shadowShader		(Shader::GetShader("Shadow/Shadow"		, "Shadow/Shadow")),

			finalShader			(Shader::GetShader("Final", "Final"))
		{
			quad.InitWithTexCoord(quadVertices, 2);
			sphere = Comp::ResManager::GetAsset("sphere/sphere.obj");
			cone = Comp::ResManager::GetAsset("cone/cone.obj");
		}

		MasterRenderer::~MasterRenderer() {
			quad.release();
			Comp::ResManager::FreeAsset("sphere/sphere.obj");
			Comp::ResManager::FreeAsset("cone/cone.obj");
			GRenderer = nullptr;
			
			SAVE_DELETE(deferred);
			SAVE_DELETE(skybox);
		}

		void MasterRenderer::process(const Comp::Asset* model, ECS::Transform * transform) {
			
			float d = abs(glm::distance(model->entity->pos(), Comp::GCam->pos()));

			if (d > model->d3) {
				if (model->names[3] != "")
					renderModels[model->meshs[3]].push_back(transform);
			}
			else if (d > model->d2) {
				if (model->names[2] != "")
					renderModels[model->meshs[2]].push_back(transform);
			}
			else if (d > model->d1) {
				if (model->names[1] != "")
					renderModels[model->meshs[1]].push_back(transform);
			}
			else if (model->names[0] != "")
				renderModels[model->meshs[0]].push_back(transform);
		}

		void MasterRenderer::process(const Comp::AnimatedAsset* model, ECS::Transform * transform) {
			animModels[model] = transform;
		}

		bool MasterRenderer::process(DirectionalLight* l)
		{
			if (l == nullptr || directionalLight == nullptr) {
				directionalLight = l;
				return true;
			}
			return false;
		}

		void MasterRenderer::process(const ParticleEmitter * l)
		{
			particles.push_back(l);
		}

		bool MasterRenderer::process(const AmbientLight * l)
		{
			if (l == nullptr || ambientLight == nullptr) {
				ambientLight = l;
				return true;
			}
			return false;
		}

		void MasterRenderer::process(const PointLight* l)
		{
			pointLights.push_back(l);
		}

		void MasterRenderer::process(const SpotLight* l)
		{
			spotLights.push_back(l);
		}

		void MasterRenderer::process(const Skybox* s)
		{
			if (skybox || !s) {
				LOGWARN("OVERRIDING SKYBOX");
				SAVE_DELETE(skybox)
			}
			if (s)
				skybox = new SkyboxComp(s->path);
		}

		void MasterRenderer::process(PostEffect* pe)
		{
			postEffects.emplace_back(pe);
		}

		void MasterRenderer::remove(PostEffect* pe)
		{
			auto it = std::find(postEffects.begin(), postEffects.end(), pe);
			if (it != postEffects.end())
				postEffects.erase(it);
		}

		void MasterRenderer::render() {

			ASSERT(Comp::GCam);
			if (!Comp::GCam)
				return;

			prepare();

			shadowRenderPass();
			geometryPass();
			
			glEnable(GL_STENCIL_TEST);
			
			for (auto l : pointLights)
				pointLightPass(l);
			
			for (auto l : spotLights)
				spotLightPass(l);
			
			glDisable(GL_STENCIL_TEST);
			
			directionalLightPass();

			shadowPass();
			postEffectPass();
			finalPass();

			clear();
		}

		void MasterRenderer::resize(int w, int h)
		{
			if (deferred)
				deferred->resize(w, h);
		}

		void MasterRenderer::prepare()
		{
			if (deferred == nullptr) 
				deferred = new DeferredRenderer();

			float w = Settings::WIDTH;
			float h = Settings::HEIGHT;

			deferred->startFrame();

			instancedShader->start();
			instancedShader->updateCamera(Comp::GCam);

			assetShader->start();
			assetShader->updateCamera(Comp::GCam);

			animShader->start();
			animShader->updateCamera(Comp::GCam);

			nullShader->start();
			nullShader->updateCamera(Comp::GCam);

			pointLightShader->start();
			pointLightShader->updateCamera(Comp::GCam);
			pointLightShader->setUniform("ScreenSize", vec2(w, h));
			pointLightShader->setUniform("NormalTexture"  , NORMAL);
			pointLightShader->setUniform("DiffuseTexture" , DIFFUSE);
			pointLightShader->setUniform("PositionTexture", POSITION);
			pointLightShader->setUniform("SpecularTexture", SPECULAR);

			spotLightShader->start();
			spotLightShader->updateCamera(Comp::GCam);
			spotLightShader->setUniform("ScreenSize", vec2(w, h));
			spotLightShader->setUniform("NormalTexture"  , NORMAL);
			spotLightShader->setUniform("DiffuseTexture" , DIFFUSE);
			spotLightShader->setUniform("PositionTexture", POSITION);
			spotLightShader->setUniform("SpecularTexture", SPECULAR);

			direcLightShader->start();
			direcLightShader->setUniform("NormalTexture"  , NORMAL);
			direcLightShader->setUniform("DiffuseTexture" , DIFFUSE);
			direcLightShader->setUniform("PositionTexture", POSITION);
			direcLightShader->setUniform("SpecularTexture", SPECULAR);

			Shader::Stop();
		}

		void MasterRenderer::clear()
		{
			for (auto& model : renderModels)
				model.second.clear();

			animModels.clear();
			pointLights.clear();
			spotLights.clear();
		}

		void MasterRenderer::instancedAssets(const Comp::AssetMesh* model, const Vector<ECS::Transform*>& transforms, Shader* s, const Comp::Frustum& cam)
		{
			if (!transforms.size())
				return;

			static Vector<mat4> mat;
			mat.resize(max(2 * transforms.size(), mat.size()));

			for (size_t i = 0; i < transforms.size(); i += 2)
				//if (cam.isInFrustum(model->min(transforms[i]), model->max(transforms[i]))) 
				{
					mat4 mm = transforms[i]->modelMatrix();
					mat[i] = mm;
					mat[i + 1] = glm::transpose(glm::inverse(mm));
				}

			glBindBuffer(GL_ARRAY_BUFFER, model->instanceID);
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2 * transforms.size(), mat.data(), GL_DYNAMIC_DRAW));

			s->start();

			model->bind();

			for (auto i : range(model->entries.size()))
				if (renderMesh(model->flags, i)) {

					Comp::Material* m = model->mat[model->entries[i].MatIndex];
					s->UpdateMaterial(m);

					if (m->TwoSided)
						glDisable(GL_CULL_FACE);

					model->draw((uint)i, (uint)transforms.size());

					if (m->TwoSided)
						glEnable(GL_CULL_FACE);
				}
				else break;
		}

		void MasterRenderer::renderAssets(const Comp::AssetMesh* model, const Vector<ECS::Transform*>& transforms, Shader* s, const Comp::Frustum& cam)
		{
			if (!transforms.size())
				return;

			s->start();
			model->bind();

			for (auto i : range(model->entries.size()))
				if (renderMesh(model->flags, i)) {

					Comp::Material* m = model->mat[model->entries[i].MatIndex];
					s->UpdateMaterial(m);

					if (m->TwoSided)
						glDisable(GL_CULL_FACE);

					for (auto t : transforms) 
						//if (cam.isInFrustum(model->min(t), model->max(t))) 
						{
							s->UpdateTransform(t);
							model->draw((uint)i);
						}

					if (m->TwoSided)
						glEnable(GL_CULL_FACE);
				}
				else break;
		}

		void MasterRenderer::renderAnimations(const Comp::AnimatedMesh* mesh, ECS::Transform* t, Shader* s, const Comp::Frustum& cam)
		{
			if (!t) return;

			//if (!cam.isInFrustum(mesh->min(t), mesh->max(t)))
			//	return;

			s->start();
			s->UpdateTransform(t);

			mesh->bind();

			char buffer[50];
			for (int i : range(mesh->numBones)) {
				sprintf(buffer, "bones[%i]", i);
				s->setUniform(buffer, mesh->boneInfo[i].FinalTransformation);
			}
				
			for (auto i : range(mesh->entries.size()))
				if (renderMesh(mesh->flags, i)) {

					Comp::Material* m = mesh->mat[mesh->entries[i].MatIndex];
					s->UpdateMaterial(m);

					if (m->TwoSided)
						glDisable(GL_CULL_FACE);

					mesh->draw((uint)i);

					if (m->TwoSided)
						glEnable(GL_CULL_FACE);
				}
				else break;
		}

		void MasterRenderer::renderParticles()
		{
			static const float GRAVITY_POINT_DISTANCE_FROM_CAM = 80.f;
			static System::Clock c;

			float dt = (float)c.resetS();
			const vec3 attractorPosition = Comp::GCam->pos() + -Comp::GCam->forward * GRAVITY_POINT_DISTANCE_FROM_CAM * .75f;

			for (auto em : particles) 
				em->Update(dt, attractorPosition);
		}

		bool MasterRenderer::renderMesh(int flag, size_t i)
		{
			return !BITSET(flag, i) || i >= 10;
		}

		void MasterRenderer::finalPass()
		{
			deferred->bindForFinalPass();

			finalShader->start();
			finalShader->setUniform("FinalTexture", FINAL);

			quad.bind();
			quad.Draw();
		}

		void MasterRenderer::shadowPass()
		{
			deferred->bindForShadowPass();

			shadowShader->start();
			shadowShader->setUniform("FinalTexture"		, FINAL);
			shadowShader->setUniform("PositionTexture"	, POSITION);
			shadowShader->setUniform("NormalTexture"	, NORMAL);
			shadowShader->setUniform("LightPos"			, directionalLight->entity->pos());
			shadowShader->setUniform("ViewProj"			, Comp::GCam->proj * Comp::GCam->view);

			for (int i : range(Settings::NUM_CASCADES)) {

				float z = (Comp::GCam->proj * vec4(0, 0, -Settings::CASCADES[i + 1], 1)).z;
				mat4 vp = directionalLight->shadow[i]->viewProj;

				shadowShader->setUniform( "EndClipSpace[" + std::to_string(i) + "]", z);
				shadowShader->setUniform("LightViewProj[" + std::to_string(i) + "]", vp);
				shadowShader->setUniform("ShadowTexture[" + std::to_string(i) + "]", DEPTH + i);
			}

			quad.bind();

			//glEnable(GL_BLEND);
			//glBlendEquation(GL_FUNC_ADD);
			//glBlendFunc(GL_ONE, GL_ONE);

			// FOR ALL LIGHTS WITH SHADOWS
			if (directionalLight->Base.shadowType == ShadowMappingType::NO)
				return;

			directionalLight->bindDepth();

			if (ambientLight) ambientLight->bind(shadowShader);

			quad.Draw();

			//glDisable(GL_BLEND);
		}

		void MasterRenderer::shadowRenderPass()
		{
			if (!directionalLight)
				return;

			directionalLight->update();
			shadowRenderPass(directionalLight->Base, directionalLight->shadow);

			// FOR ALL LIGHTS WITH SHADOWS
		}

		void MasterRenderer::postEffectPass()
		{
			deferred->bindForPostEffectPass();

			for (auto pe : postEffects)
				pe->render(
					deferred->finalTexture,
					deferred->depth,
					deferred->buffers[(uint)POSITION],
					deferred->buffers[(uint)DIFFUSE],
					deferred->buffers[(uint)NORMAL],
					deferred->buffers[(uint)SPECULAR]
				);
		}

		void MasterRenderer::shadowRenderPass(BaseLight & light, const Vector<ShadowRenderer*>& shadow)
		{
			if (light.shadowType == ShadowMappingType::NO ||
			   (light.shadowType == ShadowMappingType::STATIC && light.StaticCasted == true))
				return;
			
			for (ShadowRenderer* s : shadow) {

				animShadow->start();
				animShadow->setUniform("ViewProj", s->viewProj);

				assetShadow->start();
				assetShadow->setUniform("ViewProj", s->viewProj);

				instancedShadow->start();
				instancedShadow->setUniform("ViewProj", s->viewProj);

				s->bind();

				for (auto& model : renderModels)
					if (!(model.first->flags & Comp::Asset::NO_SHADOW_CASTER)) {
						if (model.first->instanceID && model.second.size() > 3)
							instancedAssets(model.first, model.second, instancedShadow, s->frustum);
						else
							renderAssets(model.first, model.second, assetShadow, s->frustum);
					}

				for (auto& anim : animModels)
					if (!(anim.first->flags & Comp::Asset::NO_SHADOW_CASTER))
						renderAnimations(anim.first->mesh, anim.second, animShadow, s->frustum);

				s->unBind();
			}
			
			light.StaticCasted = true;
		}

		void MasterRenderer::geometryPass()
		{
			deferred->bindForGeomPass();

			glDepthMask(GL_TRUE);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);

			if (skybox)
				skybox->Draw();

			for (auto& model : renderModels)
				if (model.first->instanceID && model.second.size() > 3)
					instancedAssets(model.first, model.second, instancedShader, Comp::GCam->frustum);
				else
					renderAssets(model.first, model.second, assetShader, Comp::GCam->frustum);
			
			for (auto& anim : animModels) 
				renderAnimations(anim.first->mesh, anim.second, animShader, Comp::GCam->frustum);

			//RenderParticles();

			glDepthMask(GL_FALSE);
		}

		void MasterRenderer::spotLightPass(const SpotLight* light)
		{
			vec3 dim = vec3(CalcPointLightBSphere(light->Base));

			//if (!Comp::GCam->frustum.isInFrustum(light->entity->pos() - dim, light->entity->pos() + dim))
			//	return;

			mat4 mat = MAT4::ModelMat(dim, light->entity->rot(), light->entity->pos());

			deferred->bindForStencilPass();

			nullShader->start();
			nullShader->setUniform("ModelMatrix", mat);

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);

			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			cone->bind();
			cone->draw(0);

			deferred->bindForLightPass();

			spotLightShader->start();
			spotLightShader->setUniform("ModelMatrix", mat);
			light->bind("spotLight", spotLightShader);

			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);

			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);

			glCullFace(GL_FRONT);

			cone->bind();
			cone->draw(0);

			glCullFace(GL_BACK);

			glDisable(GL_BLEND);
		}

		void MasterRenderer::pointLightPass(const PointLight* light)
		{
			vec3 dim = vec3(CalcPointLightBSphere(light->Base));
			
			//if (!Comp::GCam->frustum.isInFrustum(light->entity->pos() - dim, light->entity->pos() + dim))
			//	return;

			mat4 mat = MAT4::ModelMat(dim, quat(), light->entity->pos());

			deferred->bindForStencilPass();

			nullShader->start();
			nullShader->setUniform("ModelMatrix", mat);

			glEnable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);

			glClear(GL_STENCIL_BUFFER_BIT);

			glStencilFunc(GL_ALWAYS, 0, 0);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

			sphere->bind();
			sphere->draw(0);

			deferred->bindForLightPass();

			pointLightShader->start();
			pointLightShader->setUniform("ModelMatrix", mat);
			light->bind("pointLight", pointLightShader);

			glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

			glDisable(GL_DEPTH_TEST);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			sphere->bind();
			sphere->draw(0);

			glCullFace(GL_BACK);

			glDisable(GL_BLEND);
		}

		void MasterRenderer::directionalLightPass()
		{
			if (!directionalLight)
				return;

			deferred->bindForLightPass();

			direcLightShader->start();
			directionalLight->bind("directionalLight", direcLightShader);

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_ONE, GL_ONE);

			quad.bind();
			quad.Draw();

			glDisable(GL_BLEND);
		}
	}
}