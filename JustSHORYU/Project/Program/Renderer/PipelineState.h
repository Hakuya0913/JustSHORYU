#pragma once

/*

PipelineStateを管理

*/

#include<d3d12.h>
#include<d3dcompiler.h>
#include<string>
#include"../Utility/ComPtr.h"

#pragma comment(lib,"d3dcompiler.lib")

class PipelineState
{
public:

	PipelineState();

	//PSO設定
	void SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& layout);
	void SetRootSignature(ID3D12RootSignature* rootSig);

	bool SetVS(const std::wstring& filePath);
	bool SetPS(const std::wstring& filePath);

	//PSO生成
	bool Create(ID3D12Device6* device);

private:

	//Shader読み込み
	bool LoadShader(const std::wstring& filePath, ComPtr<ID3DBlob>& shaderBlob);

	bool isValid = false;

	//PSO設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

	//生成されたPSO
	ComPtr<ID3D12PipelineState> pso;

	//Shader
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;

};