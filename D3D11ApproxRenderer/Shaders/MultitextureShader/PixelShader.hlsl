struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


Texture2D shaderTextures[2];
SamplerState SampleType;
float gammaCorrection = 2.0;

float4 main(PixelInputType input) : SV_TARGET
{
    float4 texture1Color;
    float4 texture2Color;
    float4 outputTextureColor;

    texture1Color = shaderTextures[0].Sample(SampleType, input.tex);
    texture2Color = shaderTextures[1].Sample(SampleType, input.tex);
    outputTextureColor = texture1Color * texture2Color * gammaCorrection;
    outputTextureColor = saturate(outputTextureColor);
    return outputTextureColor;
}