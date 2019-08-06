#pragma once

#include "Common/Common.h"

#include "Component/Asset/Model/Texture.h"

#include "System/ECS/Include.h"

#include "System/Renderer/RenderEnums.h"
#include "System/Renderer/Shader/Shader.h"
#include "System/Renderer/Shadow/ShadowBox.h"
#include "System/Renderer/Shadow/ShadowRenderer.h"
#include "System/Renderer/Deferred/DeferredRenderer.h"

namespace Oath {
	namespace Render {

		struct BaseLight
		{
			ShadowMappingType shadowType = ShadowMappingType::DYNAMIC;
			bool StaticCasted = false;

			BaseLight(const vec3& c, float a, float d) : 
				Color(c), AmbientIntensity(a), DiffuseIntensity(d) {}

			vec3 Color = vec3(1);
			float AmbientIntensity = 0.0f;
			float DiffuseIntensity = 0.0f;

			void bind(const String& name, Shader* shader) const;
		};

		struct LightAttenuation
		{
			LightAttenuation(float c, float l, float e) : 
				Constant(c), Linear(l), Exp(e) {}

			float Constant = 1.0f;
			float Linear = 0.0f;
			float Exp = 0.0f;

			void bind(const String& name, Shader* shader) const;
		};

		struct DirectionalLight : ECS::Component<DirectionalLight>
		{
			DirectionalLight(const vec3& c, float a, float d, const vec3& dir) :
				Base(c, a, d), direction(dir) {}

			void update();
			void bindDepth();

			Vector<ShadowRenderer*> shadow;
			ShadowBox* box = nullptr;
			BaseLight Base;
			vec3 direction = vec3(0);

			void bind(const String& name, Shader* shader) const;
		};

		struct PointLightBase
		{
			PointLightBase(const vec3& c, float a, float d, float co, float l, float e) :
				Base(c, a, d), Attenuation(co, l, e) {}

			BaseLight Base;
			LightAttenuation Attenuation;

			void bind(const String& name, Shader* shader, ECS::Entity* entity = nullptr) const;
		};

		struct PointLight : ECS::Component<PointLight>
		{
			PointLight(const vec3& c, float a, float d, float co, float l, float e, GLsizei w = 400.0f, GLsizei h = 300.0f) :
				Base(c, a, d, co, l, e), w(w), h(h) {}

			void Update();

			GLsizei w, h;
			ShadowRenderer* shadow[6];
			PointLightBase Base;

			void bind(const String& name, Shader* shader, ECS::Entity* entity = nullptr) const;
		};

		struct SpotLight : ECS::Component<SpotLight>
		{
			SpotLight(const vec3& c, float a, float d, float co, float l, float e, const vec3& dir, float cut, GLsizei w = 400.0f, GLsizei h = 300.0f) :
				Base(c, a, d, co, l, e), Direction(dir), Cutoff(cut), w(w), h(h) {}

			void Update();

			GLsizei w, h;
			ShadowRenderer* shadow = nullptr;
			PointLightBase Base;
			vec3 Direction = vec3(0);
			float Cutoff = 0.0f;

			void bind(const String& name, Shader* shader) const;
		};

		struct AmbientLight : ECS::Component<AmbientLight>
		{
			AmbientLight(float c) : Factor(c) {}
			AmbientLight(vec3 c) : Factor(c) {}

			vec3 Factor;

			void bind(Shader* shader) const;
		};
	}
}
