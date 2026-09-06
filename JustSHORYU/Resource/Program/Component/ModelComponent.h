#pragma once

/*

モデルの情報を持つコンポーネント

*/

#include<d3d12.h>
#include<DirectXMath.h>
#include<vector>
#include<cstdint>
#include<string>
#include<assimp/scene.h>
#include"../Utility/ComPtr.h"

class ModelComponent
{
public:

	struct Vertex
	{
		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT3 normal{};
		DirectX::XMFLOAT2 texCoord{};
	};

	struct BoneWeight
	{

		uint32_t boneIndex = 0;
		float weight = 0.0f;

	};

	struct Bone
	{

		std::string name;

		//Node配列上でのBoneの位置
		uint32_t nodeIndex = 0;

		//Mesh空間 -> Bone空間
		DirectX::XMFLOAT4X4 offsetMatrix{};

	};

	struct Mesh
	{

		std::string name;
		
		std::vector<Vertex>		vertices;
		std::vector<uint32_t>	indices;

		//各頂点が影響を受けるBone
		std::vector<std::vector<BoneWeight>> boneWeights;

		//このMeshで使用するBone
		std::vector<uint32_t> boneIndices;

		uint32_t materialIndex = 0;

	};

	struct Node
	{

		std::string name;

		int32_t parentIndex = -1;

		DirectX::XMFLOAT4X4 localTransform{};

		std::vector<uint32_t> children;

	};

	ModelComponent() = default;
	~ModelComponent() = default;

	//モデルの読み込み
	bool Load(const std::string& filePath);

	//Getter
	size_t		GetMeshCount() const { return meshes.size(); }
	const Mesh& GetMesh(size_t index) const { return meshes.at(index); }

	size_t		GetNodeCount() const { return nodes.size(); }
	const Node& GetNode(size_t index) const { return nodes.at(index); }

	size_t		GetBoneCount() const { return bones.size(); }
	const Bone& GetBone(size_t index) const { return bones.at(index); }
	

private:

	//Assimpを使用した各ロード処理
	bool LoadScene(	const aiScene* scene);
	bool LoadNodes(	const aiNode*  node, int32_t parentIndex);
	bool LoadMeshes(const aiScene* scene);
	bool LoadMesh(	const aiMesh* mesh);
	bool LoadBones(	const aiMesh*  mesh, Mesh& result);

	//Utility
	DirectX::XMFLOAT4X4 ConvertMatrix(const aiMatrix4x4& name) const;
	uint32_t			FindNodeIndex(const std::string& name) const;

	std::vector<Mesh> meshes;
	std::vector<Node> nodes;
	std::vector<Bone> bones;

};