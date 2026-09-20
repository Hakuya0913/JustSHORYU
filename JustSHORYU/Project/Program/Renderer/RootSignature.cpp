#include"RootSignature.h"
#include<iterator>

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
	D3D12_ROOT_PARAMETER rootParameters[4]{};

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

	//Param3 : DiffuseTexture : t0
	D3D12_DESCRIPTOR_RANGE srvRange{};
	srvRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRange.NumDescriptors = 1;
	srvRange.BaseShaderRegister = 0;
	srvRange.RegisterSpace = 0;
	srvRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[3].DescriptorTable.pDescriptorRanges = &srvRange;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//RootSignatureDesc設定
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.NumParameters	= std::size(rootParameters);
	rootSigDesc.pParameters		= rootParameters;

	//StaticSampler設定
	D3D12_STATIC_SAMPLER_DESC staticSampler{};

	staticSampler.Filter			= D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSampler.AddressU			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressV			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.AddressW			= D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSampler.MipLODBias		= 0.0f;
	staticSampler.MaxAnisotropy		= 1;
	staticSampler.ComparisonFunc	= D3D12_COMPARISON_FUNC_ALWAYS;
	staticSampler.BorderColor		= D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	staticSampler.MinLOD			= 0.0f;
	staticSampler.MaxLOD			= D3D12_FLOAT32_MAX;
	staticSampler.ShaderRegister	= 0;
	staticSampler.RegisterSpace		= 0;
	staticSampler.ShaderVisibility	= D3D12_SHADER_VISIBILITY_PIXEL;

	rootSigDesc.NumStaticSamplers	= 1;
	rootSigDesc.pStaticSamplers		= &staticSampler;
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