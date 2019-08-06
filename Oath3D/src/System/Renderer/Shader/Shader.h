#pragma once

#include "Common/Common.h"

namespace Oath {

	namespace ECS {
		struct Transform;
	}
	namespace Comp {
		struct Camera;
		struct Material;
	}

	namespace Render {

		class Shader {
		public:
			static Shader* GetShader(const String& vert, const String& geom, const String& frag);
			static Shader* GetShader(const String& vert, const String& frag);
			static Shader* GetShader(const String& compute);
			static void ReloadShaders();

			void start() { GLCall(glUseProgram(ProgramID)); }
			void Delete() { GLCall(glDeleteProgram(ProgramID)); }
			static void Stop() { glUseProgram(0); }

			void updateCamera(Comp::Camera* cam);
			void UpdateMVP(const Comp::Camera* cam, ECS::Transform* t);
			void UpdateTransform(ECS::Transform* transform);
			void UpdateMaterial(const Comp::Material* mat);

		private:
			struct GLSLStructInfo {
				String Name, Type;
			};

			Shader();
			~Shader();

			void addShader(GLenum type, const String& path);
			void complete();

			void	LoadOneTimeUniforms();
			void	GetAllUniforms(const String& text);

			std::map<String, Vector<GLSLStructInfo>> FindStruct(const String& text);
			void	GetUniform(const String& name, const String& type, std::map<String, Vector<GLSLStructInfo>>& structs);

			GLuint	LoadShader(const String& path, GLenum type);
			String	LoadShaderSource(const String& path);
			void	CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const String& errorMessage);

			int		getLocation(const String& name);
		public:
			void setUniform(const String& name, int value);
			void setUniform(const String& name, bool value);
			void setUniform(const String& name, float value);
			void setUniform(const String& name, const vec2& value);
			void setUniform(const String& name, const vec3& value);
			void setUniform(const String& name, const vec4& value);
			void setUniform(const String& name, const mat4& value);

		private:
			GLuint ProgramID;
			Vector<GLuint> shaderIDs;
			Vector<long long> lastMods;
			Vector<std::pair<String, GLuint>> uniforms;
		};
	}
}