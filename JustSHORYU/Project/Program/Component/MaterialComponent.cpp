#include"MaterialComponent.h"
#include<assimp/Importer.hpp>
#include<assimp/material.h>
#include<assimp/postprocess.h>
#include<filesystem>
#include<Windows.h>

bool MaterialComponent::Load(const std::string& filePath)
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		filePath,
		aiProcess_Triangulate | aiProcess_FlipUVs
	);

	if (!scene)
	{

		return false;

	}

	if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
	{

		return false;

	}

	if (scene->mNumMaterials == 0)
	{

		return false;

	}

	materials.clear();

	return LoadMaterials(scene);

}

//Material一覧読み込み
bool MaterialComponent::LoadMaterials(const aiScene* scene)
{

	if (!scene)
	{

		return false;

	}

	materials.reserve(scene->mNumMaterials);

	for (UINT i = 0; i < scene->mNumMaterials; ++i)
	{

		if (!LoadMaterial(scene->mMaterials[i]))
		{

			return false;

		}

	}

	return true;

}

//1つのMaterial読み込み
bool MaterialComponent::LoadMaterial(const aiMaterial* material)
{

	if (!material)
	{

		return false;

	}

	//処理が成功したか
	aiReturn isFuncSuccess;

	Material result;

	//name
	aiString name;

	isFuncSuccess = material->Get(AI_MATKEY_NAME, name);

	if (isFuncSuccess == AI_SUCCESS)
	{

		result.name = name.C_Str();

	}

	//DiffuseColor
	aiColor4D baseColor;

	isFuncSuccess 
		= aiGetMaterialColor(
			material,
			AI_MATKEY_COLOR_DIFFUSE,
			&baseColor
	);

	if (isFuncSuccess == AI_SUCCESS)
	{

		result.baseColor.x = baseColor.r;
		result.baseColor.y = baseColor.g;
		result.baseColor.z = baseColor.b;
		result.baseColor.w = baseColor.a;

	}

	//BaseColorTexture
	result.baseColorTexture = GetTexturePath(material, aiTextureType_BASE_COLOR);

	//Metalic and Roughness Texture
	result.metalicRoughnessTexture = GetTexturePath(material, aiTextureType_UNKNOWN);

	//NormalTexture
	result.normalTexture = GetTexturePath(material, aiTextureType_NORMALS);

	//AmbientOcclusionTexture
	result.ambientOcclusionTexture = GetTexturePath(material, aiTextureType_AMBIENT_OCCLUSION);

	//EmissiveTexture
	result.emissiveTexture = GetTexturePath(material, aiTextureType_EMISSIVE);

	materials.emplace_back(result);

	return true;

}

//テクスチャパス取得
std::string MaterialComponent::GetTexturePath(const aiMaterial* material, aiTextureType type) const
{

	if (!material)
	{

		return {};

	}

	if (material->GetTextureCount(type) == 0)
	{

		return {};

	}

	aiReturn isFuncSuccess;

	aiString path;

	isFuncSuccess
		= material->GetTexture(
			type,
			0,
			&path
		);

	if (isFuncSuccess != AI_SUCCESS)
	{

		return {};

	}

	return path.C_Str();

}