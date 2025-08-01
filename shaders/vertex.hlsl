 
cbuffer ConstantBuffer : register(b0) {
    matrix World;
    matrix View; 
    matrix Projection;
};

struct VertexInput {
    float3 position : POSITION;
    float3 color : COLOR;
};

struct VertexOutput {
    float4 position : SV_POSITION;
    float3 color : COLOR;
};

VertexOutput main(VertexInput input) {
    VertexOutput output;
    
    float4 worldPos = mul(float4(input.position, 1.0f), World);
    float4 viewPos = mul(worldPos, View);
    output.position = mul(viewPos, Projection);
    
    output.color = input.color;
    
    return output;
}