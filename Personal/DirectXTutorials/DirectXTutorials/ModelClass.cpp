
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

	// 버텍스 버퍼와 인덱스 버퍼를 초기화.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	// 버텍스 버퍼와 인덱스 버퍼를 해제.
	ShutdownBuffers();

	return;
}

void ModelClass::Render(ID3D11DeviceContext* DeviceContext)
{
	// 정점버퍼와 인덱스버퍼를 그래픽스 파이프라인에 넣음.
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

	// 버텍스 배열의 길이를 설정함.
	mVertexCount = 3;

	// 인덱스 배열의 길이를 설정함.
	mIndexCount = 3;

	// 버텍스 배열을 생성 (임시배열)
	VertexType = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 인덱스 배열을 생성 (임시배열)
	Indices = new unsigned __int64[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// 버텍스-인덱스배열에 시계방향 순서대로 넣어야함.
	// 그렇지 않으면 DirectX는 Backface에 있다고 판단하여 Culling되어 보이지 않음.

	// 버텍스 배열에 값을 넣음.
	Vertices[0].Position = XMFLOAT3(-1.0f, -1.0f, 0.0f); // 왼쪽 아래
	Vertices[0].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[1].Position = XMFLOAT3(0.0f, 1.0f, 0.0f); // 상단 가운데
	Vertices[1].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	Vertices[2].Position = XMFLOAT3(1.0f, -1.0f, 0.0f); // 오른쪽 아래
	Vertices[2].Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// 인덱스 배열에 값을 넣음.
	Indices[0] = 0; // 왼쪽 아래
	Indices[1] = 1; // 상단 가운데
	Indices[2] = 2; // 오른쪽 아래

	// 버텍스-인덱스배열을 이용하여 버텍스-인덱스버퍼를 생성함.

	// 버텍스 버퍼의 Description을 작성.
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// 버텍스 데이터를 가리키는 보조 리소스 구조체를 작성.
	VertexData.pSysMem = Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// 버텍스 버퍼를 생성함.
	Result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &mVertexBuffer);
	if (Result == false)
	{
		return false;
	}

	// 인덱스 버퍼의 Description을 작성.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned __int64) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성함.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (Result == false)
	{
		return false;
	}

	// CreateBuffer를 통해 값이 할당된 버텍스-인덱스 배열을 해제.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

	// 종료
	return true;
}

void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제.
	if (mIndexBuffer != nullptr)
	{
		mIndexBuffer->Release();
		mIndexBuffer = nullptr;
	}

	// 버텍스 버퍼를 해제.
	if (mVertexBuffer != nullptr)
	{
		mVertexBuffer->Release();
		mVertexBuffer = nullptr;
	}

	return;
}

// 버텍스-인덱스 버퍼를 GPU의 어셈블러로 넣어 렌더링을 요청함.
void ModelClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	unsigned __int32 Stride;
	unsigned __int32 Offset;

	// 버텍스 단위와 오프셋을 설정 (1개 버텍스).
	Stride = sizeof(VertexType);
	Offset = 0;

	// Input Assembler(GPU)에 버텍스 버퍼를 활성화하여 렌더링을 가능하게 함.
	DeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &Stride, &Offset);

	// Input Assembler에 인덱스 버퍼를 활성화하여 렌더링을 가능하게 함.
	DeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 버텍스 버퍼로 그릴 기본형을 설정. 여기서는 삼각형임.
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

