
#include "ModelClass.h"


ModelClass::ModelClass()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mVertexCount = 0;
	mIndexCount = 0;
}

ModelClass::ModelClass(const ModelClass& Other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* Device)
{
	bool Result;

	// ���ؽ� ���ۿ� �ε��� ���۸� �ʱ�ȭ.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// ���ؽ� ���ۿ� �ε��� ���۸� ����.
	ShutdownBuffers();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* DeviceContext)
{
	// �������ۿ� �ε������۸� �׷��Ƚ� ���������ο� ����.
	RenderBuffers(DeviceContext);

	return;
}

bool ModelClass::InitializeBuffers(ID3D11Device* Device)
{
	VertexType* Vertices;
	unsigned __int64* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;
	HRESULT Result;

	// ���ؽ� �迭�� ���̸� ������.
	mVertexCount = 3;

	// �ε��� �迭�� ���̸� ������.
	mIndexCount = 3;

	// ���ؽ� �迭�� ���� (�ӽù迭)
	VertexType = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// �ε��� �迭�� ���� (�ӽù迭)
	Indices = new unsigned __int64[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// ���ؽ�-�ε����迭�� �ð���� ������� �־����.
	// �׷��� ������ DirectX�� Backface�� �ִٰ� �Ǵ��Ͽ� Culling�Ǿ� ������ ����.

	// ���ؽ� �迭�� ���� ����.
	Vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // ���� �Ʒ�
	Vertices[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f); // ��� ���
	Vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f); // ������ �Ʒ�
	Vertices[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// �ε��� �迭�� ���� ����.
	Indices[0] = 0; // ���� �Ʒ�
	Indices[1] = 1; // ��� ���
	Indices[2] = 2; // ������ �Ʒ�

	// ���ؽ�-�ε����迭�� �̿��Ͽ� ���ؽ�-�ε������۸� ������.

	// ���ؽ� ������ Description�� �ۼ�.
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// ���ؽ� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// ���ؽ� ���۸� ������.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &mVertexBuffer);
	if (Result == false)
	{
		return false;
	}

	// �ε��� ������ Description�� �ۼ�.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned __int64) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ������.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (Result == false)
	{
		return false;
	}

	// CreateBuffer�� ���� ���� �Ҵ�� ���ؽ�-�ε��� �迭�� ����.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	// ����
	return true;
}

void ModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� ����.
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	// ���ؽ� ���۸� ����.
	if (mVertexBuffer != nullptr)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

// ���ؽ�-�ε��� ���۸� GPU�� ������� �־� �������� ��û��.
void ModelClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned __int32 Stride;
	unsigned __int32 Offset;

	// ���ؽ� ������ �������� ���� (1�� ���ؽ�).
	Stride = sizeof(VertexType);
	Offset = 0;

	// Input Assembler(GPU)�� ���ؽ� ���۸� Ȱ��ȭ�Ͽ� �������� �����ϰ� ��.
	DeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &Stride, &Offset);

	// Input Assembler�� �ε��� ���۸� Ȱ��ȭ�Ͽ� �������� �����ϰ� ��.
	DeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���ؽ� ���۷� �׸� �⺻���� ����. ���⼭�� �ﰢ����.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

