#pragma once

#include "Common/Common.h"

#include "Component/Asset/Model/Material.h"

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace Oath {

	class BaseLoader {
	public:
		BaseLoader(const String& filename);

		const aiScene* scene;
		bool isValid = true;
	protected:
		Comp::Material* processMat(aiMaterial* aimat);

		String folder;
	};
}
