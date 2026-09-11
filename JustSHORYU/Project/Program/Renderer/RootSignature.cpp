#include"RootSignature.h"

RootSignature::RootSignature()
{

	//特に処理なし

}

bool RootSignature::Create(ID3D12Device6* device)
{

	if (device == nullptr)
	{

		return false;

	}

	isValid = false;

	//RootParameter
	D3D12_ROOT_PARAMETER rootParameters[3]{};

	//Param0 : TransformBuffer : b0
	rootParameters[0].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[0].Descriptor.RegisterSpace	= 0;
	rootParameters[0].ShaderVisibility			= D3D12_SHADER_VISIBILITY_VERTEX;

	//Pram1 : View / Proj MatrixBuffer : b1
	rootParameters[1].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].Descriptor.ShaderRegister = 1;
	rootParameters[1].Descriptor.RegisterSpace	= 0;
	rootParameters[1].ShaderVisibility			= D3D12_SHADER_VISIBILITY_VERTEX;

	//Pram2 : MaterialBuffer : b2
	rootParameters[2].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[2].Descriptor.ShaderRegister = 2;
	rootParameters[2].Descriptor.RegisterSpace	= 0;
	rootParameters[2].ShaderVisibility			= D3D12_SHADER_VISIBILITY_PIXEL;

	//RootSignatureDesc設定
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters = 3;
	rootSigDesc.pParameters = rootParameters;

	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.pStaticSamplers = nullptr;

	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootSignatureをシリアライズ
	ComPtr<ID3DBlob> signatureBlob;
	ComPtr<ID3DBlob> errorBlob;

	HRESULT hr;

	hr = D3D12SerializeRootSignature(
		&rootSigDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		signatureBlob.GetAddressOf(),
		errorBlob.GetAddressOf()
	);

	if (FAILED(hr))
	{

		return false;

	}

	//RootSignatureを生成
	hr = device->CreateRootSignature(
		0,
		signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSig.GetAddressOf())
	);

	if (FAILED(hr))
	{

		return false;

	}

	isValid = true;

	return true;

}