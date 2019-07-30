
#include "FontShaderClass.h"


FontShaderClass::FontShaderClass()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mConstantBuffer = nullptr;
	mSampleState = nullptr;
	mPixelBuffer = nullptr;
}

FontShaderClass::FontShaderClass(const FontShaderClass& Other)
{
}

FontShaderClass::~FontShaderClass()
{
}

bool FontShaderClass::Initialize(ID3D11Device* Device, HWND hWnd)
{
	bool Result;

	// ���ؽ� ���̴��� �ȼ� ���̴��� �ʱ�ȭ.
	Result = InitializeShader(Device, hWnd, const_cast<WCHAR*>(L"../DirectXTutorials/Font.vs"), const_cast<WCHAR*>(L"../DirectXTutorials/Font.ps"));
	if (Result == false)
	{
		return false;
	}

	return true;
}

void FontShaderClass::Shutdown()
{
	// ���ؽ� ���̴��� �ȼ� ���̴� �� �׿� ���õ� �͵��� ����.
	ShutdownShader();

	return;
}

bool FontShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount,
	DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat, ID3D11ShaderResourceView* Texture,
	DirectX::FXMVECTOR PixelColor)
{
	bool Result;

	// �������� ����� ���̴��� ���ڸ� �Է�.
	Result = SetShaderParameters(DeviceContext, WorldMat, ViewMat, ProjectionMat, Texture, PixelColor);
	if (Result == false)
	{
		return false;
	}

	// ���̴��� �̿��Ͽ� �غ�� ���۸� �׸�.
	RenderShader(DeviceContext, IndexCount);

	return true;
}

// ���̴� ������ �ҷ��� DirectX�� GPU���� ��밡�� �ϵ��� �ϴ� �Լ� (�߿��� �Լ���)
bool FontShaderClass::InitializeShader(ID3D11Device* Device, HWND hWnd, WCHAR* VsFileName, WCHAR* PsFileName)
{
	HRESULT Result;
	ID3DBlob* ErrorMessage;
	ID3DBlob* VertexShaderBuffer;
	ID3DBlob* PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned int NumElements;
	D3D11_SAMPLER_DESC SamplerDesc;
	D3D11_BUFFER_DESC ConstantBufferDesc;
	D3D11_BUFFER_DESC PixelBufferDesc;

	// �� �Լ����� ����ϴ� �����͸� �ʱ�ȭ
	ErrorMessage = nullptr;
	VertexShaderBuffer = nullptr;
	PixelShaderBuffer = nullptr;

	// ���⼭ ���̴� ���α׷��� ���۷� ��������.

	// ���ؽ� ���̴��� ������.
	Result = D3DCompileFromFile(VsFileName, nullptr, nullptr, "FontVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		// ���̴� �����Ͽ� �����ϸ� ���� �޼����� �����.
		if (ErrorMessage != nullptr)
		{
			OutputShaderErrorMessage(ErrorMessage, hWnd, VsFileName);
		}
		// ���� �޼����� ���ٸ� ���̴� ������ ã�� ���Ѱ�.
		else
		{
			MessageBoxW(hWnd, VsFileName, L"Missing VertexShader File", MB_OK);
		}

		return false;
	}


	// �ȼ� ���̴��� ������.
	Result = D3DCompileFromFile(PsFileName, nullptr, nullptr, "FontPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
	if (FAILED(Result))
	{
		// ���̴� �������� �����ϸ� ���� �޼����� �����.
		if (ErrorMessage != nullptr)
		{
			OutputShaderErrorMessage(ErrorMessage, hWnd, PsFileName);
		}
		// ���� �޼����� ���ٸ� ���̴� ������ ã�� ���Ѱ�.
		else
		{
			MessageBoxW(hWnd, VsFileName, L"Missing PixelShader File", MB_OK);
		}

		return false;
	}

	// ���۸� �̿��Ͽ� Ŭ���������� �ִ� ���ؽ�-�ȼ� ���̴��� ������.

	// ���۷κ��� ���ؽ� ���̴��� ����.
	Result = Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader);
	if (FAILED(Result))
	{
		return false;
	}

	// ���۷κ��� �ȼ� ���̴��� ����.
	Result = Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader);
	if (FAILED(Result))
	{
		return false;
	}

	// ���ؽ� �Է� ���̾ƿ� Description�� �ۼ�.
	// �� ������ ModelClass�� ���̴��� �ִ� VertexType�� ��ġ�ؾ� ��.
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	// ���̾ƿ��� ��� ������ ������.
	NumElements = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// ���ؽ� �Է� ���̾ƿ��� ����.
	Result = Device->CreateInputLayout(PolygonLayout, NumElements, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &mLayout);
	if (FAILED(Result))
	{
		return false;
	}

	// ������ �ʴ� ���ؽ�-�ȼ� ���̴� ���۸� ����.
	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = nullptr;

	// ���ؽ� ���̴��� �ִ� ��� ��� ������ Description�� �ۼ�.
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDesc.ByteWidth = sizeof(ConstantBufferType);
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ֵ��� ��.
	Result = Device->CreateBuffer(&ConstantBufferDesc, nullptr, &mConstantBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	// �ؽ��� ���÷� ������ Description�� ����.
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1; // �̹漺 ���͸�
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �ؽ��� ���÷� ���¸� ����.
	Result = Device->CreateSamplerState(&SamplerDesc, &mSampleState);
	if (FAILED(Result))
	{
		return false;
	}

	// �ȼ� ���̴��� ������� ������ Description�� �ۼ�.
	// ByteWidth�� ������ 16�� ������� �ϹǷ� D3D11_BIND_CONSTANT_BUFFER�� ������� ������ CreateBuffer�Լ��� ����. 
	PixelBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	PixelBufferDesc.ByteWidth = sizeof(PixelBufferType);
	PixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	PixelBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	PixelBufferDesc.MiscFlags = 0;
	PixelBufferDesc.StructureByteStride = 0;

	// �� Ŭ�������� ����Ʈ ��� ���ۿ� ���� �� �� �ֵ��� ����Ʈ ��� ������ �����͸� ����.
	Result = Device->CreateBuffer(&PixelBufferDesc, nullptr, &mPixelBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void FontShaderClass::ShutdownShader()
{

	// ����Ʈ ��� ���۸� ����.
	if (mPixelBuffer != nullptr)
	{
		mPixelBuffer->Release();
		mPixelBuffer = nullptr;
	}

	// �ؽ��� ���÷� ���¸� ����.
	if (mSampleState != nullptr)
	{
		mSampleState->Release();
		mSampleState = nullptr;
	}

	// ��� ���۸� ����.
	if (mConstantBuffer != nullptr)
	{
		mConstantBuffer->Release();
		mConstantBuffer = nullptr;
	}

	// ���̾ƿ��� ����.
	if (mLayout)
	{
		mLayout->Release();
		mLayout = nullptr;
	}

	// �ȼ� ���̴��� ����.
	if (mPixelShader != nullptr)
	{
		mPixelShader->Release();
		mPixelShader = nullptr;
	}

	// ���ؽ� ���̴��� ����.
	if (mVertexShader != nullptr)
	{
		mVertexShader->Release();
		mVertexShader = nullptr;
	}

	return;
}

// ���ؽ�-�ȼ� ���̴��� ���� �޼����� �����.
void FontShaderClass::OutputShaderErrorMessage(ID3DBlob* ErrorMessage, HWND hWnd, WCHAR* ShaderFileName)
{
	char* CompileErrors;
	unsigned __int64 bufferSize;
	std::ofstream Fout;

	// ���� �޼����� ��� �ִ� ���ڿ� ������ �����͸� ������.
	CompileErrors = reinterpret_cast<char*>(ErrorMessage->GetBufferPointer());

	// �޼����� ���̸� ������.
	bufferSize = ErrorMessage->GetBufferSize();

	// ������ ���� �ȿ� �޼����� ���.
	Fout.open("shader-error.txt");

	// ���� �޼����� ��.
	for (int i = 0; i < bufferSize; ++i)
	{
		Fout << CompileErrors[i];
	}

	// ������ ����.
	Fout.close();

	// ���� �޼����� ��ȯ.
	ErrorMessage->Release();
	ErrorMessage = nullptr;

	// ������ ������ ������ �˾� �޼����� �˷���.
	MessageBoxW(hWnd, L"Error compiling shader. Check shader-error.txt for message.", ShaderFileName, MB_OK);

	return;
}

bool FontShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext,
	DirectX::XMMATRIX& WorldMat, DirectX::XMMATRIX& ViewMat, DirectX::XMMATRIX& ProjectionMat, ID3D11ShaderResourceView* Texture,
	DirectX::FXMVECTOR PixelColor)
{
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ConstantBufferType* ConstantDataPtr;
	unsigned __int32 BufferNumber;
	PixelBufferType* DataPtr;

	// ����� ��ġ�Ͽ� ���̴����� ����� �� �ֵ��� ��.
	WorldMat = DirectX::XMMatrixTranspose(WorldMat);
	ViewMat = DirectX::XMMatrixTranspose(ViewMat);
	ProjectionMat = DirectX::XMMatrixTranspose(ProjectionMat);

	// ��� ������ ������ �� �� �ֵ��� ���.
	Result = DeviceContext->Map(mConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// ������ۿ� �����Ϳ� ���� �����͸� ������.
	ConstantDataPtr = reinterpret_cast<ConstantBufferType*>(MappedResource.pData);

	// ��� ���ۿ� ����� ������.
	DirectX::XMStoreFloat4x4(&ConstantDataPtr->World, WorldMat);
	DirectX::XMStoreFloat4x4(&ConstantDataPtr->View, ViewMat);
	DirectX::XMStoreFloat4x4(&ConstantDataPtr->Projection, ProjectionMat);

	// ��� ������ ����� ǰ.
	DeviceContext->Unmap(mConstantBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� ����.
	BufferNumber = 0;

	// ���������� ���� ���̴��� ���۸� �ٲﰪ���� �ٲ�.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &mConstantBuffer);

	// �ȼ� ���̴��� ���̴� �ؽ��� ���ҽ��� �Ҵ�.
	DeviceContext->PSSetShaderResources(0, 1, &Texture);

	// ����Ʈ ��� ������ ������ �� �ֵ��� ���.
	Result = DeviceContext->Map(mPixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(Result))
	{
		return false;
	}

	// ����Ʈ ��� ���ۿ� ���� �����͸� ������.
	DataPtr = reinterpret_cast<PixelBufferType*>(MappedResource.pData);

	// ����Ʈ ��� ���ۿ� ����Ʈ ���� ������ ������ ����.
	DirectX::XMStoreFloat4(&DataPtr->PixelColor, PixelColor);

	// ����Ʈ ��� ������ ����� ǰ.
	DeviceContext->Unmap(mPixelBuffer, 0);

	// �ȼ� ���̴����� ����Ʈ ��� ���ۿ� ���� ��ġ�� ����.
	BufferNumber = 0;

	// �ȼ� ���̴��� ����Ʈ ��� ������ ���� ����.
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &mPixelBuffer);

	return true;
}

void FontShaderClass::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	// ���� �Է� ���̾ƿ��� ����.
	DeviceContext->IASetInputLayout(mLayout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� ����.
	DeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	DeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	// �ؽ��� ���÷� ���¸� �ȼ� ���̴��� �Ҵ�.
	DeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// �ﰢ���� �׸�.
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

