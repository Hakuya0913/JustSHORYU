#include"GraphicsDevice.h"

bool GraphicsDevice::Init(HWND hwnd, UINT resolutionW, UINT resolutionH) {

	this->hwnd = hwnd;
	this->resolutionW = resolutionW;
	this->resolutionH = resolutionH;

	//各初期化処理を呼ぶ

	if (!CreateDevice())	return false;
	if (!CreateCmdQueue())	return false;
	if (!CreateSwapChain()) return false;
	if (!CreateCmdList())	return false;
	if (!CreateFence())		return false;

	CreateViewport();
	CreateScissorRect();

	if (!CreateRenderTarget()) return false;
	if (!CreateDepthStencil()) return false;

}

bool GraphicsDevice::CreateDevice() {

	HRESULT hr;

	hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(device.ReleaseAndGetAddressOf())
	);

	return SUCCEEDED(hr);

}

bool GraphicsDevice::CreateCmdQueue() {

	HRESULT hr;

	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask	= 0;

	hr = device->CreateCommandQueue(
		&desc,
		IID_PPV_ARGS(cmdQueue.ReleaseAndGetAddressOf())
	);

	return SUCCEEDED(hr);

}

bool GraphicsDevice::CreateSwapChain() {

	HRESULT hr;

	//DXGIファクトリー生成
	IDXGIFactory4* dxgiFactory = nullptr;

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

	if (FAILED(hr)) {

		return false;

	}

	//スワップチェイン設定
	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferDesc.Width					= resolutionW;
	desc.BufferDesc.Height					= resolutionH;
	desc.BufferDesc.RefreshRate.Numerator	= 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count					= 1;
	desc.SampleDesc.Quality					= 0;
	desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount						= ConstVal::D3D::BufferringCount;
	desc.OutputWindow						= hwnd;
	desc.Windowed							= TRUE;
	desc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//スワップチェイン生成
	IDXGISwapChain* dxgiSwapChain = nullptr;

	hr = dxgiFactory->CreateSwapChain(cmdQueue.Get(), &desc, &dxgiSwapChain);

	if (FAILED(hr)) {

		return false;

	}

	//IDXGISwapChain3を取得
	hr = dxgiSwapChain->QueryInterface(IID_PPV_ARGS(swapChain.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {

		return false;

	}

	//バックバッファ番号取得
	currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();

	dxgiFactory->Release();
	dxgiSwapChain->Release();

	return true;

}

bool GraphicsDevice::CreateCmdList() {

	HRESULT hr;

	//コマンドアロケータ生成
	for (size_t i = 0; i < ConstVal::D3D::BufferringCount; ++i) {

		hr = device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(cmdAllocator[i].ReleaseAndGetAddressOf())
		);

		if (FAILED(hr)) {

			return false;

		}

	}

	//コマンドリストの生成
	hr = device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator[currentBackBufferIndex].Get(),
		nullptr,
		IID_PPV_ARGS(&cmdList)
	);

	if (FAILED(hr)) {

		return false;

	}

	//コマンドリストは開かれている状態で作成されるので
	//いったん閉じる
	cmdList->Close();

	return true;

}

bool GraphicsDevice::CreateFence() {

	HRESULT hr;

	for (UINT i = 0; i < ConstVal::D3D::BufferringCount; ++i) {

		fenceValue[i] = 0;

	}

	hr = device->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {

		return false;

	}

	++fenceValue[currentBackBufferIndex];

	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	return (fenceEvent != nullptr);

}

void GraphicsDevice::CreateViewport() {

	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<float>(resolutionW);
	viewport.Height		= static_cast<float>(resolutionH);
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;

}

void GraphicsDevice::CreateScissorRect() {

	scissor.left	= 0;
	scissor.right	= resolutionW;
	scissor.top		= 0;
	scissor.bottom	= resolutionH;

}

bool GraphicsDevice::CreateRenderTarget() {

	HRESULT hr;

	//RTV用ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = ConstVal::D3D::BufferringCount;
	desc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = device->CreateDescriptorHeap(
		&desc,
		IID_PPV_ARGS(rtvHeap.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {

		return false;

	}

	//ディスクリプタサイズを取得
	rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < ConstVal::D3D::BufferringCount; ++i) {

		swapChain->GetBuffer(
			i,
			IID_PPV_ARGS(renderTargets[i].ReleaseAndGetAddressOf())
		);

		device->CreateRenderTargetView(
			renderTargets[i].Get(),
			nullptr,
			rtvHandle
		);

		rtvHandle.ptr += rtvDescriptorSize;

	}

	return true;

}

bool GraphicsDevice::CreateDepthStencil() {

	HRESULT hr;

	//DSV用ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = device->CreateDescriptorHeap(
		&heapDesc,
		IID_PPV_ARGS(dsvHeap.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {

		return false;

	}

	//ディスクリプタサイズの取得
	dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	//深度ステンシルバッファのクリア値設定
	D3D12_CLEAR_VALUE dsvClearValue;
	dsvClearValue.Format				= DXGI_FORMAT_D32_FLOAT;
	dsvClearValue.DepthStencil.Depth	= 1.0f;
	dsvClearValue.DepthStencil.Stencil	= 0;

	//VRAMにリソースを置くためのヒーププロパティ設定
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type					= D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask		= 1;
	heapProp.VisibleNodeMask		= 1;

	//DepthBuffer作成用のリソースを設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Alignment			= 0;
	resourceDesc.Width				= resolutionW;
	resourceDesc.Height				= resolutionH;
	resourceDesc.DepthOrArraySize	= 1;
	resourceDesc.MipLevels			= 1;
	resourceDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	resourceDesc.SampleDesc.Count	= 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	hr = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsvClearValue,
		IID_PPV_ARGS(depthStencilBuffer.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {

		return false;

	}

	//ディスクリプタ作成
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	device->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		nullptr,
		dsvHandle
	);

}

void GraphicsDevice::BeginFrame() {

	//レンダーターゲットの更新
	currentRenderTarget = renderTargets[currentBackBufferIndex].Get();

	//コマンド初期化
	cmdAllocator[currentBackBufferIndex]->Reset();
	cmdList->Reset(cmdAllocator[currentBackBufferIndex].Get(), nullptr);

	//ビューポート、シザー矩形を設定
	cmdList->RSSetViewports(1, &viewport);
	cmdList->RSSetScissorRects(1, &scissor);

	//現在フレームのRTVディスクリプタヒープの開始アドレスを取得
	auto currentRTVHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	currentRTVHandle.ptr += currentBackBufferIndex * rtvDescriptorSize;

	//深度ステンシルディスクリプタヒープの開始アドレス取得
	auto currentDSVHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	//リソースバリアの設定
	//バックバッファを描画可能な状態へ
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= currentRenderTarget;
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	cmdList->ResourceBarrier(1, &barrier);

	//レンダーターゲット設定
	cmdList->OMSetRenderTargets(
		1,
		&currentRTVHandle,
		FALSE,
		&currentDSVHandle
	);

	//レンダーターゲットをクリア
	cmdList->ClearRenderTargetView(
		currentRTVHandle,
		ConstVal::D3D::ClearColor,
		0,
		nullptr
	);

	//深度ステンシルビューをクリア
	cmdList->ClearDepthStencilView(
		currentDSVHandle,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr
	);

}

void GraphicsDevice::EndFrame() {

	//リソースバリア設定
	//バックバッファを表示状態へ
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags					= D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource	= currentRenderTarget;
	barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	cmdList->ResourceBarrier(1, &barrier);

	//コマンドの記録終了
	cmdList->Close();

	//コマンドの実行
	ID3D12CommandList* ppCmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, ppCmdLists);

	//スワップチェーンを切り替え
	swapChain->Present(1, 0);

	//描画完了を待つ
	WaitRender();

	//バックバッファ番号更新
	currentBackBufferIndex = swapChain->GetCurrentBackBufferIndex();

}

void GraphicsDevice::WaitRender() {

	HRESULT hr;

	const UINT64 fenceVal = fenceValue[currentBackBufferIndex];
	cmdQueue->Signal(fence.Get(), fenceVal);
	++fenceValue[currentBackBufferIndex];

	//次フレームの描画準備がまだなら待機
	if (fence->GetCompletedValue() < fenceVal) {

		hr = fence->SetEventOnCompletion(fenceVal, fenceEvent);

		if (FAILED(hr)) {

			return;

		}

		if (WAIT_OBJECT_0 != WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE)); {

			return;

		}

	}

}