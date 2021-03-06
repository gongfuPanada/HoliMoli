// A constant buffer that stores per-mesh data.
//cbuffer ModelConstantBuffer : register(b0)
//{
//    float4x4      modelToWorld;
//    float4x4      normalsToWorld;
//};

// A constant buffer that stores each set of view and projection matrices in column-major format.
//cbuffer ViewProjectionConstantBuffer : register(b1)
//{
//    float4   cameraPosition;
//    float4   lightPosition;
//    float4x4 viewProjection[2];
//};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
    min16float3 pos     : POSITION;
    min16float3 color   : COLOR0;
    min16float  vdw     : PSIZE0;
    uint        instId  : SV_InstanceID;
};

// Per-vertex data passed to the geometry shader.
// Note that the render target array index will be set by the geometry shader
// using the value of viewId.
struct VertexShaderOutput
{
    min16float4 pos     : POSITION; // SV_POSITION
    min16float3 color   : COLOR0;
    min16float  vdw     : PSIZE0;
    uint        viewId  : TEXCOORD0;  // SV_InstanceID % 2
};

// Simple shader to do vertex processing on the GPU.
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    float4 pos = float4(input.pos, 1.0f);

    // Note which view this vertex has been sent to. Used for matrix lookup.
    // Taking the modulo of the instance ID allows geometry instancing to be used
    // along with stereo instanced drawing; in that case, two copies of each 
    // instance would be drawn, one for left and one for right.
    int idx = input.instId % 2;

    // Transform the vertex position into world space.
    //pos = mul(pos, modelToWorld);

    // Correct for perspective and project the vertex position onto the screen.
    //pos = mul(pos, viewProjection[idx]);
    output.pos = (min16float4)pos;

    // Pass the color through without modification.
    output.color = input.color;

    // Pass the vdW radius through without modification.
    output.vdw = input.vdw;

    // Set the instance ID. The pass-through geometry shader will set the
    // render target array index to whatever value is set here.
    output.viewId = idx;

    return output;
}
