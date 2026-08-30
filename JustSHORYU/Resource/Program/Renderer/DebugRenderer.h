#pragma once

/*

可視化をするためのレンダラー(入力検知など)
三角形を描画する

*/

#include<d3d12.h>
#include"../Core/GraphicsDevice.h"
#include"../Utility/ComPtr.h"
#include<DirectXMath.h>
#include<SimpleMath.h>
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

class DebugRenderer {
public:

	bool Init();

	void Begin();
	void End();

	void DrawTriangle();

	//Getter
	ID3D12PipelineState* GetPSO() const { return pso.Get(); }
	ID3D12RootSignature* GetRootSig() const { return rootSig.Get(); }
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const { return vertexBufferView; }
	UINT GetVertexCount() const { return vertexCount; }

private:

	bool CreateRootSig();
	bool CreateInputLayout();
	bool CreatePSO();
	bool CreateVertexBuffer();

	bool CompileShaders();

	ID3D12Device6* device;
	ID3D12GraphicsCommandList* cmdList;

	//RootSig
	ComPtr<ID3D12RootSignature> rootSig = nullptr;

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputLayout[2]{};
	UINT inputLayoutCount = 0;

	//PSO
	ComPtr<ID3D12PipelineState> pso = nullptr;

	//Shader
	ComPtr<ID3DBlob> vs = nullptr;
	ComPtr<ID3DBlob> ps = nullptr;

	//VertexBuffer
	ComPtr<ID3D12Resource> vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	UINT vertexCount = 0;

};

struct Vertex {

	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Color color;

};