
cbuffer MatrixBuffer
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
};

// 스펙큘러 라이트의 계산을 위해 카메라의 위치가 필요.
cbuffer CameraBuffer
{
    float3 CameraPosition;
    float Padding;
};

struct VertexInputType
{
    float4 Position : POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 VeiwDirection : TEXCOORD1;
};


PixelInputType LightVertexShader(VertexInputType Input)
{
    PixelInputType Output;
    float4 WorldPosition;

    // 올바른 행렬연산을 위해 4개의 요소를 가지는 행렬을 만듦
    Input.Position.w = 1.0f;

    // World, View, Projection 행렬과 연산하여 버텍스의 위치를 맞춤.
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);

    // 픽셀 셰이더의 텍스쳐 좌표를 넣음.
    Output.Tex = Input.Tex;

    // 월드 행렬에 대한 노말 벡터를 계산함.
    Output.Normal = mul(Input.Normal, (float3x3)WorldMatrix);

    // 노말 벡터를 정규화.
    Output.Normal = normalize(Output.Normal);

    // 월드 행렬과 연산해 월드 좌표계에서의 버텍스 위치를 찾음.
    WorldPosition = mul(Input.Position, WorldMatrix);

    // 카메라의 위치와 버텍스 위치를 기준으로 뷰 방향을 결정.
    Output.VeiwDirection = CameraPosition.xyz - WorldPosition.xyz;

    // 뷰 벡터를 정규화.
    Output.VeiwDirection = normalize(Output.VeiwDirection);

    return Output;
}
