#pragma once

/*

FBXモデルの表示

*/

#include<d3d12.h>
#include"../Utility/ComPtr.h"
#include"../Component/RenderComponent.h"

class Camera;
class LightManager;

class ModelRenderer
{
public:

	ModelRenderer(
		ID3D12Device6* device,
		Camera& camera,
		LightManager& lightManager
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

	//DX12リソースの作成
	bool CreateRootSig();
	bool CreatePSO();

	ComPtr<ID3D12Device> device;

	ComPtr<ID3D12RootSignature> rootSig;
	ComPtr<ID3D12PipelineState> pso;

	//外部の参照
	Camera& camera;
	LightManager& lightManager;

};