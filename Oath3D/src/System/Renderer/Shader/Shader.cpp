#include "Common/Common.h"
#include "Shader.h"

#include "System/ECS/Transform.h"
#include "Component/Asset/Model/Material.h"
#include "Component/Camera/Camera.h"

#include "glm/mat4x4.hpp"

namespace Oath {

	namespace Render {

		std::map< std::tuple<String, String, String>, Shader*> shadersVsGoFg;
		std::map< std::tuple<String, String>, Shader*> shadersVsFg;
		Vector<String> sources;

		Shader * Shader::GetShader(const String & vert, const String & geom, const String & frag)
		{
			std::tuple<String, String, String> pair{ vert, geom, frag };

			if (shadersVsGoFg.find(pair) == shadersVsGoFg.end()) {

				Shader* s = new Shader();

				LOGINFO("SHADER", "LOADING SHADER", s->ProgramID, ":", vert, geom, frag);

				s->addShader(GL_VERTEX_SHADER  , vert + Settings::SHADER_VERT_EXT);
				s->addShader(GL_GEOMETRY_SHADER, geom + Settings::SHADER_GEOM_EXT);
				s->addShader(GL_FRAGMENT_SHADER, frag + Settings::SHADER_FRAG_EXT);

				s->complete();

				s->start();
				s->LoadOneTimeUniforms();
				Stop();

				shadersVsGoFg.emplace(pair, s);
			}

			return shadersVsGoFg[pair];
		}

		Shader * Shader::GetShader(const String & vert, const String & frag)
		{
			std::tuple<String, String> pair{ vert, frag };

			if (shadersVsFg.find(pair) == shadersVsFg.end()){

				Shader* s = new Shader();

				LOGINFO("SHADER", "LOADING SHADER", s->ProgramID, ":", vert, frag);

				s->addShader(GL_VERTEX_SHADER  , vert + Settings::SHADER_VERT_EXT);
				s->addShader(GL_FRAGMENT_SHADER, frag + Settings::SHADER_FRAG_EXT);
			
				s->complete();
				
				s->start();
				s->LoadOneTimeUniforms();
				Stop();
				
				shadersVsFg.emplace(pair, s);
			}

			return shadersVsFg[pair];
		}
		
		Shader* Shader::GetShader(const String& compute){

			Shader* s = new Shader();

			LOGINFO("SHADER", "LOADING SHADER", s->ProgramID, ":", compute);

			s->addShader(GL_COMPUTE_SHADER, compute + Settings::SHADER_COMP_EXT);
			
			s->complete();
				
			return s;
		}

		void Shader::ReloadShaders()
		{
			for (auto& s : shadersVsFg) 
				if (File::LastMod(Settings::SHADER_PATH + std::get<0>(s.first) + Settings::SHADER_VERT_EXT) > s.second->lastMods[0] ||
					File::LastMod(Settings::SHADER_PATH + std::get<1>(s.first) + Settings::SHADER_FRAG_EXT) > s.second->lastMods[1])
				{
					Shader shader;

					shader.addShader(GL_VERTEX_SHADER  , std::get<0>(s.first) + Settings::SHADER_VERT_EXT);
					shader.addShader(GL_FRAGMENT_SHADER, std::get<1>(s.first) + Settings::SHADER_FRAG_EXT);
					
					shader.complete();
					
					shader.start();
					shader.LoadOneTimeUniforms();
					shader.Stop();

					LOGINFO("SHADER", "RELOADING SHADER :", s.second->ProgramID, shader.ProgramID, std::get<0>(s.first), std::get<1>(s.first));
					
					s.second->Delete();
					*s.second = shader;
				}

			for (auto& s : shadersVsGoFg)
				if (File::LastMod(Settings::SHADER_PATH + std::get<0>(s.first) + Settings::SHADER_VERT_EXT) > s.second->lastMods[0] ||
					File::LastMod(Settings::SHADER_PATH + std::get<1>(s.first) + Settings::SHADER_GEOM_EXT) > s.second->lastMods[1] ||
					File::LastMod(Settings::SHADER_PATH + std::get<2>(s.first) + Settings::SHADER_FRAG_EXT) > s.second->lastMods[2]) 
				{
					Shader shader;

					shader.addShader(GL_VERTEX_SHADER  , std::get<0>(s.first) + Settings::SHADER_VERT_EXT);
					shader.addShader(GL_GEOMETRY_SHADER, std::get<1>(s.first) + Settings::SHADER_GEOM_EXT);
					shader.addShader(GL_FRAGMENT_SHADER, std::get<2>(s.first) + Settings::SHADER_FRAG_EXT);

					shader.complete();

					shader.start();
					shader.LoadOneTimeUniforms();
					shader.Stop();

					LOGINFO("SHADER", "RELOADING SHADER :", s.second->ProgramID, shader.ProgramID, std::get<0>(s.first), std::get<1>(s.first));

					s.second->Delete();
					*s.second = shader;
				}
		}

		Shader::Shader() {
			ProgramID = glCreateProgram();
		}
		
		void Shader::addShader(GLenum type, const String& path){
			
			String source = LoadShaderSource(Settings::SHADER_PATH + path);
			
			if (source == "") {
				LOGERROR("SHADER ERROR FAILED TO LOAD SOURCE : ", path);
				return;
			}

			shaderIDs.emplace_back(LoadShader(source, type));
			sources.emplace_back(source);
			lastMods.emplace_back(File::LastMod(Settings::SHADER_PATH + path));
		}
		
		void Shader::complete() {
			
			GLCall(glLinkProgram(ProgramID));
			CheckShaderError(ProgramID, GL_LINK_STATUS, true, "Linking failed");

			GLCall(glValidateProgram(ProgramID));
			CheckShaderError(ProgramID, GL_VALIDATE_STATUS, true, "Program Invalid!");

			for (GLuint id : shaderIDs) {
				GLCall(glDetachShader(ProgramID, id));
				GLCall(glDeleteShader(id));
			}

			start();
			for (const String& s : sources)
				GetAllUniforms(s);
			Stop();

			sources.clear();
		}

		Shader::~Shader() {
			Stop();
		}

		void Shader::updateCamera(Comp::Camera * cam)
		{
			for (auto uniform : uniforms)
				if (uniform.first == "ProjMatrix") {
					GLCall(glUniformMatrix4fv(uniform.second, 1, false, &cam->proj[0][0]));
				}
				else if (uniform.first == "ViewMatrix") {
					GLCall(glUniformMatrix4fv(uniform.second, 1, false, &cam->view[0][0]));
				}
				else if (uniform.first == "EyePos") {
					vec3 pos = cam->pos();
					GLCall(glUniform3f(uniform.second, pos.x, pos.y, pos.z));
				}
		}

		void Shader::UpdateMVP(const Comp::Camera * cam, ECS::Transform * t)
		{
			setUniform("MVP", cam->proj * cam->view * t->modelMatrix());
		}

		void Shader::UpdateTransform(ECS::Transform * transform)
		{
			mat4 mm = transform->modelMatrix();

			for (auto uniform : uniforms)
				if (uniform.first == "ModelMatrix") {
					GLCall(glUniformMatrix4fv(uniform.second, 1, false, &mm[0][0]));
				}
				else if (uniform.first == "InvModelMatrix") {
					mat3 imm = mat3(glm::transpose(glm::inverse(mm)));
					GLCall(glUniformMatrix3fv(uniform.second, 1, false, &imm[0][0]));
				}
		}

		void Shader::UpdateMaterial(const Comp::Material * mat)
		{
			mat->Bind();

			for (auto uniform : uniforms)
				if (uniform.first == "SpecularExponent") {
					GLCall(glUniform1f(uniform.second, mat->SpecularExponent));
				}
		}

		GLuint Shader::LoadShader(const String& text, GLenum type) {

			const GLchar* p = text.c_str();
			GLint length = (GLint)text.length();

			GLuint ID = glCreateShader(type);
			GLCall(glShaderSource(ID, 1, &p, &length));
			GLCall(glCompileShader(ID));
			CheckShaderError(ID, GL_COMPILE_STATUS, false, type + "Compiling failed");
			GLCall(glAttachShader(ProgramID, ID));
			return ID;
		}

		String Shader::LoadShaderSource(const String & path)
		{
			String source;
			auto lines = File::ReadAllLines(path);

			for (auto& l : lines) {
				size_t s = l.find("#include");

				if (s != String::npos) {
					size_t f = l.find("\"", s) + 1, n = l.find("\"", f);
					source.append(LoadShaderSource(Settings::SHADER_PATH + l.substr(f, n - f)) + "\n");
				}
				else
					source.append(l + "\n");
			}

			return source;
		}

		void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const String& errorMessage) {
			GLint success = 0;

			if (isProgram)
				GLCall(glGetProgramiv(shader, flag, &success))
			else
				GLCall(glGetShaderiv(shader, flag, &success))

			if (success == GL_FALSE) {
				GLchar error[1024] = { 0 };

				if (isProgram)
					GLCall(glGetProgramInfoLog(shader, sizeof(error), NULL, error))
				else
					GLCall(glGetShaderInfoLog(shader, sizeof(error), NULL, error))

				LOGERROR("SHADER ERROR MESSAGE : ", error);
			}
		}

		void Shader::GetUniform(const String & name, const String & type, std::map<String, Vector<GLSLStructInfo>>& structs)
		{
			if (structs.find(type) != structs.end()) {
				for (const GLSLStructInfo& s : structs[type])
					GetUniform(name + "." + s.Name, s.Type, structs);
				return;
			}

			int uniformLocation = glGetUniformLocation(ProgramID, name.c_str());
			//if (uniformLocation == 0xFFFFFFFF)
			//	LOGWARN("UNIFORM NOT FOUND OR NOT USED : ", name, uniformLocation);

			uniforms.emplace_back(std::make_pair(name, uniformLocation));
		}

		void Shader::LoadOneTimeUniforms()
		{
			for (auto uniform : uniforms)
				if (uniform.first == "DiffuseTexture") {
					GLCall(glUniform1i(uniform.second, 0));
				}
				else if (uniform.first == "AmbientTexture") {
					GLCall(glUniform1i(uniform.second, 1));
				}
				else if (uniform.first == "DisplacementTexture") {
					GLCall(glUniform1i(uniform.second, 2));
				}
				else if (uniform.first == "NormalTexture") {
					GLCall(glUniform1i(uniform.second, 3));
				}
				else if (uniform.first == "SpecularTexture") {
					GLCall(glUniform1i(uniform.second, 4));
				}
				else if (uniform.first == "SpecularHightlight") {
					GLCall(glUniform1i(uniform.second, 5));
				}
				else if (uniform.first == "LightTexture") {
					GLCall(glUniform1i(uniform.second, 6));
				}
				else if (uniform.first == "HeightTexture") {
					GLCall(glUniform1i(uniform.second, 7));
				}
		}

		void Shader::GetAllUniforms(const String & text)
		{
			std::map<String, Vector<GLSLStructInfo>> structs = FindStruct(text);

			static const String UNIFORM = "uniform";

			for (auto token : String::split(text, '\n')) {
				auto lineToken = token.Trim(';').split(' ');

				if (lineToken.size() != 3)
					continue;

				if (lineToken[0] == UNIFORM && lineToken[2].find('[') == lineToken[2].find(']'))
					GetUniform(lineToken[2], lineToken[1], structs);
			}
		}

		std::map<String, Vector<Shader::GLSLStructInfo>> Shader::FindStruct(const String & text)
		{
			static const String STRUCT = "struct";
			std::map<String, Vector<GLSLStructInfo>> structs;

			size_t structStartLocation = text.find(STRUCT);

			while (structStartLocation != -1)
			{
				if (!(structStartLocation != 0 && 
					(String::isWhitespace(text.at(structStartLocation - 1)) || text.at(structStartLocation - 1) == ';') && 
					(String::isWhitespace(text.at(structStartLocation + STRUCT.length())))))
				{
					structStartLocation = text.find(STRUCT, structStartLocation + STRUCT.length());
					continue;
				}

				size_t nameBegin = structStartLocation + STRUCT.length() + 1;
				size_t braceBegin = text.find("{", nameBegin);
				size_t braceEnd = text.find("}", braceBegin);

				String structName = String(text.substr(nameBegin, braceBegin)).Trim();
				Vector<GLSLStructInfo> glslStructs;

				int componentSemicolonPos = (int)text.find(";", braceBegin);
				while (componentSemicolonPos != -1 && componentSemicolonPos < braceEnd)
				{
					int componentNameEnd = componentSemicolonPos + 1;

					while (String::isWhitespace(text.at(componentNameEnd - 1)) || text.at(componentNameEnd - 1) == ';')
						componentNameEnd--;

					int componentNameStart = componentSemicolonPos;

					while (!String::isWhitespace(text.at(componentNameStart - 1)))
						componentNameStart--;

					int componentTypeEnd = componentNameStart;

					while (String::isWhitespace(text.at(componentTypeEnd - 1)))
						componentTypeEnd--;

					int componentTypeStart = componentTypeEnd;

					while (!String::isWhitespace(text.at(componentTypeStart - 1)))
						componentTypeStart--;

					glslStructs.push_back(
						{ 
						text.substr(componentNameStart, componentNameEnd),
						text.substr(componentTypeStart, componentTypeEnd) 
						});

					componentSemicolonPos = (int)text.find(";", componentSemicolonPos + 1);
				}

				structs.emplace(structName, glslStructs);
				structStartLocation = text.find(STRUCT, structStartLocation + STRUCT.length());
			}

			return structs;
		}

		int Shader::getLocation(const String & name)
		{
			for (auto uniform : uniforms)
				if (uniform.first == name) 
					return uniform.second;

			int Location = glGetUniformLocation(ProgramID, name.c_str());

			if (Location == 0xFFFFFFFF)
				LOGWARN(ProgramID, "UNIFORM NOT FOUND : ", name);

			uniforms.emplace_back(std::make_pair(name, Location));
			return Location;
		}

		void Shader::setUniform(const String& name, bool value) {
			GLCall(glUniform1f(getLocation(name), (float)value));
		}

		void Shader::setUniform(const String& name, int value) {
			GLCall(glUniform1i(getLocation(name), value));
		}

		void Shader::setUniform(const String& name, float value) {
			GLCall(glUniform1f(getLocation(name), value));
		}

		void Shader::setUniform(const String& name, const glm::vec2& value) {
			GLCall(glUniform2f(getLocation(name), value.x, value.y));
		}

		void Shader::setUniform(const String& name, const glm::vec3& value) {
			GLCall(glUniform3f(getLocation(name), value.x, value.y, value.z));
		}

		void Shader::setUniform(const String& name, const vec4& value)
		{
			GLCall(glUniform4f(getLocation(name), value.x, value.y, value.z, value.w));
		}

		void Shader::setUniform(const String& name, const glm::mat4& value) {
			GLCall(glUniformMatrix4fv(getLocation(name), 1, false, &value[0][0]));
		}
	}
}