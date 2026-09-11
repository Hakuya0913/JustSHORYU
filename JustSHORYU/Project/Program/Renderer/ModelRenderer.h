#pragma once

/*

FBXモデルの表示

*/

#include<d3d12.h>
#include<vector>
#include<cstdint>
#include"../Utility/ComPtr.h"
#include"../Component/RenderComponent.h"
#include"RootSignature.h"
#include"PipelineState.h"

class Camera;
class LightManager;

class ModelRenderer
{
public:

	ModelRenderer(
		ID3D12Device6* device,
		Camera& camera
	);
	~ModelRenderer() = default;

	//初期化
	bool Init();

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

		uint32_t indexCount		= 0;
		uint32_t materialIndex	= 0;

	};

	//定数バッファ
	struct alignas(256) TransformBuffer
	{

		DirectX::XMFLOAT4X4 world{};
		DirectX::XMFLOAT4X4 view{};
		DirectX::XMFLOAT4X4 proj{};

	};

	struct alignas(256) MaterialBuffer
	{

		DirectX::XMFLOAT4 baseColor{};

		float metalic			= 0.0f;
		float roughness			= 1.0f;
		float ambientOcclusion	= 1.0f;
		float padding			= 0.0f;	//パディング用(16byteアライン)

		DirectX::XMFLOAT3 emissiveColor{};
		float			  emissiveStrength = 1.0f;

	};

	//初期化に使用する
	bool CreateModelResource(const ModelComponent& model);
	bool CreateMeshResource( const ModelComponent::Mesh& mesh, MeshResource& resource);
	bool CreateVertexBuffer( const ModelComponent::Mesh& mesh, MeshResource& resource);
	bool CreateIndexBUffer(	 const ModelComponent::Mesh& mesh, MeshResource& resource);

	bool CreateConstantBuffers();

	//更新
	void UpdateTransformBuffer(const RenderComponent& renderComponent);
	void UpdateMaterialBuffer( const MaterialComponent::Material& material);

	//DX12
	ComPtr<ID3D12Device6> device;

	RootSignature rootSig;
	PipelineState pso;

	//外部の参照
	Camera& camera;

	//モデルリソース
	std::vector<MeshResource> meshResources;

	//定数バッファ
	ComPtr<ID3D12Resource> transformBuffer;
	ComPtr<ID3D12Resource> materialBuffer;

	TransformBuffer* mappedTransformBuffer = nullptr;
	MaterialBuffer*  mappedMaterialBuffer  = nullptr;

};