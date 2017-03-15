cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_INPUT
{
    float4 Pos  : POSITION; 
    float2 Tex  : TEXCOORD0;
    float3 Nor  : NORMAL;
};

struct GS_INPUT
{
    float4 Pos  : POSITION;
    float4 PosV : TEXCOORD0;
};

GS_INPUT main(VS_INPUT input)
{
    GS_INPUT output;
    input.Pos.w = 1.0f;
    output.Pos = mul(input.Pos, worldMatrix);
    output.Pos = mul(output.Pos, viewMatrix);
    output.Pos = mul(output.Pos, projectionMatrix);
    output.PosV = mul(input.Pos, worldMatrix);
    output.PosV = mul(output.PosV, viewMatrix);
    return output;
}