#pragma once

/*

GPU制御クラスのヘッダー
初期化～各フレームの開始/終了時処理
同期など行う

*/

#include<Windows.h>
#include<d3d12.h>
#include<dxgi.h>
#include<dxgi1_4.h>

#include"../Utility/ComPtr.h"
#include"ConstantCore.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class GraphicsDevice {
public:

	static GraphicsDevice& GetInstance();

	bool Init(HWND hwnd, UINT resolutionW = ConstVal::D3D::ResolutionW, UINT resolutionH = ConstVal::D3D::ResolutionH);

	void BeginFrame();
	void EndFrame();

	//Getter
	ID3D12Device6* GetDevice()		const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCmdList()	const { return cmdList.Get(); }
	UINT						GetCurrentBackBufferIndex() const { return currentBackBufferIndex; }

private:	//描画に使用するDX12のオブジェクト群

	HWND hwnd;
	UINT resolutionW = 0;
	UINT resolutionH = 0;
	UINT currentBackBufferIndex = 0;

	bool CreateDevice();
	bool CreateCmdQueue();
	bool CreateSwapChain();
	bool CreateCmdList();
	bool CreateFence();
	void CreateViewport();
	void CreateScissorRect();

	ComPtr<ID3D12Device6>				device = nullptr;
	ComPtr<ID3D12CommandQueue>			cmdQueue = nullptr;
	ComPtr<IDXGISwapChain3>				swapChain = nullptr;
	ComPtr<ID3D12CommandAllocator>		cmdAllocator[ConstVal::D3D::BufferringCount]{ nullptr };
	ComPtr<ID3D12GraphicsCommandList>	cmdList = nullptr;
	HANDLE								fenceEvent = nullptr;
	ComPtr<ID3D12Fence>					fence = nullptr;
	UINT64								fenceValue[ConstVal::D3D::BufferringCount];
	D3D12_VIEWPORT						viewport;
	D3D12_RECT							scissor;

private:	//描画に使用するオブジェクト群

	bool CreateRenderTarget();
	bool CreateDepthStencil();

	UINT rtvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	ComPtr<ID3D12Resource>		 renderTargets[ConstVal::D3D::BufferringCount]{ nullptr };

	UINT dsvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	ComPtr<ID3D12Resource>		 depthStencilBuffer = nullptr;

private:	//描画ループの中で使用するもの

	void WaitRender();

	ID3D12Resource* currentRenderTarget = nullptr;

private:

	GraphicsDevice() = default;

};