
#include "BitmapClass.h"


BitmapClass::BitmapClass()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mTexture = nullptr;
}

BitmapClass::BitmapClass(const BitmapClass& Other)
{
}

BitmapClass::~BitmapClass()
{
}

bool BitmapClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, int ScreenWidth, int ScreenHeight, const char* TextureFileName, int BitmapWidth, int BitmapHeight)
{
	bool Result;

	// ��ũ�� ����� ������.
	mScreenWidth = ScreenWidth;
	mScreenHeight = ScreenHeight;

	// �������� �ȼ� ����� ������.
	mBitmapWidth = BitmapWidth;
	mBitmapHeight = BitmapHeight;

	// ���� ������ ��ġ�� -1�� �ʱ�ȭ.
	mPrevPosX = -1;
	mPrevPosY = -1;

	// ���ؽ�-�ε��� ���۸� �ʱ�ȭ.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	// �𵨿��� �ؽ��ĸ� �ҷ���.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	// �� �ؽ��ĸ� ����.
	ReleaseTexture();

	// ���ؽ�-�ε��� ���۸� ����.
	ShutdonwBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* DeviceContext, int PositionX, int PositionY)
{
	bool Result;

	// ���� ���ؽ� ���۸� �������Ͽ� ȭ���� �ٸ���ġ�� ������ �� �� �ֵ�����.
	Result = UpdateBuffers(DeviceContext, PositionX, PositionY);
	if (Result == false)
	{
		return false;
	}

	RenderBuffers(DeviceContext);

	return true;
}

bool BitmapClass::InitializeBuffers(ID3D11Device* Device)
{
	VertexType* Vertices;
	unsigned long* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;
	HRESULT Result;

	// ���ؽ� �迭�� ������ ����.
	mVertexCount = 6;

	// �ε��� �迭�� ������ ����.
	mIndexCount = mVertexCount;

	// ���ؽ� �迭 ����.
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// �ε��� �迭 ����.
	Indices = new unsigned long[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// ���ؽ� �迭�� 0���� �ʱ�ȭ.
	memset(Vertices, 0, (sizeof(VertexType) * mVertexCount));

	// �ε��� �迭�� �����͸� ����.
	for (int i = 0; i < mIndexCount; ++i)
	{
		Indices[i] = i;
	}

	// ���� ���ؽ� ������ Description�� �ۼ�.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// ���긮�ҽ� ����ü�� ���ؽ� �����Ϳ� ���� �����͸� ������.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// ���ؽ� ���۸� ����.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &mVertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// ���� �ε��� ������ Description�� �ۼ�.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// ���긮�ҽ� ����ü�� �ε��� �����Ϳ� ���� �����͸� ������.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// ���۸� �����ϰ� ���� ���ؽ�-�ε��� �迭�� ������.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	return true;
}

void BitmapClass::ShutdonwBuffers()
{
	// �ε��� ���۸� ������.
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	// ���ؽ� ���۸� ������.
	if (mVertexBuffer != nullptr)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

// �� ������ ���� ���� ���ؽ� ������ ������ �� ��ġ�� ������.
bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* DeviceContext, int PositionX, int PositionY)
{
	float Left, Right, Top, Bottom;
	VertexType* Vertices;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VerticesPtr;
	HRESULT Result;

	// �̹����� ��ġ�� ������ ���Ͽ� �޶������� Ȯ��.
	// ���� ��� ���⼭ �Լ��� ���������� ������.
	if ((PositionX == mPrevPosX) && (PositionY == mPrevPosY))
	{
		return true;
	}

	// �̹��� ��ġ�� ����Ǿ��ٸ� ���ο� ��ġ�� ������.
	mPrevPosX = PositionX;
	mPrevPosY = PositionY;

	// ��Ʈ���� ���� ���� ��ũ�� ��ǥ�� ���.
	Left = static_cast<float>(((mScreenWidth / 2) * -1) + static_cast<float>(PositionX));

	// ��Ʈ���� ������ ���� ��ũ�� ��ǥ�� ���
	Right = Left + mBitmapWidth;

	// ��Ʈ���� ���� ���� ��ũ�� ��ǥ�� ���.
	Top = static_cast<float>(((mScreenHeight / 2)) + static_cast<float>(PositionY));

	// ��Ʈ���� �Ʒ��� ���� ��ũ�� ��ǥ�� ���.
	Bottom = Top - mBitmapHeight;

	// ���ؽ� �迭�� ����.
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// ���ؽ� �迭�� �����Ϳ� ���� �ҷ���.
	// ù��° Ʈ���̾ޱ�
	DirectX::XMStoreFloat3(&Vertices[0].Position, DirectX::XMVectorSet(Left, Top, 0.0f, 0.0f)); // Left Top
	DirectX::XMStoreFloat2(&Vertices[0].Texture, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[1].Position, DirectX::XMVectorSet(Right, Bottom, 0.0f, 0.0f)); // Bottom Right
	DirectX::XMStoreFloat2(&Vertices[1].Texture, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[2].Position, DirectX::XMVectorSet(Left, Bottom, 0.0f, 0.0f)); // Bottom Left
	DirectX::XMStoreFloat2(&Vertices[2].Texture, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// �ι�° Ʈ���̾ޱ�
	DirectX::XMStoreFloat3(&Vertices[3].Position, DirectX::XMVectorSet(Left, Top, 0.0f, 0.0f)); // Top Left
	DirectX::XMStoreFloat2(&Vertices[3].Texture, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[4].Position, DirectX::XMVectorSet(Right, Top, 0.0f, 0.0f)); // Top Right
	DirectX::XMStoreFloat2(&Vertices[4].Texture, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[5].Position, DirectX::XMVectorSet(Right, Bottom, 0.0f, 0.0f)); // Top Left
	DirectX::XMStoreFloat2(&Vertices[5].Texture, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));

	// ���ؽ� ���ۿ� �ۼ��ϱ� ���� ���.
	Result = DeviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// ���ؽ� ���۳��� �������� �����͸� ����.
	VerticesPtr = reinterpret_cast<VertexType*>(MappedResource.pData);

	// ���ؽ� ������ �����͸� ������.
	memcpy(VerticesPtr, reinterpret_cast<void*>(Vertices), (sizeof(VertexType) * mVertexCount));

	// ���ؽ� ������ ����� ����.
	DeviceContext->Unmap(mVertexBuffer, 0);

	// ���ؽ� �迭�� ������.
	delete[] Vertices;
	Vertices = nullptr;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned int Stride;
	unsigned int Offset;

	// ���ؽ� ������ ��Ʈ���̵�� �������� ����.
	Stride = sizeof(VertexType);
	Offset = 0;

	// �������� �� �� �ֵ��� �Է� ��������� ���ؽ� ���۸� Ȱ������ ����.
	DeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &Stride, &Offset);

	// �������� �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ ����.
	DeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �� ���ؽ� ���۷κ��� Draw�ؾ��ϴ� ������Ƽ���� ���¸� ����.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	bool Result;

	// �ؽ��� ������Ʈ�� ����.
	mTexture = new TextureClass;
	if (mTexture == nullptr)
	{
		return false;
	}

	// �ؽ��� ������Ʈ�� �ʱ�ȭ.
	Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	// �ؽ��� ��ü�� ����.
	if (mTexture != nullptr)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = nullptr;
	}

	return;
}

