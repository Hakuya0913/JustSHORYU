#pragma once

/*

RootSignature情報を管理する

*/

#include<d3d12.h>
#include<d3dcompiler.h>
#include"../Utility/ComPtr.h"

#pragma comment(lib,"d3dcompiler.lib")

class RootSignature
{
public:

	RootSignature();

	bool Create(ID3D12Device6* device);

	bool IsValid() const { return isValid; }

	ID3D12RootSignature* GetRootSignature() const { return rootSig.Get(); }

private:

	bool isValid = false;

	ComPtr<ID3D12RootSignature> rootSig;

};