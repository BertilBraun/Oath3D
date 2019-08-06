#include "Common/Common.h"
#include "Material.h"
#include "System/Resource/ResourceManager.h"

namespace Oath {

	Comp::Material::Material(const LoadMaterial & vars) : LoadMaterial(vars) {

		if (DiffuseTexturePath != "")
			DiffuseTexture = *ResManager::GetTexture(DiffuseTexturePath);
		if (DisplacementTexturePath != "")
			DisplacementTexture = *ResManager::GetTexture(DisplacementTexturePath);
		if (NormalTexturePath != "")
			NormalTexture = *ResManager::GetTexture(NormalTexturePath);
		if (SpecularTexturePath != "")
			SpecularTexture = *ResManager::GetTexture(SpecularTexturePath);
		if (SpecularHightlightPath != "")
			SpecularHightlight = *ResManager::GetTexture(SpecularHightlightPath);
		if (LightTexturePath != "")
			LightTexture = *ResManager::GetTexture(LightTexturePath);
		if (HeightTexturePath != "")
			HeightTexture = *ResManager::GetTexture(HeightTexturePath);

		DiffuseTexture.offset = 0;

		HeightTexture.offset = 1;
		DisplacementTexture.offset = 2;
		NormalTexture.offset = 3;

		LightTexture.offset = 4;

		SpecularTexture.offset = 5;
		SpecularHightlight.offset = 6;
	}

	Comp::Material::~Material()
	{
		if (DiffuseTexturePath != "")
			ResManager::FreeTexture(DiffuseTexturePath);
		if (DisplacementTexturePath != "")
			ResManager::FreeTexture(DisplacementTexturePath);
		if (NormalTexturePath != "")
			ResManager::FreeTexture(NormalTexturePath);
		if (SpecularTexturePath != "")
			ResManager::FreeTexture(SpecularTexturePath);
		if (SpecularHightlightPath != "")
			ResManager::FreeTexture(SpecularHightlightPath);
		if (LightTexturePath != "")
			ResManager::FreeTexture(LightTexturePath);
		if (HeightTexturePath != "")
			ResManager::FreeTexture(HeightTexturePath);
	}

	const void Comp::Material::Bind() const {

		DiffuseTexture.Bind();
		DisplacementTexture.Bind();
		NormalTexture.Bind();
		SpecularTexture.Bind();
		SpecularHightlight.Bind();
		LightTexture.Bind();
		HeightTexture.Bind();
	}
}
