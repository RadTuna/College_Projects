
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
		// �� �����͸� �ҷ���.
		Result = LoadFBXModel(ModelFilename);
		if (Result == false)
		{
			return false;
		}
	}
	else
	{
		// �� �����͸� �ҷ���.
		Result = LoadModelTxt(ModelFilename);
		if (Result == false)
		{
			return false;
		}
	}

	// ���ؽ� ���ۿ� �ε��� ���۸� �ʱ�ȭ. // �� �����͸� �ҷ����� �ʱ�ȭ�� �ؾ���.
	Result = InitializeBuffers(Device);
	if (Result == false)
	{
		return false;
	}

	// �ؽ��� ���� �ҷ���.
	Result = LoadTexture(Device, DeviceContext, TextureFileName);
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

	// �ؽ��� ���� ����.
	ReleaseTexture();

	// �� �����͸� ����.
	ReleaseModel();

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
	unsigned long* Indices;
	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA VertexData;
	D3D11_SUBRESOURCE_DATA IndexData;
	HRESULT Result;

	// Model �����͸� ���� �ϵ��ڵ��ϴ� ���� �ƴ� ������ �ҷ����� ������� �����.

	// ���ؽ� �迭�� ���� (�ӽù迭)
	Vertices = new VertexType[mVertexCount];
	if (Vertices == nullptr)
	{
		return false;
	}

	// �ε��� �迭�� ���� (�ӽù迭)
	Indices = new unsigned long[mIndexCount];
	if (Indices == nullptr)
	{
		return false;
	}

	// ���ؽ�-�ε����迭�� �ð���� ������� �־����.
	// �׷��� ������ DirectX�� Backface�� �ִٰ� �Ǵ��Ͽ� Culling�Ǿ� ������ ����.
	// ���ؽ�-�ε����迭�� �ҷ��� �𵨵����͸� ����.
	for (int i = 0; i < mVertexCount; ++i)
	{
		DirectX::XMStoreFloat3(&Vertices[i].Position, DirectX::XMVectorSet(mModel[i].X, mModel[i].Y, mModel[i].Z, 0.0f));
		DirectX::XMStoreFloat2(&Vertices[i].Texture, DirectX::XMVectorSet(mModel[i].Tu, mModel[i].Tv, 0.0f, 0.0f));
		DirectX::XMStoreFloat3(&Vertices[i].Normal, DirectX::XMVectorSet(mModel[i].Nx, mModel[i].Ny, mModel[i].Nz, 0.0f));

		Indices[i] = i;
	}

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
	if (FAILED(Result))
	{
		return false;
	}

	// �ε��� ������ Description�� �ۼ�.
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�.
	IndexData.pSysMem = Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ������.
	Result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &mIndexBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// CreateBuffer�� ���� ���� �Ҵ�� ���ؽ�-�ε��� �迭�� ����.
	delete[] Vertices;
	Vertices = nullptr;

	delete[] Indices;
	Indices = nullptr;

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
	unsigned int Stride;
	unsigned int Offset;

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
	// �ؽ��� ��ü�� ����.
	mTexture = new TextureClass;
	if (mTexture == nullptr)
	{
		return false;
	}

	// �ؽ��� ��ü�� �ʱ�ȭ.
	bool Result = mTexture->Initialize(Device, DeviceContext, FileName);
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// �ؽ��� ��ü�� ����.
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

	// �� ������ ����.
	Fin.open(Filename);

	// ������ ���� ���ϸ� �Լ��� ������.
	if (Fin.fail())
	{
		return false;
	}

	// ���ؽ��� ������ �б� ���� ��ġ�� �̵���.
	Fin.get(Input);
	while (Input != ':')
	{
		Fin.get(Input);
	}

	// ���ؽ��� ������ ����.
	Fin >> mVertexCount;

	// �ε����� ������ ���ؽ� ������ ������ ������ ����.
	mIndexCount = mVertexCount;

	// ���ؽ� ������ �б� ���� �� ��ü�� ������.
	mModel = new ModelType[mVertexCount];
	if (mModel == nullptr)
	{
		return false;
	}

	// �����͸� �б����� ������������ �̵���.
	Fin.get(Input);
	while (Input != ':')
	{
		Fin.get(Input);
	}
	Fin.get(Input);
	Fin.get(Input);

	// ���ؽ� ������ ����.
	for (int i = 0; i < mVertexCount; ++i)
	{
		Fin >> mModel[i].X >> mModel[i].Y >> mModel[i].Z;
		Fin >> mModel[i].Tu >> mModel[i].Tv;
		Fin >> mModel[i].Nx >> mModel[i].Ny >> mModel[i].Nz;
	}

	// �� ������ ����.
	Fin.close();

	return true;
}

bool ModelClass::LoadFBXModel(const char* FBXFilename)
{
	bool Result;

	// FBX�ļ� ��ü�� ����.
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

	// ���ؽ�-�ε��� ī��Ʈ�� ����.
	mVertexCount = DXFBXParser->GetVertexCount();
	mIndexCount = DXFBXParser->GetIndexCount();

	// ���ؽ� ������ �б����� �� ��ü�� ����.
	mModel = new ModelType[mVertexCount];
	if (mModel == nullptr)
	{
		return false;
	}

	for (int i = 0; i < mVertexCount; ++i)
	{
		// ��ġ�� ����.
		mModel[i].X = DXFBXParser->GetFBXVertices()[i].Position.x;
		mModel[i].Y = DXFBXParser->GetFBXVertices()[i].Position.y;
		mModel[i].Z = DXFBXParser->GetFBXVertices()[i].Position.z;

		// UV�� ����.
		mModel[i].Tu = DXFBXParser->GetFBXVertices()[i].UV.x;
		mModel[i].Tv = DXFBXParser->GetFBXVertices()[i].UV.y;

		// �븻���͸� ����.
		mModel[i].Nx = DXFBXParser->GetFBXVertices()[i].Normal.x;
		mModel[i].Ny = DXFBXParser->GetFBXVertices()[i].Normal.y;
		mModel[i].Nz = DXFBXParser->GetFBXVertices()[i].Normal.z;
	}

	// FBX�ļ� ��ü�� ����.
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

