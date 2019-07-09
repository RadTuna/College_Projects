
#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	mVertexShader = nullptr;
	mPixelShader = nullptr;
	mLayout = nullptr;
	mMatrixBuffer = nullptr;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* Device, HWND hWnd)
{
	bool Result;

	// ���ؽ� ���̴��� �ȼ� ���̴��� �ʱ�ȭ.
	Result = InitializeShader(Device, hWnd, const_cast<WCHAR*>(L"../Engine/ColorVS.vs"), const_cast<WCHAR*>(L"../Engine/ColorPS.ps"));
	if (Result == false)
	{
		return false;
	}

	return true;
}

void ColorShaderClass::Shutdown()
{
	// ���ؽ� ���̴��� �ȼ� ���̴� �� �׿� ���õ� �͵��� ����.
	ShutdownShader();
	
	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::FXMMATRIX WorldMat, DirectX::CXMMATRIX ViewMat, DirectX::CXMMATRIX ProjectionMat)
{
	bool Result;

	// �������� ����� ���̴��� ���ڸ� �Է�.
	Result = SetShaderParameters(DeviceContext, WorldMat, ViewMat, ProjectionMat);
	if (Result == false)
	{
		return false;
	}

	// ���̴��� �̿��Ͽ� �غ�� ���۸� �׸�.
	RenderShader(DeviceContext, IndexCount);

	return true;
}

// ���̴� ������ �ҷ��� DirectX�� GPU���� ��밡�� �ϵ��� �ϴ� �Լ� (�߿��� �Լ���)
bool ColorShaderClass::InitializeShader(ID3D11Device* Device, HWND hWnd, WCHAR* VsFileName, WCHAR* PsFileName)
{
	HRESULT Result;
	ID3DBlob* ErrorMessage;
	ID3DBlob* VertexShaderBuffer;
	ID3DBlob* PixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	unsigned __int32 NumElements;
	D3D11_BUFFER_DESC MatrixBufferDesc;

	// �� �Լ����� ����ϴ� �����͸� �ʱ�ȭ
	ErrorMessage = nullptr;
	VertexShaderBuffer = nullptr;
	PixelShaderBuffer = nullptr;

	// ���⼭ ���̴� ���α׷��� ���۷� ��������.

	// ���ؽ� ���̴��� ������.
	Result = D3DCompileFromFile(VsFileName, nullptr, nullptr, "ColorPixelShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
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
			char* FileName;
			wcstombs_s(nullptr, FileName, 128, VsFileName, 128);
			MessageBox(hWnd, FileName, "Missing VertexShader File", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴��� ������.
	Result = D3DCompileFromFile(PsFileName, nullptr, nullptr, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
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
			char* FileName;
			wcstombs_s(nullptr, FileName, 128, PsFileName, 128);
			MessageBox(hWnd, FileName, "Missing PixelShader File", MB_OK);
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

	PolygonLayout[0].SemanticName = "COLOR";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

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
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;
	MatrixBufferDesc.

	// ��� ���� �����͸� ����� �� Ŭ�������� ���� ���̴� ��� ���ۿ� ������ �� �ֵ��� ��.
	Result = Device->CreateBuffer(&MatrixBufferDesc, nullptr, &mMatrixBuffer);
	if (FAILED(Result))
	{
		return false;
	}

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// ��� ���۸� ����.
	if (mMatrixBuffer != nullptr)
	{
		mMatrixBuffer->Release();
		mMatrixBuffer = nullptr;
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
void ColorShaderClass::OutputShaderErrorMessage(ID3DBlob* ErrorMessage, HWND hWnd, WCHAR* ShaderFileName)
{
	char* CompileErrors;
	unsigned __int64 bufferSize;
	unsigned __int64 i;
	std::ofstream Fout;

	// ���� �޼����� ��� �ִ� ���ڿ� ������ �����͸� ������.
	CompileErrors = reinterpret_cast<char*>(ErrorMessage->GetBufferPointer());

	// �޼����� ���̸� ������.
	bufferSize = ErrorMessage->GetBufferSize();

	// ������ ���� �ȿ� �޼����� ���.
	Fout.open("shader-error.txt");

	// ���� �޼����� ��.
	for (i = 0; i < bufferSize; ++i)
	{
		Fout << CompileErrors[i];
	}

	// ������ ����.
	Fout.close();

	// ���� �޼����� ��ȯ.
	ErrorMessage->Release();
	ErrorMessage = nullptr;

	// ������ ������ ������ �˾� �޼����� �˷���.
	char* FileName;
	wcstombs_s(nullptr, FileName, 128, ShaderFileName, 128);
	MessageBox(hWnd, "Error compiling shader. Check shader-error.txt for message.", FileName, MB_OK);
}

