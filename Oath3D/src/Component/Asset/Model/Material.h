#pragma once

#include "Common/Common.h"

#include "Texture.h"

namespace Oath {

	namespace Comp {

		struct LoadMaterial
		{
			String DiffuseTexturePath = "";
			
			String HeightTexturePath = "";
			String DisplacementTexturePath = "";
			String NormalTexturePath = "";
			
			String LightTexturePath = "";

			String SpecularTexturePath = "";
			String SpecularHightlightPath = "";

			float SpecularExponent = 0.0f;
			bool TwoSided = false;
		};

		struct Material : public LoadMaterial
		{
			Material() {}
			Material(const LoadMaterial& vars);
			~Material();

			const void Bind() const;

			Texture DiffuseTexture;
			
			Texture HeightTexture;
			Texture DisplacementTexture;
			Texture NormalTexture;
			
			Texture LightTexture;
			
			Texture SpecularTexture;
			Texture SpecularHightlight;
		};
	}
}