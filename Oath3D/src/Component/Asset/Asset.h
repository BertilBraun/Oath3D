#pragma once

#include "Common/Common.h"
#include "System/ECS/Component.h"

#include "Component/Asset/Model/Asset/AssetMesh.h"

namespace Oath {
	namespace Comp {

		struct Asset : public ECS::Component<Asset> {

			Asset() { }
			Asset(const String& path, const int& flagsIn = 0) :
				Asset(path, path, path, path, flagsIn) {}
			Asset(const String& path1, const String& path2, const String& path3, const String& path4, const int& flagsIn = 0) :
				names({ path1, path2, path3, path4 }), flags(flagsIn)
			{
				ASSERT(names.size() == 4);
				ASSERT(meshs.size() == 4);
			}

			Vector<AssetMesh*> meshs{ nullptr, nullptr, nullptr, nullptr };
			Vector<String> names;
			int flags = 0, d1 = 100, d2 = 200, d3 = 300;

			enum FLAG : uint32 {
				NONE = 0,
				DONT_RENDER_MESH0 = BIT(0),
				DONT_RENDER_MESH1 = BIT(1),
				DONT_RENDER_MESH2 = BIT(2),
				DONT_RENDER_MESH3 = BIT(3),
				DONT_RENDER_MESH4 = BIT(4),
				DONT_RENDER_MESH5 = BIT(5),
				DONT_RENDER_MESH6 = BIT(6),
				DONT_RENDER_MESH7 = BIT(7),
				DONT_RENDER_MESH8 = BIT(8),
				DONT_RENDER_MESH9 = BIT(9),
				NO_SHADOW_CASTER = BIT(10)
			};
		};
	}
}