#include"ModelRenderer.h"

using namespace DirectX;

bool ModelRenderer::Init(ID3D12Device6* device, ID3D12GraphicsCommandList* cmdList, Camera& camera)
{

	this->device = device;
	this->cmdList = cmdList;
	this->camera = &camera;

	if (device == nullptr)
	{
		return false;
	}

	//RootSig作成
	if (rootSig.Create(device) == false)
	{
		return false;
	}

	//PSO作成

	//PSOに渡すInputLayout作成
	D3D12_INPUT_ELEMENT_DESC inputElement[4];

	inputElement[0].SemanticName		 = "POSITION";
	inputElement[0].SemanticIndex		 = 0;
	inputElement[0].Format				 = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElement[0].InputSlot			 = 0;
	inputElement[0].AlignedByteOffset	 = 0;
	inputElement[0].InputSlotClass		 = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputElement[0].InstanceDataStepRate = 0;

	inputElement[1].SemanticName		 = "NORMAL";
	inputElement[1].SemanticIndex		 = 0;
	inputElement[1].Format				 = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElement[1].InputSlot			 = 0;
	inputElement[1].AlignedByteOffset	 = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElement[1].InputSlotClass		 = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputElement[1].InstanceDataStepRate = 0;

	inputElement[2].SemanticName			= "TEXCOORD";
	inputElement[2].SemanticIndex			= 0;
	inputElement[2].Format					= DXGI_FORMAT_R32G32_FLOAT;
	inputElement[2].InputSlot				= 0;
	inputElement[2].AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
	inputElement[2].InputSlotClass			= D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputElement[2].InstanceDataStepRate	= 0;

	inputElement[3].SemanticName		 = "TANGENT";
	inputElement[3].SemanticIndex		 = 0;
	inputElement[3].Format				 = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElement[3].InputSlot			 = 0;
	inputElement[3].AlignedByteOffset	 = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElement[3].InputSlotClass		 = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	inputElement[3].InstanceDataStepRate = 0;

	//InputElementをInputLayoutにまとめる
	D3D12_INPUT_LAYOUT_DESC inputLayout{};
	
	inputLayout.pInputElementDescs = inputElement;
	inputLayout.NumElements = _countof(inputElement);

	pso.SetInputLayout(inputLayout);
	pso.SetRootSignature(rootSig.GetRootSignature());
	pso.SetVS(L"CSO/ModelVS.cso");
	pso.SetPS(L"CSO/ModelPS.cso");

	if (pso.Create(device) == false)
	{
		return false;
	}

	if (CreateConstantBuffers() == false)
	{
		return false;
	}

	return true;

}

bool ModelRenderer::CreateModelResource(const ModelComponent& model)
{

	meshResources.clear();
	meshResources.reserve(model.GetMeshCount());

	for (size_t i = 0; i < model.GetMeshCount(); ++i)
	{

		const Mesh& mesh = model.GetMesh(i);

		MeshResource resource{};

		if (CreateMeshResource(mesh, resource) == false)
		{

			meshResources.clear();
			return false;

		}

		meshResources.emplace_back(std::move(resource));

	}

	return true;

}

bool ModelRenderer::CreateMeshResource(const Mesh& mesh, MeshResource& resource)
{

	if (mesh.vertices.empty()) return false;
	if (mesh.indices.empty())  return false;

	if (CreateVertexBuffer(mesh, resource) == false) return false;
	if (CreateIndexBuffer( mesh, resource) == false) return false;

	resource.indexCount    = mesh.indices.size();
	resource.materialIndex = mesh.materialIndex;

	return true;

}

bool ModelRenderer::CreateVertexBuffer(const Mesh& mesh, MeshResource& resource)
{

	const UINT bufferSize = sizeof(Vertex) * mesh.vertices.size();

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width				= bufferSize;
	resourceDesc.Height				= 1;
	resourceDesc.DepthOrArraySize	= 1;
	resourceDesc.MipLevels			= 1;
	resourceDesc.SampleDesc.Count	= 1;
	resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr;

	hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource.vertexBuffer.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	void* mappedData = nullptr;

	D3D12_RANGE readRange{};
	readRange.Begin = 0;
	readRange.End   = 0;

	hr = resource.vertexBuffer->Map(0, &readRange, &mappedData);

	if (FAILED(hr)) return false;

	memcpy(mappedData, mesh.vertices.data(), bufferSize);

	resource.vertexBuffer->Unmap(0, nullptr);

	//VertexBufferView
	resource.vertexBufferView.BufferLocation = resource.vertexBuffer->GetGPUVirtualAddress();
	resource.vertexBufferView.SizeInBytes = bufferSize;
	resource.vertexBufferView.StrideInBytes = sizeof(Vertex);

	return true;

}

bool ModelRenderer::CreateIndexBuffer(const Mesh& mesh, MeshResource& resource)
{

	const UINT bufferSize = sizeof(uint32_t) * mesh.indices.size();

	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = bufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr;

	hr = device->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(resource.indexBuffer.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	void* mappedData = nullptr;

	D3D12_RANGE readRange{};
	readRange.Begin = 0;
	readRange.End	= 0;

	hr = resource.indexBuffer->Map(0, &readRange, &mappedData);

	if (FAILED(hr))
	{
		return false;
	}

	memcpy(mappedData, mesh.indices.data(), bufferSize);

	resource.indexBuffer->Unmap(0, nullptr);

	//IndexBufferView
	resource.indexBufferView.BufferLocation = resource.indexBuffer->GetGPUVirtualAddress();
	resource.indexBufferView.SizeInBytes = bufferSize;
	resource.indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	return true;
 
}

bool ModelRenderer::CreateConstantBuffers()
{

	//TransformBuffer
	{

		const UINT bufferSize = AlignConstantBufferSize(sizeof(TransformBuffer));

		D3D12_HEAP_PROPERTIES heapProps{};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = bufferSize;
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags  = D3D12_RESOURCE_FLAG_NONE;

		HRESULT hr;

		hr = device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(transformBuffer.GetAddressOf())
		);

		if (FAILED(hr)) return false;

		void* mappedData = nullptr;

		//readRange部分(第二引数)がnullptr = 読み取りしない
		hr = transformBuffer->Map(0, nullptr, &mappedData);

		if (FAILED(hr)) return false;

		mappedTransformBuffer = static_cast<TransformBuffer*>(mappedData);

	}

	//Material
	{

		const UINT bufferSize = AlignConstantBufferSize(sizeof(MaterialBuffer));

		D3D12_HEAP_PROPERTIES heapProps{};
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment			= 0;
		resourceDesc.Width				= bufferSize;
		resourceDesc.Height				= 1;
		resourceDesc.DepthOrArraySize	= 1;
		resourceDesc.MipLevels			= 1;
		resourceDesc.Format				= DXGI_FORMAT_UNKNOWN;
		resourceDesc.SampleDesc.Count	= 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

		HRESULT hr;

		hr = device->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(materialBuffer.GetAddressOf())
		);

		if (FAILED(hr)) return false;

		void* mappedData = nullptr;

		hr = materialBuffer->Map(0, nullptr, &mappedData);

		if (FAILED(hr)) return false;

		mappedMaterialBuffer = static_cast<MaterialBuffer*>(mappedData);

	}

	return true;

}

bool ModelRenderer::CreateTextureResources(const ModelComponent& model)
{

	textureResources.clear();

	textureResources.reserve(
		model.GetEmbeddedTextureCount());

	for (uint32_t i = 0; i < model.GetEmbeddedTextureCount(); ++i)
	{
		if (!CreateTextureResource(model.GetEmbeddedTexture(i), i))
		{
			textureResources.clear();
			return false;
		}
	}

	return true;

}

bool ModelRenderer::CreateTextureResource(const EmbeddedTexture& embeddedTexture, uint32_t embeddedTextureIndex)
{

	std::vector<uint8_t> pixelData;

	UINT width = 0;
	UINT height = 0;
	UINT rowPitch = 0;

	if (!DecodeEmbeddedTexture(
		embeddedTexture,
		pixelData,
		width,
		height,
		rowPitch))
	{
		return false;
	}

	TextureResource resource{};

	if (CreateTextureUploadResource(
		pixelData,
		width,
		height,
		rowPitch,
		resource) == false)
	{
		return false;
	}

	resource.embeddedTextureIndex =
		embeddedTextureIndex;

	textureResources.emplace_back(
		std::move(resource));

	return true;

}

bool ModelRenderer::DecodeEmbeddedTexture(
	const EmbeddedTexture& embeddedTexture,
	std::vector<uint8_t>& pixelData,
	UINT& width, UINT& height, UINT& rowPitch
)
{

	if (embeddedTexture.data.empty())
	{
		return false;
	}

	HRESULT hr = CoInitializeEx(
		nullptr,
		COINIT_MULTITHREADED);

	bool needUninitialize =
		SUCCEEDED(hr);

	if (FAILED(hr) &&
		hr != RPC_E_CHANGED_MODE)
	{
		return false;
	}

	ComPtr<IWICImagingFactory> factory;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(factory.GetAddressOf()));

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICStream> stream;

	hr = factory->CreateStream(
		stream.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = stream->InitializeFromMemory(
		embeddedTexture.data.data(),
		static_cast<DWORD>(
			embeddedTexture.data.size()));

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICBitmapDecoder> decoder;

	hr = factory->CreateDecoderFromStream(
		stream.Get(),
		nullptr,
		WICDecodeMetadataCacheOnLoad,
		decoder.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICBitmapFrameDecode> frame;

	hr = decoder->GetFrame(
		0,
		frame.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICFormatConverter> converter;

	hr = factory->CreateFormatConverter(
		converter.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0,
		WICBitmapPaletteTypeCustom);

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = converter->GetSize(
		&width,
		&height);

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	rowPitch = width * 4;

	pixelData.resize(
		static_cast<size_t>(rowPitch) * height);

	hr = converter->CopyPixels(
		nullptr,
		rowPitch,
		static_cast<UINT>(pixelData.size()),
		pixelData.data());

	if (needUninitialize)
	{
		CoUninitialize();
	}

	return SUCCEEDED(hr);

}

bool ModelRenderer::CreateTextureUploadResource(
	const std::vector<uint8_t>& pixelData,
	UINT width, UINT height, UINT rowPitch,
	TextureResource& resource
)
{

	if (device == nullptr || cmdList == nullptr)
	{
		return false;
	}

	D3D12_RESOURCE_DESC textureDesc{};

	textureDesc.Dimension =
		D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;

	textureDesc.Format =
		DXGI_FORMAT_R8G8B8A8_UNORM;

	textureDesc.SampleDesc.Count = 1;

	textureDesc.Layout =
		D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_HEAP_PROPERTIES defaultHeap{};

	defaultHeap.Type =
		D3D12_HEAP_TYPE_DEFAULT;

	HRESULT hr;

	hr = device->CreateCommittedResource(
		&defaultHeap,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(resource.texture.GetAddressOf())
		);

	if (FAILED(hr))
	{
		return false;
	}

	UINT64 uploadSize = 0;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};

	device->GetCopyableFootprints(
		&textureDesc,
		0,
		1,
		0,
		&footprint,
		nullptr,
		nullptr,
		&uploadSize);

	D3D12_RESOURCE_DESC uploadDesc{};

	uploadDesc.Dimension =
		D3D12_RESOURCE_DIMENSION_BUFFER;

	uploadDesc.Width = uploadSize;
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;

	uploadDesc.Format =
		DXGI_FORMAT_UNKNOWN;

	uploadDesc.SampleDesc.Count = 1;

	uploadDesc.Layout =
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES uploadHeap{};

	uploadHeap.Type =
		D3D12_HEAP_TYPE_UPLOAD;

	ComPtr<ID3D12Resource> uploadBuffer;

	if (FAILED(
		device->CreateCommittedResource(
			&uploadHeap,
			D3D12_HEAP_FLAG_NONE,
			&uploadDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(
				uploadBuffer.GetAddressOf()))))
	{
		return false;
	}

	void* mappedData = nullptr;

	if (FAILED(
		uploadBuffer->Map(
			0,
			nullptr,
			&mappedData)))
	{
		return false;
	}

	uint8_t* destination =
		static_cast<uint8_t*>(mappedData);

	for (UINT y = 0; y < height; ++y)
	{
		std::memcpy(
			destination +
			footprint.Offset +
			static_cast<size_t>(
				y * footprint.Footprint.RowPitch),

			pixelData.data() +
			static_cast<size_t>(
				y * rowPitch),

			rowPitch);
	}

	uploadBuffer->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION dst{};

	dst.pResource =
		resource.texture.Get();

	dst.Type =
		D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;

	dst.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION src{};

	src.pResource =
		uploadBuffer.Get();

	src.Type =
		D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;

	src.PlacedFootprint =
		footprint;

	cmdList->CopyTextureRegion(
		&dst,
		0,
		0,
		0,
		&src,
		nullptr);

	D3D12_RESOURCE_BARRIER barrier{};

	barrier.Type =
		D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	barrier.Transition.pResource =
		resource.texture.Get();

	barrier.Transition.Subresource =
		D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	barrier.Transition.StateBefore =
		D3D12_RESOURCE_STATE_COPY_DEST;

	barrier.Transition.StateAfter =
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	cmdList->ResourceBarrier(
		1,
		&barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =
		srvHeap->GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr +=
		static_cast<SIZE_T>(
			textureResources.size() *
			srvDescriptorSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format =
		DXGI_FORMAT_R8G8B8A8_UNORM;

	srvDesc.ViewDimension =
		D3D12_SRV_DIMENSION_TEXTURE2D;

	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		resource.texture.Get(),
		&srvDesc,
		cpuHandle);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
		srvHeap->GetGPUDescriptorHandleForHeapStart();

	gpuHandle.ptr +=
		static_cast<UINT64>(
			textureResources.size() *
			srvDescriptorSize);

	resource.srvHandle = gpuHandle;

	return true;

}

bool ModelRenderer::DecodeEmbeddedTexture(
	const EmbeddedTexture& embeddedTexture,
	std::vector<uint8_t>& pixelData,
	UINT& width, UINT& height, UINT& rowPitch
)
{

	if (embeddedTexture.data.empty())
	{
		return false;
	}

	HRESULT hr = CoInitializeEx(
		nullptr,
		COINIT_MULTITHREADED);

	bool needUninitialize =
		SUCCEEDED(hr);

	if (FAILED(hr) &&
		hr != RPC_E_CHANGED_MODE)
	{
		return false;
	}

	ComPtr<IWICImagingFactory> factory;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(factory.GetAddressOf()));

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICStream> stream;

	hr = factory->CreateStream(
		stream.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = stream->InitializeFromMemory(
		embeddedTexture.data.data(),
		static_cast<DWORD>(embeddedTexture.data.size())
	);

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICBitmapDecoder> decoder;

	hr = factory->CreateDecoderFromStream(
		stream.Get(),
		nullptr,
		WICDecodeMetadataCacheOnLoad,
		decoder.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICBitmapFrameDecode> frame;

	hr = decoder->GetFrame(
		0,
		frame.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	ComPtr<IWICFormatConverter> converter;

	hr = factory->CreateFormatConverter(
		converter.GetAddressOf());

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0,
		WICBitmapPaletteTypeCustom);

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	hr = converter->GetSize(
		&width,
		&height);

	if (FAILED(hr))
	{
		if (needUninitialize)
		{
			CoUninitialize();
		}

		return false;
	}

	rowPitch = width * 4;

	pixelData.resize(
		static_cast<size_t>(rowPitch) * height);

	hr = converter->CopyPixels(
		nullptr,
		rowPitch,
		static_cast<UINT>(pixelData.size()),
		pixelData.data());

	if (needUninitialize)
	{
		CoUninitialize();
	}

	return SUCCEEDED(hr);

}

bool ModelRenderer::CreateTextureUploadResource(
	const std::vector<uint8_t>& pixelData,
	UINT width, UINT height, UINT rowPitch,
	TextureResource& resource
)
{

	if (device == nullptr || cmdList == nullptr)
	{
		return false;
	}

	D3D12_RESOURCE_DESC textureDesc{};

	textureDesc.Dimension =
		D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_HEAP_PROPERTIES defaultHeap{};
	defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;

	HRESULT hr;

	hr = device->CreateCommittedResource(
		&defaultHeap,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(resource.texture.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	UINT64 uploadSize = 0;

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint{};

	device->GetCopyableFootprints(
		&textureDesc,
		0,
		1,
		0,
		&footprint,
		nullptr,
		nullptr,
		&uploadSize);

	D3D12_RESOURCE_DESC uploadDesc{};

	uploadDesc.Dimension =
		D3D12_RESOURCE_DIMENSION_BUFFER;

	uploadDesc.Width = uploadSize;
	uploadDesc.Height = 1;
	uploadDesc.DepthOrArraySize = 1;
	uploadDesc.MipLevels = 1;

	uploadDesc.Format =
		DXGI_FORMAT_UNKNOWN;

	uploadDesc.SampleDesc.Count = 1;

	uploadDesc.Layout =
		D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	D3D12_HEAP_PROPERTIES uploadHeap{};

	uploadHeap.Type =
		D3D12_HEAP_TYPE_UPLOAD;

	ComPtr<ID3D12Resource> uploadBuffer;

	hr = device->CreateCommittedResource(
		&uploadHeap,
		D3D12_HEAP_FLAG_NONE,
		&uploadDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf())
	);

	if (FAILED(hr))
	{
		return false;
	}

	void* mappedData = nullptr;

	hr = uploadBuffer->Map(
		0,
		nullptr,
		&mappedData
	);

	if (FAILED(hr))
	{
		return false;
	}

	uint8_t* destination = static_cast<uint8_t*>(mappedData);

	for (UINT y = 0; y < height; ++y)
	{
		std::memcpy(
			destination + footprint.Offset + static_cast<size_t>(y * footprint.Footprint.RowPitch),
			pixelData.data() + static_cast<size_t>(y * rowPitch),
			rowPitch
		);
	}

	uploadBuffer->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION dst{};
	dst.pResource = resource.texture.Get();
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = 0;

	D3D12_TEXTURE_COPY_LOCATION src{};
	src.pResource = uploadBuffer.Get();
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = footprint;

	cmdList->CopyTextureRegion(
		&dst,
		0,
		0,
		0,
		&src,
		nullptr
	);

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = resource.texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	cmdList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle =	srvHeap->GetCPUDescriptorHandleForHeapStart();

	cpuHandle.ptr += static_cast<SIZE_T>(textureResources.size() * srvDescriptorSize);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(
		resource.texture.Get(),
		&srvDesc,
		cpuHandle
	);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

	gpuHandle.ptr += static_cast<UINT64>(textureResources.size() * srvDescriptorSize);

	resource.srvHandle = gpuHandle;

	return true;

}

//Update

void ModelRenderer::UpdateTransformBuffer(const RenderComponent& renderComponent)
{

	const XMMATRIX world = renderComponent.GetTransform().GetWorldMatrix();
	const XMMATRIX view = camera->GetrViewMatrix();
	const XMMATRIX proj = camera->GetProjectionMatrix();

	XMStoreFloat4x4(&mappedTransformBuffer->world, XMMatrixTranspose(world));
	XMStoreFloat4x4(&mappedTransformBuffer->view, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mappedTransformBuffer->proj, XMMatrixTranspose(proj));

}

void ModelRenderer::UpdateMaterialBuffer(const Material& material)
{

	mappedMaterialBuffer->baseColor			= material.baseColor;
	mappedMaterialBuffer->metalic			= material.metallic;
	mappedMaterialBuffer->roughness			= material.roughness;
	mappedMaterialBuffer->ambientOcclusion	= material.ambientOcclusion;
	mappedMaterialBuffer->emissiveColor		= material.emissiveColor;
	mappedMaterialBuffer->emissiveStrength	= material.emissiveStrength;

}

void ModelRenderer::Render(ID3D12GraphicsCommandList* cmdList, RenderComponent& renderComponent)
{

	if (cmdList == nullptr)
	{
		return;
	}

	const ModelComponent& model = renderComponent.GetModel();

	//GPUリソースがまだ作成されていない場合
	if (meshResources.size() != model.GetMeshCount())
	{

		if (CreateModelResource(model) == false)
		{
			return;
		}

	}

	//Transform更新
	UpdateTransformBuffer(renderComponent);

	//コマンドリストに要素をセット
	cmdList->SetGraphicsRootSignature(rootSig.GetRootSignature());
	cmdList->SetPipelineState(pso.GetPSO());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->SetGraphicsRootConstantBufferView(0, transformBuffer->GetGPUVirtualAddress());

	for (const MeshResource& mesh : meshResources)
	{

		/*
		形状の描画のみ

		//Materialのインデックスが範囲外ならスキップ
		if (mesh.materialIndex >= model.GetMaterialCount())
		{
			continue;
		}

		cmdList->IASetVertexBuffers(0, 1, &mesh.vertexBufferView);
		cmdList->IASetIndexBuffer(&mesh.indexBufferView);
		cmdList->DrawIndexedInstanced(mesh.indexCount, 1, 0, 0, 0);
		
		*/

	}

}

constexpr UINT ModelRenderer::AlignConstantBufferSize(UINT size)
{

	return (size + 255) & ~255;

}
