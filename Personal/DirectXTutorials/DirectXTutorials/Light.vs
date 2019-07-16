
cbuffer MatrixBuffer
{
    matrix WorldMatrix;
    matrix ViewMatrix;
    matrix ProjectionMatrix;
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
};


PixelInputType LightVertexShader(VertexInputType Input)
{
    PixelInputType Output;

    // �ùٸ� ��Ŀ����� ���� 4���� ��Ҹ� ������ ����� ����
    Input.Position.w = 1.0f;

    // World, View, Projection ��İ� �����Ͽ� ���ؽ��� ��ġ�� ����.
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);

    // �ȼ� ���̴��� �ؽ��� ��ǥ�� ����.
    Output.Tex = Input.Tex;

    // ���� ��Ŀ� ���� �븻 ���͸� �����.
    Output.Normal = mul(Input.Normal, (float3x3)WorldMatrix);

    // �븻 ���͸� ����ȭ.
    Output.Normal = normalize(Output.Normal);

    return Output;
}
