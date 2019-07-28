
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

	// 스크린 사이즈를 저장함.
	mScreenWidth = ScreenWidth;
	mScreenHeight = ScreenHeight;

	// 렌더링할 픽셀 사이즈를 저장함.
	mBitmapWidth = BitmapWidth;
	mBitmapHeight = BitmapHeight;

	// 이전 렌더링 위치를 -1로 초기화.
	mPrevPosX = -1;
	mPrevPosY = -1;

	// 버텍스-인덱스 버퍼를 초기화.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	// 모델에서 텍스쳐를 불러옴.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void BitmapClass::Shutdown()
{
	// 모델 텍스쳐를 해제.
	ReleaseTexture();

	// 버텍스-인덱스 버퍼를 해제.
	ShutdonwBuffers();

	return;
}

bool BitmapClass::Render(ID3D11DeviceContext* DeviceContext, int PositionX, int PositionY)
{
	bool Result;

	// 동적 버텍스 버퍼를 리빌드하여 화면의 다른위치에 렌더링 할 수 있도록함.
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

	// 버텍스 배열의 개수를 설정.
	mVertexCount = 6;

	// 인덱스 배열의 개수를 설정.
	mIndexCount = mVertexCount;

	// 버텍스 배열 생성.
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 인덱스 배열 생성.
	Indices = new unsigned long[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// 버텍스 배열을 0으로 초기화.
	memset(Vertices, 0, (sizeof(VertexType) * mVertexCount));

	// 인덱스 배열에 데이터를 넣음.
	for (int i = 0; i < mIndexCount; ++i)
	{
		Indices[i] = i;
	}

	// 동적 버텍스 버퍼의 Description을 작성.
	VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// 서브리소스 구조체에 버텍스 데이터에 대한 포인터를 지정함.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// 버텍스 버퍼를 생성.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &mVertexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 Description을 작성.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 서브리소스 구조체에 인덱스 데이터에 대한 포인터를 지정함.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// 버퍼를 생성하고 남은 버텍스-인덱스 배열을 해제함.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	return true;
}

void BitmapClass::ShutdonwBuffers()
{
	// 인덱스 버퍼를 해제함.
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	// 버텍스 버퍼를 해제함.
	if (mVertexBuffer != nullptr)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

// 매 프레임 마다 동적 버텍스 버퍼의 내용을 새 위치로 변경함.
bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* DeviceContext, int PositionX, int PositionY)
{
	float Left, Right, Top, Bottom;
	VertexType* Vertices;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	VertexType* VerticesPtr;
	HRESULT Result;

	// 이미지의 위치가 이전과 비교하여 달라졌는지 확인.
	// 같을 경우 여기서 함수를 정상적으로 종료함.
	if ((PositionX == mPrevPosX) && (PositionY == mPrevPosY))
	{
		return true;
	}

	// 이미지 위치가 변경되었다면 새로운 위치를 저장함.
	mPrevPosX = PositionX;
	mPrevPosY = PositionY;

	// 비트맵의 왼쪽 끝의 스크린 좌표를 계산.
	Left = static_cast<float>(((mScreenWidth / 2) * -1) + static_cast<float>(PositionX));

	// 비트맵의 오른쪽 끝의 스크린 좌표를 계산
	Right = Left + mBitmapWidth;

	// 비트맵의 위쪽 끝의 스크린 좌표를 계산.
	Top = static_cast<float>(((mScreenHeight / 2)) + static_cast<float>(PositionY));

	// 비트맵의 아래쪽 끝의 스크린 좌표를 계산.
	Bottom = Top - mBitmapHeight;

	// 버텍스 배열을 생성.
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 버텍스 배열을 데이터와 같이 불러옴.
	// 첫번째 트라이앵글
	DirectX::XMStoreFloat3(&Vertices[0].Position, DirectX::XMVectorSet(Left, Top, 0.0f, 0.0f)); // Left Top
	DirectX::XMStoreFloat2(&Vertices[0].Texture, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[1].Position, DirectX::XMVectorSet(Right, Bottom, 0.0f, 0.0f)); // Bottom Right
	DirectX::XMStoreFloat2(&Vertices[1].Texture, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[2].Position, DirectX::XMVectorSet(Left, Bottom, 0.0f, 0.0f)); // Bottom Left
	DirectX::XMStoreFloat2(&Vertices[2].Texture, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// 두번째 트라이앵글
	DirectX::XMStoreFloat3(&Vertices[3].Position, DirectX::XMVectorSet(Left, Top, 0.0f, 0.0f)); // Top Left
	DirectX::XMStoreFloat2(&Vertices[3].Texture, DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[4].Position, DirectX::XMVectorSet(Right, Top, 0.0f, 0.0f)); // Top Right
	DirectX::XMStoreFloat2(&Vertices[4].Texture, DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat3(&Vertices[5].Position, DirectX::XMVectorSet(Right, Bottom, 0.0f, 0.0f)); // Top Left
	DirectX::XMStoreFloat2(&Vertices[5].Texture, DirectX::XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f));

	// 버텍스 버퍼에 작성하기 위해 잠금.
	Result = DeviceContext->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// 버텍스 버퍼내의 데이터의 포인터를 얻음.
	VerticesPtr = reinterpret_cast<VertexType*>(MappedResource.pData);

	// 버텍스 버퍼의 데이터를 복사함.
	memcpy(VerticesPtr, reinterpret_cast<void*>(Vertices), (sizeof(VertexType) * mVertexCount));

	// 버텍스 버퍼의 잠금을 해제.
	DeviceContext->Unmap(mVertexBuffer, 0);

	// 버텍스 배열을 해제함.
	delete[] Vertices;
	Vertices = nullptr;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned int Stride;
	unsigned int Offset;

	// 버텍스 버퍼의 스트라이드와 오프셋을 설정.
	Stride = sizeof(VertexType);
	Offset = 0;

	// 렌더링을 할 수 있도록 입력 어셈블러에서 버텍스 버퍼를 활성으로 설정.
	DeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &Stride, &Offset);

	// 렌더링을 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정.
	DeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 버텍스 버퍼로부터 Draw해야하는 프리미티브의 형태를 설정.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool BitmapClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	bool Result;

	// 텍스쳐 오브젝트를 생성.
	mTexture = new TextureClass;
	if (mTexture == nullptr)
	{
		return false;
	}

	// 텍스쳐 오브젝트를 초기화.
	Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void BitmapClass::ReleaseTexture()
{
	// 텍스쳐 객체를 해제.
	if (mTexture != nullptr)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = nullptr;
	}

	return;
}

