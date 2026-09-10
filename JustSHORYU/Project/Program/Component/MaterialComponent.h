#pragma once

/*

マテリアルを保持するコンポーネント

*/

#include<DirectXMath.h>
#include<string>
#include<vector>
#include<cstdint>
#include<assimp/scene.h>

class MaterialComponent
{
public:

	//PBR前提
	struct Material
	{

		std::string name;

		//BaseColor
		DirectX::XMFLOAT4 baseColor = { 1.0f,1.0f,1.0f,1.0f };

		//Metalic
		float metalic = 0.0f;

		//Roughness
		float roughness = 1.0f;

		//AmbientOcclusion
		float ambientOcclusion = 1.0f;

		//EmissiveColor
		DirectX::XMFLOAT3 emissiveColor = { 0.0f,0.0f,0.0f };

		//EmissiveStrength
		float emissiveStrength = 1.0f;

		//テクスチャ
		std::string baseColorTexture;			//BaseColor
		std::string metalicRoughnessTexture;	//Metalic and Roughness
		std::string normalTexture;				//Normal
		std::string ambientOcclusionTexture;	//AmbientOcculusion
		std::string emissiveTexture;			//EmissiveTexture

	};

	MaterialComponent() = default;
	~MaterialComponent() = default;

	//ロード
	bool Load(const std::string& filePath);

	//Getter
	size_t GetMaterialCount()					const { return materials.size(); }
	const Material& GetMaterial(size_t index)	const { return materials.at(index); }


private:

	bool LoadMaterials(const aiScene* scene);
	bool LoadMaterial(const aiMaterial* material);
	
	std::string GetTexturePath(const aiMaterial* material, aiTextureType type) const;

	std::vector<Material> materials;

};