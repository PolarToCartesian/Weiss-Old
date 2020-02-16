cbuffer cCamera
{
	matrix cameraMat;
}

cbuffer cObject
{
    matrix objectMat;
};

struct VSoutput
{
    float4 color : COLOR;
    float4 positionSV : SV_Position;
};

VSoutput main(float3 position : POSITION, float4 color : COLOR)
{    
    VSoutput output;
    output.color = color;
    output.positionSV = mul(mul(float4(position.x, position.y, position.z, 1.0f), objectMat), cameraMat);
    
    return output;
}