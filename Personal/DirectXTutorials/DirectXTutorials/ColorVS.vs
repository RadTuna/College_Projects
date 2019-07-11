
// 전역변수
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// 타입 정의
struct VertexInputType
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

//버텍스 셰이더
PixelInputType ColorVertexShader(VertexInputType Input)
{
    PixelInputType Output;

	// 올바른 행렬 연산을 위해 float4 벡터를 사용함.
    Input.Position.w = 1.0f;

	// 버텍스의 위치를 월드, 뷰, 투영의 순으로 계산함.
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);

	// 픽셀 셰이더에서 사용하기 위해 입력 색상을 저장함.
    Output.Color = Input.Color;

    return Output;
}