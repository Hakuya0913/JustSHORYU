Texture2D DiffuseTexture : register(t0);

SamplerState linearSampler : register(s0);

struct PSInput
{
    
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    
};

float4 main(PSInput input) : SV_TARGET
{
    
    return DiffuseTexture.Sample(linearSampler, input.texCoord);
    
}