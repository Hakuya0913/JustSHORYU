#include"ModelComponent.h"
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<algorithm>
#include<stdexcept>

bool ModelComponent::Load(const std::string& filePath)
{

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		filePath,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs
	);

	if (!scene) return false;

	if ((scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0) return false;

	if (!scene->mRootNode) return false;

	return LoadScene(scene);

}

bool ModelComponent::LoadScene(const aiScene* scene)
{

	meshes.clear();
	nodes.clear();
	bones.clear();

	if (!LoadNodes(scene->mRootNode, -1)) return false;

	if (!LoadMeshes(scene)) return false;

	return true;

}

bool ModelComponent::LoadNodes(
	const aiNode* node,
	int32_t parentIndex
)
{

	if (!node) return false;

	Node currentNode;
	currentNode.name			= node->mName.C_Str();
	currentNode.parentIndex		= parentIndex;
	currentNode.localTransform	= ConvertMatrix(node->mTransformation);

	uint32_t currentIndex = static_cast<uint32_t>(nodes.size());
	
	nodes.emplace_back(currentNode);

	//小Nodeを読み込む
	for (UINT i = 0; i < node->mNumChildren; ++i)
	{

		const aiNode* child = node->mChildren[i];

		int32_t childIndex = static_cast<int32_t>(nodes.size());

		if (!LoadNodes(child, currentIndex)) return false;

		nodes[currentIndex].children.emplace_back(childIndex);

	}

	return true;

}

//Mesh読み込み
bool ModelComponent::LoadMeshes(const aiScene* scene)
{

	if (!scene) return false;

	meshes.reserve(scene->mNumMeshes);

	for (UINT i = 0; i < scene->mNumMeshes; ++i)
	{

		if (!LoadMesh(scene->mMeshes[i]))
		{

			return false;

		}

	}

	return true;

}

//1つのMesh読み込み
bool ModelComponent::LoadMesh(const aiMesh* mesh)
{

	if (!mesh)
	{

		return false;

	}

	Mesh result;
	result.name = mesh->mName.C_Str();

	//Vertex
	result.vertices.reserve(mesh->mNumVertices);
	result.boneWeights.reserve(mesh->mNumVertices);

	//Vertex
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{

		Vertex vertex;

		//Position
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		//Normal
		if (mesh->HasNormals())
		{

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

		}

		//UV
		if (mesh->HasTextureCoords(0))
		{

			vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = mesh->mTextureCoords[0][i].y;

		}

		result.vertices.emplace_back(vertex);

	}

	//Index
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{

		const aiFace& face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; ++j)
		{

			result.indices.emplace_back(face.mIndices[j]);

		}

	}

	//Bone
	if (!LoadBones(mesh,result))
	{

		return false;

	}

	result.materialIndex = mesh->mMaterialIndex;

	meshes.emplace_back(std::move(result));

	return true;

}

bool ModelComponent::LoadBones(const aiMesh* mesh, Mesh& result)
{

	if (!mesh)
	{

		return false;

	}

	for (UINT i = 0; i < mesh->mNumBones; ++i)
	{

		const aiBone* aiBone = mesh->mBones[i];

		std::string boneName = aiBone->mName.C_Str();

		//すでに登録済みか検索
		uint32_t boneIndex = 0;
		bool isFound = false;

		for (uint32_t j = 0; j < bones.size(); ++j)
		{

			if (bones[j].name == boneName)
			{

				boneIndex = j;
				isFound = true;
				break;

			}

		}

		//新しいBone
		if (!isFound)
		{

			Bone bone;

			bone.name			= boneName;
			bone.nodeIndex		= FindNodeIndex(boneName);
			bone.offsetMatrix	= ConvertMatrix(aiBone->mOffsetMatrix);

			boneIndex = static_cast<uint32_t>(bones.size());

			bones.emplace_back(bone);

		}

		result.boneIndices.emplace_back(boneIndex);

		//VertexWeight

		for (UINT j = 0; j < aiBone->mNumWeights; ++j)
		{

			const aiVertexWeight& weight = aiBone->mWeights[j];

			if (weight.mVertexId >= result.boneWeights.size())
			{

				continue;

			}

			BoneWeight boneWeight;

			boneWeight.boneIndex = boneIndex;
			boneWeight.weight = weight.mWeight;

			result.boneWeights[weight.mVertexId].emplace_back(boneWeight);

		}

	}

	return true;

}

uint32_t ModelComponent::FindNodeIndex(const std::string& name) const
{

	for (uint32_t i = 0; i < nodes.size(); ++i)
	{

		if (nodes[i].name == name)
		{

			return i;

		}

	}

	return UINT32_MAX;

}

DirectX::XMFLOAT4X4 ModelComponent::ConvertMatrix(const aiMatrix4x4& matrix) const
{

	DirectX::XMFLOAT4X4 result;

	result._11 = matrix.a1;
	result._12 = matrix.a2;
	result._13 = matrix.a3;
	result._14 = matrix.a4;

	result._21 = matrix.b1;
	result._22 = matrix.b2;
	result._23 = matrix.b3;
	result._24 = matrix.b4;

	result._31 = matrix.c1;
	result._32 = matrix.c2;
	result._33 = matrix.c3;
	result._34 = matrix.c4;

	result._41 = matrix.d1;
	result._42 = matrix.d2;
	result._43 = matrix.d3;
	result._44 = matrix.d4;

	return result;
	
}