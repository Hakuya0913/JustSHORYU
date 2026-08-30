#include"DebugRenderer.h"

bool DebugRenderer::Init() {

	GraphicsDevice& graphicsDevice = GraphicsDevice::GetInstance();

	device = graphicsDevice.GetDevice();
	cmdList = graphicsDevice.GetCmdList();

	//取得の確認
	if (device == nullptr || cmdList == nullptr) {
		return false;
	}

	//各生成関数の呼び出しとチェック
	if (!CompileShaders()) return false;
	if (!CreateRootSig()) return false;
	if (!CreateInputLayout()) return false;
	if (!CreatePSO()) return false;
	if (!CreateVertexBuffer()) return false;

	return true;

}

void DebugRenderer::Begin() {

	//現時点で処理なし

}

void DebugRenderer::End() {

	//現時点で処理なし

}

void DebugRenderer::DrawTriangle() {

	//PSO設定
	cmdList->SetPipelineState(pso.Get());

	//RootSig設定
	cmdList->SetGraphicsRootSignature(rootSig.Get());

	//PrimitiveTopology
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//VertexBuffer設定
	cmdList->IASetVertexBuffers(
		0,
		1,
		&vertexBufferView
	);

	//描画
	cmdList->DrawInstanced(
		vertexCount,
		1,
		0,
		0
	);

}

bool DebugRenderer::CreateRootSig() {

	HRESULT hr;

	//RootSig設定
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc{};

	rootSigDesc.NumParameters = 0;
	rootSigDesc.pParameters = nullptr;

	//StaticSmaplerなし
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.pStaticSamplers = nullptr;

	//InputAssemblerを使用可能にする
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//RootSigをシリアライズするためのデータ
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(
		&rootSigDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		rootSigBlob.ReleaseAndGetAddressOf(),
		errorBlob.ReleaseAndGetAddressOf()
	);

	if (FAILED(hr)) {

		return false;

	}

	//RootSigを生成
	hr = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(rootSig.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {
	
		return false;

	}

	return true;

}

bool DebugRenderer::CreateInputLayout() {
	
	//Shader側で定義した入力構造体と紐づける
	//POSITIONと紐づける設定
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;

	//COLORと紐づける設定
	inputLayout[1].SemanticName = "COLOR";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;

	//InputLayoutの数を設定
	inputLayoutCount = _countof(inputLayout);

	return true;

}

bool DebugRenderer::CreatePSO() {

	assert(device != nullptr);
	assert(rootSig != nullptr);
	assert(vs != nullptr);
	assert(ps != nullptr);

	assert(vs->GetBufferPointer() != nullptr);
	assert(ps->GetBufferPointer() != nullptr);

	assert(vs->GetBufferSize() > 0);
	assert(ps->GetBufferSize() > 0);

	HRESULT hr;
	
	//PSO設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	//RootSig設定
	psoDesc.pRootSignature = rootSig.Get();

	//VS設定
	psoDesc.VS.pShaderBytecode = vs->GetBufferPointer();
	psoDesc.VS.BytecodeLength = vs->GetBufferSize();

	//PS設定
	psoDesc.PS.pShaderBytecode = ps->GetBufferPointer();
	psoDesc.PS.BytecodeLength = ps->GetBufferSize();

	//InputLayout設定
	psoDesc.InputLayout.pInputElementDescs = inputLayout;
	psoDesc.InputLayout.NumElements = inputLayoutCount;

	//primitiveTopologyType設定
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//RasterizerState設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;;

	psoDesc.RasterizerState = rasterizerDesc;

	//BlendState設定
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].LogicOpEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psoDesc.BlendState = blendDesc;

	//DepthStencilState設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState = depthStencilDesc;

	//RenderTarget設定
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//DepthStencilFormat設定
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//SampleDesc設定
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;

	psoDesc.SampleMask = UINT_MAX;

	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.NodeMask = 0;
	psoDesc.StreamOutput = {};
	psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	psoDesc.CachedPSO = {};

	//PSO生成
	hr = device->CreateGraphicsPipelineState(
		&psoDesc, 
		IID_PPV_ARGS(pso.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {
	
		return false;

	}

	return true;

}

bool DebugRenderer::CreateVertexBuffer() {

	HRESULT hr;

	//三角形の頂点データ
	Vertex vertices[] =
	{
		{

			DirectX::SimpleMath::Vector3(0.0f,0.5f,0.0f),
			DirectX::SimpleMath::Color(1.0f,0.0f,0.0f,1.0f)

		},

		{

			DirectX::SimpleMath::Vector3(0.5f,-0.5f,0.0f),
			DirectX::SimpleMath::Color(1.0f,0.0f,0.0f,1.0f)

		},

		{

			DirectX::SimpleMath::Vector3(-0.5f,-0.5f,0.0f),
			DirectX::SimpleMath::Color(1.0f,0.0f,0.0f,1.0f)
		
		}

	};

	vertexCount = 3;

	//VertexBufferのサイズ
	const UINT vertexBufferSize = sizeof(vertices);

	//Heap設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	//Resource設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = vertexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//VertexBuffer作成
	hr = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(vertexBuffer.ReleaseAndGetAddressOf())
	);

	if (FAILED(hr)) {

		return false;

	}

	//CPUからVertexBufferへコピー
	void* mappedData = nullptr;

	D3D12_RANGE readRange{};
	readRange.Begin = 0;
	readRange.End = 0;

	hr = vertexBuffer->Map(
		0,
		&readRange,
		&mappedData
	);

	if (FAILED(hr)) {

		return false;

	}

	memcpy(mappedData, vertices, vertexBufferSize);

	vertexBuffer->Unmap(0, nullptr);

	//VertexBufferView設定
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = vertexBufferSize;
	vertexBufferView.StrideInBytes = sizeof(Vertex);

	return true;

}

bool DebugRenderer::CompileShaders() {

	HRESULT hr;

	//VS読み込み
	hr = D3DCompileFromFile(
		L"Resource/Program/Shader/DebugVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		0,
		0,
		vs.ReleaseAndGetAddressOf(),
		nullptr
	);

	if (FAILED(hr)) {

		return false;

	}

	//PS読み込み
	hr = D3DCompileFromFile(
		L"Resource/Program/Shader/DebugPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		ps.ReleaseAndGetAddressOf(),
		nullptr
	);

	if (FAILED(hr)) {

		return false;

	}

	return true;

}