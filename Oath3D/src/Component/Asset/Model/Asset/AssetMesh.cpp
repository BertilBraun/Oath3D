#include "Common/Common.h"
#include "AssetMesh.h"
#include "System/Loader/Loader.h"
#include "System/Renderer/Shader/Shader.h"
#include "System/ECS/Include.h"

namespace Oath {

	namespace Comp {

		AssetMesh::AssetMesh(GLuint vao, GLuint vbo, GLuint ibo, const String & name)
		{
			this->vao = vao;
			this->vbo = vbo;
			this->ibo = ibo;
			this->MeshName = MeshName;
		}

		void AssetMesh::bind() const {

			GLCall(glBindVertexArray(vao));

			for (auto i : range(9))
				GLCall(glEnableVertexAttribArray(i));
		}

		void AssetMesh::draw(uint i) const
		{
			glDrawElementsBaseVertex(GL_TRIANGLES,
				entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(uint) * entries[i].BaseIndex),
				entries[i].BaseVertex);
		}

		void AssetMesh::draw(uint i, uint instances) const
		{
			glDrawElementsInstancedBaseVertex(GL_TRIANGLES,
				entries[i].NumIndices,
				GL_UNSIGNED_INT,
				(void*)(sizeof(uint) * entries[i].BaseIndex),
				instances,
				entries[i].BaseVertex);
		}

		void AssetMesh::Delete()
		{
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ibo);

			glDeleteVertexArrays(1, &vao);
			vbo = ibo = vao = 0;

			for (auto m : mat)
				delete m;
		}

		vec3 AssetMesh::min(ECS::Transform* t) const
		{
			return t->modelMatrix() * minIn;
		}

		vec3 AssetMesh::max(ECS::Transform* t) const
		{
			return t->modelMatrix() * maxIn;
		}
	}
}