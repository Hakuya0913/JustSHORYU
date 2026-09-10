#include"PipelineState.h"

PipelineState::PipelineState()
{

    desc = {};

    // デフォルト設定に合わせる

    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

    // Rasterizer
    desc.RasterizerState.FillMode               = D3D12_FILL_MODE_SOLID;
    desc.RasterizerState.CullMode               = D3D12_CULL_MODE_BACK;
    desc.RasterizerState.FrontCounterClockwise  = FALSE;
    desc.RasterizerState.DepthBias              = D3D12_DEFAULT_DEPTH_BIAS;
    desc.RasterizerState.DepthBiasClamp         = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
    desc.RasterizerState.SlopeScaledDepthBias   = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
    desc.RasterizerState.DepthClipEnable        = TRUE;
    desc.RasterizerState.MultisampleEnable      = FALSE;
    desc.RasterizerState.AntialiasedLineEnable  = FALSE;
    desc.RasterizerState.ForcedSampleCount      = 0;
    desc.RasterizerState.ConservativeRaster     = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    // Blend
    desc.BlendState.AlphaToCoverageEnable                   = FALSE;
    desc.BlendState.IndependentBlendEnable                  = FALSE;
    desc.BlendState.RenderTarget[0].BlendEnable             = FALSE;
    desc.BlendState.RenderTarget[0].LogicOpEnable           = FALSE;
    desc.BlendState.RenderTarget[0].SrcBlend                = D3D12_BLEND_ONE;
    desc.BlendState.RenderTarget[0].DestBlend               = D3D12_BLEND_ZERO;
    desc.BlendState.RenderTarget[0].BlendOp                 = D3D12_BLEND_OP_ADD;
    desc.BlendState.RenderTarget[0].SrcBlendAlpha           = D3D12_BLEND_ONE;
    desc.BlendState.RenderTarget[0].DestBlendAlpha          = D3D12_BLEND_ZERO;
    desc.BlendState.RenderTarget[0].BlendOpAlpha            = D3D12_BLEND_OP_ADD;
    desc.BlendState.RenderTarget[0].LogicOp                 = D3D12_LOGIC_OP_NOOP;
    desc.BlendState.RenderTarget[0].RenderTargetWriteMask   = D3D12_COLOR_WRITE_ENABLE_ALL;

    // Depth / Stencil
    desc.DepthStencilState.DepthEnable      = TRUE;
    desc.DepthStencilState.DepthWriteMask   = D3D12_DEPTH_WRITE_MASK_ALL;
    desc.DepthStencilState.DepthFunc        = D3D12_COMPARISON_FUNC_LESS;
    desc.DepthStencilState.StencilEnable    = FALSE;
    desc.DepthStencilState.StencilReadMask  = D3D12_DEFAULT_STENCIL_READ_MASK;
    desc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

    // Render Target
    desc.NumRenderTargets   = 1;
    desc.RTVFormats[0]      = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.DSVFormat          = DXGI_FORMAT_D32_FLOAT;

    // MSAA
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;

}

void PipelineState::SetInputLayout(const D3D12_INPUT_LAYOUT_DESC& layout)
{

    desc.InputLayout = layout;

}

void PipelineState::SetRootSignature(ID3D12RootSignature* rootSignature)
{

    desc.pRootSignature = rootSignature;

}

void PipelineState::SetVS(const std::wstring& filePath)
{

    if (LoadShader(filePath, vsBlob) == false)
    {

        return;

    }

    desc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
    desc.VS.BytecodeLength = vsBlob->GetBufferSize();

}