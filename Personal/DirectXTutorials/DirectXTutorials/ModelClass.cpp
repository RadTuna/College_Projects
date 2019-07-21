
#include "ModelClass.h"
#include "FBXParser.h"


ModelClass::ModelClass()
{
	mVertexBuffer = nullptr;
	mIndexBuffer = nullptr;
	mTexture = nullptr;
	mModel = nullptr;
}

ModelClass::ModelClass(const ModelClass& Other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* TextureFileName, const char* ModelFilename, bool IsFBX)
{
	bool Result;

	if (IsFBX == true)
	{
		// 모델 데이터를 불러옴.
		Result = LoadFBXModel(ModelFilename);
		if (Result == false)
		{
			return false;
		}
	}
	else
	{
		// 모델 데이터를 불러옴.
		Result = LoadModelTxt(ModelFilename);
		if (Result == false)
		{
			return false;
		}
	}

	// 버텍스 버퍼와 인덱스 버퍼를 초기화. // 모델 데이터를 불러오고 초기화를 해야함.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	// 텍스쳐 모델을 불러옴.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
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

	// 텍스쳐 모델을 해제.
	ReleaseTexture();

	// 모델 데이터를 해제.
	ReleaseModel();

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
	unsigned long* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;
	HRESULT Result;

	// Model 데이터를 직접 하드코딩하는 것이 아닌 파일을 불러오는 방식으로 변경됨.

	// 버텍스 배열을 생성 (임시배열)
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// 인덱스 배열을 생성 (임시배열)
	Indices = new unsigned long[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// 버텍스-인덱스배열에 시계방향 순서대로 넣어야함.
	// 그렇지 않으면 DirectX는 Backface에 있다고 판단하여 Culling되어 보이지 않음.
	// 버텍스-인덱스배열에 불러온 모델데이터를 넣음.
	for (int i = 0; i < mVertexCount; ++i)
	{
		DirectX::XMStoreFloat3(&Vertices[i].Position, DirectX::XMVectorSet(mModel[i].X, mModel[i].Y, mModel[i].Z, 0.0f));
		DirectX::XMStoreFloat2(&Vertices[i].Texture, DirectX::XMVectorSet(mModel[i].Tu, mModel[i].Tv, 0.0f, 0.0f));
		DirectX::XMStoreFloat3(&Vertices[i].Normal, DirectX::XMVectorSet(mModel[i].Nx, mModel[i].Ny, mModel[i].Nz, 0.0f));

		Indices[i] = i;
	}

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
	if (FAILED(Result))
	{
		return false;
	}

	// 인덱스 버퍼의 Description을 작성.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성함.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// CreateBuffer를 통해 값이 할당된 버텍스-인덱스 배열을 해제.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

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
	unsigned int Stride;
	unsigned int Offset;

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

int ModelClass::GetIndexCount() const
{
	return mIndexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture() const
{
	return mTexture->GetTexture();
}

bool ModelClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	// 텍스쳐 객체를 생성.
	mTexture = new TextureClass;
	if (mTexture == nullptr)
	{
		return false;
	}

	// 텍스쳐 객체를 초기화.
	bool Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// 텍스쳐 객체를 해제.
	if (mTexture != nullptr)
	{
		mTexture->Shutdown();
		delete mTexture;
		mTexture = nullptr;
	}
}

bool ModelClass::LoadModelTxt(const char* Filename)
{
	std::ifstream Fin;
	char Input;

	// 모델 파일을 열음.
	Fin.open(Filename);

	// 파일을 열지 못하면 함수를 종료함.
	if (Fin.fail())
	{
		return false;
	}

	// 버텍스의 개수를 읽기 위한 위치로 이동함.
	Fin.get(Input);
	while (Input != ':')
	{
		Fin.get(Input);
	}

	// 버텍스의 개수를 읽음.
	Fin >> mVertexCount;

	// 인덱스의 개수를 버텍스 개수와 동일한 값으로 맞춤.
	mIndexCount = mVertexCount;

	// 버텍스 정보를 읽기 위한 모델 객체를 생성함.
	mModel = new ModelType[mVertexCount];
	if (mModel == nullptr)
	{
		return false;
	}

	// 데이터를 읽기위한 시작지점으로 이동함.
	Fin.get(Input);
	while (Input != ':')
	{
		Fin.get(Input);
	}
	Fin.get(Input);
	Fin.get(Input);

	// 버텍스 정보를 읽음.
	for (int i = 0; i < mVertexCount; ++i)
	{
		Fin >> mModel[i].X >> mModel[i].Y >> mModel[i].Z;
		Fin >> mModel[i].Tu >> mModel[i].Tv;
		Fin >> mModel[i].Nx >> mModel[i].Ny >> mModel[i].Nz;
	}

	// 모델 파일을 닫음.
	Fin.close();

	return true;
}

bool ModelClass::LoadFBXModel(const char* FBXFilename)
{
	bool Result;

	// FBX파서 객체를 생성.
	FBXParser* DXFBXParser = new FBXParser;
	if (DXFBXParser == nullptr)
	{
		return false;
	}

	Result = DXFBXParser->Initialize(FBXFilename);
	if (Result == false)
	{
		return false;
	}

	Result = DXFBXParser->ImportScene();
	if (Result == false)
	{
		return false;
	}

	// 버텍스-인덱스 카운트를 얻음.
	mVertexCount = DXFBXParser->GetVertexCount();
	mIndexCount = DXFBXParser->GetIndexCount();

	// 버텍스 정보를 읽기위한 모델 객체를 생성.
	mModel = new ModelType[mVertexCount];
	if (mModel == nullptr)
	{
		return false;
	}

	for (int i = 0; i < mVertexCount; ++i)
	{
		// 위치를 얻음.
		mModel[i].X = DXFBXParser->GetFBXVertices()[i].Position.x;
		mModel[i].Y = DXFBXParser->GetFBXVertices()[i].Position.y;
		mModel[i].Z = DXFBXParser->GetFBXVertices()[i].Position.z;

		// UV를 얻음.
		mModel[i].Tu = DXFBXParser->GetFBXVertices()[i].UV.x;
		mModel[i].Tv = DXFBXParser->GetFBXVertices()[i].UV.y;

		// 노말벡터를 얻음.
		mModel[i].Nx = DXFBXParser->GetFBXVertices()[i].Normal.x;
		mModel[i].Ny = DXFBXParser->GetFBXVertices()[i].Normal.y;
		mModel[i].Nz = DXFBXParser->GetFBXVertices()[i].Normal.z;
	}

	// FBX파서 객체를 해제.
	delete DXFBXParser;
	DXFBXParser = nullptr;

	return true;
}

void ModelClass::ReleaseModel()
{
	if (mModel != nullptr)
	{
		delete mModel;
		mModel = nullptr;
	}
}

