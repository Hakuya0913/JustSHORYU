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

	struct Material
	{

		std::string name;

		//基本色
		DirectX::XMFLOAT4 diffuseColor{	 1.0f, 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT4 specularColor{ 1.0f, 1.0f, 1.0f, 1.0f };

		//光沢
		float shininess = 0.0f;

		//テクスチャ
		std::string diffuseTexture;
		std::string specularTexture;
		std::string normalTexture;
		std::string emissiveTexture;

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