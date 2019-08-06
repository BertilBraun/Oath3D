#pragma once

#include "Common/Common.h"

#include "BaseLoader.h"
#include "Component/Asset/Model/Material.h"
#include "Component/Asset/Model/Asset/AssetMesh.h"
#include "Component/Asset/Model/Asset/IndexModel.h"

namespace Oath {

	class AssetLoader : public BaseLoader {
	public:
		AssetLoader(const String& filename);
		~AssetLoader();

		Comp::IndexModel model;

		Vector<Comp::MeshEntry> entries;
		Vector<Comp::Material*> mat;

	private:
		void init();
		void processMesh(aiMesh* mesh);
	};
}
