
// ��������
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

// Ÿ�� ����
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

//���ؽ� ���̴�
PixelInputType ColorVertexShader(VertexInputType Input)
{
    PixelInputType Output;

	// �ùٸ� ��� ������ ���� float4 ���͸� �����.
    Input.Position.w = 1.0f;

	// ���ؽ��� ��ġ�� ����, ��, ������ ������ �����.
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);

	// �ȼ� ���̴����� ����ϱ� ���� �Է� ������ ������.
    Output.Color = Input.Color;

    return Output;
}