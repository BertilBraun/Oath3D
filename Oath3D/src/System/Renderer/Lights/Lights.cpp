#include "Common/Common.h"
#include "Lights.h"

namespace Oath {
	namespace Render {

		void BaseLight::bind(const String& name, Shader* shader) const
		{
			shader->setUniform(name + ".Color", Color);
			shader->setUniform(name + ".AmbientIntensity", AmbientIntensity);
			shader->setUniform(name + ".DiffuseIntensity", DiffuseIntensity);
		}

		void DirectionalLight::update()
		{
			box->update(direction);

			mat4 view = glm::lookAt(vec3(0, 0, 0), normalize(direction), vec3(0, 1, 0));

			for (int i : range(Settings::NUM_CASCADES)) {
				
				mat4 proj = glm::ortho(box->lbn[i].x, box->rtf[i].x, box->lbn[i].y, box->rtf[i].y, box->lbn[i].z, box->rtf[i].z);

				shadow[i]->viewProj = proj * view;
				shadow[i]->view = view;
				shadow[i]->proj = proj;

				shadow[i]->frustum.Update(shadow[i]->viewProj);
			}
		}

		void DirectionalLight::bindDepth()
		{
			for (int i : range((int)shadow.size()))
				shadow[i]->bindDepth(i);
		}

		void DirectionalLight::bind(const String& name, Shader* shader) const
		{
			Base.bind(name + ".Base", shader);
			shader->setUniform(name + ".Direction", direction);
		}

		void LightAttenuation::bind(const String& name, Shader* shader) const
		{
			shader->setUniform(name + ".Constant", Constant);
			shader->setUniform(name + ".Linear", Linear);
			shader->setUniform(name + ".Exp", Exp);
		}

		void PointLight::Update()
		{
			//TODO UPDATE PROJ VIEW FRUSTUM BASED ON GCam FOR ALL 6 CAM SIDES
		}

		void PointLight::bind(const String& name, Shader* shader, ECS::Entity* entity) const
		{
			Base.bind(name, shader);
			shader->setUniform(name + ".Position", (entity) ? entity->transform.pos() : this->entity->transform.pos());
		}

		void SpotLight::Update()
		{
			//TODO UPDATE PROJ VIEW FRUSTUM BASED ON GCam
		}

		void SpotLight::bind(const String & name, Shader * shader) const
		{
			Base.bind(name + ".Base", shader, entity);
			shader->setUniform(name + ".Direction", Direction);
			shader->setUniform(name + ".Cutoff", Cutoff);
		}

		void AmbientLight::bind(Shader * shader) const
		{
			shader->setUniform("AmbientFactor", Factor);
		}

		void PointLightBase::bind(const String & name, Shader * shader, ECS::Entity * entity) const
		{
			Base.bind(name + ".Base", shader);
			Attenuation.bind(name + ".Atten", shader);
		}
	}
}
