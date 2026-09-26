#pragma once

/*

FBXモデルの表示

*/

#include<d3d12.h>
#include<vector>
#include<cstdint>
#include<array>
#include<dxgi1_6.h>
#include<unordered_map>
#include<wincodec.h>
#include<DirectXMath.h>
#include"../Utility/ComPtr.h"
#include"RootSignature.h"
#include"PipelineState.h"
#include"../Camera/Camera.h"
#include"../Component/RenderComponent.h"
#include"../Component/ModelComponent.h"
#include"../Component/ModelStructure.h"

class ModelRenderer
{
public:

	ModelRenderer() = default;
	~ModelRenderer() = default;

	//初期化
	bool Init(ID3D12Device6* device, ID3D12GraphicsCommandList* cmdList, Camera& camera);

	bool CreateModelResource(const ModelComponent& model);

	//描画
	void Render(
		ID3D12GraphicsCommandList* cmdList,
		RenderComponent& renderComponent
	);

private:

	//GPUでのメッシュリソース
	struct MeshResource
	{

		ComPtr<ID3D12Resource> vertexBuffer;
		ComPtr<ID3D12Resource> indexBuffer;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		D3D12_INDEX_BUFFER_VIEW  indexBufferView{};

		uint32_t indexCount = 0;
		uint32_t materialIndex = 0;

	};

	//定数バッファ
	struct alignas(256) TransformBuffer
	{

		DirectX::XMFLOAT4X4 world{};
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 proj{};

		float padding[16] = {};	//padding

	};

	struct alignas(256) MaterialBuffer
	{

		DirectX::XMFLOAT4 baseColor{};

		float metalic = 0.0f;
		float roughness = 1.0f;
		float ambientOcclusion = 1.0f;
		float padding = 0.0f;	//パディング用(16byteアライン)

		DirectX::XMFLOAT3 emissiveColor{};
		float			  emissiveStrength = 1.0f;

		float padding2[44] = {};	//padding

	};

	struct TextureResource
	{

		ComPtr<ID3D12Resource> texture;

		D3D12_GPU_DESCRIPTOR_HANDLE srvHandle{};

		uint32_t embeddedTextureIndex = 0;

	};

	//初期化に使用する
	bool CreateMeshResource(const Mesh& mesh, MeshResource& resource);
	bool CreateVertexBuffer(const Mesh& mesh, MeshResource& resource);
	bool CreateIndexBuffer(const Mesh& mesh, MeshResource& resource);

	bool CreateTextureResources(const ModelComponent& model);
	bool CreateTextureResource(const EmbeddedTexture& embeddedTexture, uint32_t embeddedTextureIndex);

	bool DecodeEmbeddedTexture(
		const EmbeddedTexture& embeddedTexture,
		std::vector<uint8_t>& pixelData,
		UINT& width, UINT& height, UINT& rowPitch
	);

	bool CreateTextureUploadResource(
		const std::vector<uint8_t>& pixelData,
		UINT width, UINT height, UINT rowPitch,
		TextureResource& resource
	);

	bool CreateConstantBuffers();

	//更新
	void UpdateTransformBuffer(const RenderComponent& renderComponent);
	void UpdateMaterialBuffer(const Material& material);

	//定数バッファサイズを256byte境界に合わせる
	constexpr UINT AlignConstantBufferSize(UINT size);

	//DX12
	ComPtr<ID3D12Device6> device;
	ComPtr<ID3D12GraphicsCommandList> cmdList;

	RootSignature rootSig;
	PipelineState pso;

	//外部の参照
	Camera* camera;

	//モデルリソース
	std::vector<MeshResource> meshResources;

	//定数バッファ
	ComPtr<ID3D12Resource> transformBuffer;
	ComPtr<ID3D12Resource> materialBuffer;

	TransformBuffer* mappedTransformBuffer = nullptr;
	MaterialBuffer* mappedMaterialBuffer = nullptr;

	std::vector<TextureResource> textureResources;
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	UINT srvDescriptorSize = 0;

};