#pragma once

#include "Component/Asset/Model/Material.h"

namespace Oath {

	namespace ECS {
		struct Transform;
	}

	namespace Comp {

		enum class MESH_VB_TYPES : uint {
			VERTEX = 0,
			NORMAL,
			TANGENT,
			COLOR,
			TEXTURE,
			WORLDMAT,
			INVWORLDMAT = 9,
			NUM
		};
		
		struct MeshEntry {
			uint NumIndices = 0;
			uint BaseVertex = 0;
			uint BaseIndex = 0;
			uint MatIndex = 0xFFFFFFFF;
		};

		struct AssetMesh {

			AssetMesh(GLuint vao, GLuint vbo, GLuint ibo, const String & name);
				
			void bind() const;
			void draw(uint i) const;
			void draw(uint i, uint instances) const;
			void Delete();

			String MeshName;
			Vector<MeshEntry> entries;
			Vector<Material*> mat;

			vec3 min(ECS::Transform* t) const;
			vec3 max(ECS::Transform* t) const;

			vec3 minIn = vec3(100000), maxIn = vec3(-100000);
			int flags = 0;

			GLuint instanceID = 0;
		protected:
			GLuint vao, vbo, ibo;
		};
	}
}
