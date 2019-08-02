
#include "TextureClass.h"


TextureClass::TextureClass()
{
	mTargaData = nullptr;
	mTexture = nullptr;
	mTextureView = nullptr;
}

TextureClass::TextureClass(const TextureClass& Other)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	HRESULT hResult;
	int Height, Width;
	bool Result;
	D3D11_TEXTURE2D_DESC TextureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC SrvDesc;

	// �޸𸮿��� Targa�̹��� �����͸� �ҷ���.
	Result = LoadTarga(FileName, Height, Width);
	if (Result == false)
	{
		return false;
	}

	// �ؽ��� Description�� �ۼ�.
	TextureDesc.Height = Height;
	TextureDesc.Width = Width;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// ���� �ؽ��ĸ� ����.
	hResult = Device->CreateTexture2D(&TextureDesc, nullptr, &mTexture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Targa�̹��� �������� �� ���̸� ����.
	unsigned __int32 RowPitch = (Width * 4) * sizeof(unsigned char);

	// Targa�̹��� �����͸� �ؽ��Ŀ� ����.
	DeviceContext->UpdateSubresource(mTexture, 0, nullptr, mTargaData, RowPitch, 0);

	// ���̴� ���ҽ� �� Description�� �ۼ�.
	SrvDesc.Format = TextureDesc.Format;
	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SrvDesc.Texture2D.MostDetailedMip = 0;
	SrvDesc.Texture2D.MipLevels = -1;

	// �ؽ��ĸ� ���� ���̴� ���ҽ� �並 ����.
	hResult = Device->CreateShaderResourceView(mTexture, &SrvDesc, &mTextureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// �ؽ��ĸ� ���� Mipmap�� ����.
	DeviceContext->GenerateMips(mTextureView);

	// �̹����� �ؽ��Ŀ� �ε� �Ǿ����Ƿ� Targa�����͸� ����.
	delete[] mTargaData;
	mTargaData = nullptr;

	return true;
}

void TextureClass::Shutdown()
{
	// �ؽ��� �� ���ҽ��� ����.
	if (mTextureView != nullptr)
	{
		mTextureView->Release();
		mTextureView = nullptr;
	}

	// �ؽ��ĸ� ����.
	if (mTexture != nullptr)
	{
		mTexture->Release();
		mTexture = nullptr;
	}

	// Targa�����͸� ����.
	if (mTargaData != nullptr)
	{
		delete[] mTargaData;
		mTargaData = nullptr;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return mTextureView;
}

bool TextureClass::LoadTarga(const char* FileName, int& Height, int& Width)
{
	FILE* FilePtr;
	int Error;
	int Bpp;
	unsigned int Count;
	TargaHeader TargaFileHeader;
	unsigned char* TargaImage;

	// ���̳ʸ� ���� Targa������ ��.
	Error = fopen_s(&FilePtr, FileName, "rb");
	if (Error != 0)
	{
		return false;
	}

	// ��� ������ ����.
	Count = static_cast<unsigned int>(fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr));
	if (Count != 1)
	{
		return false;
	}

	// ����� �߿������� ����.
	Height = static_cast<int>(TargaFileHeader.Height);
	Width = static_cast<int>(TargaFileHeader.Width);
	Bpp = static_cast<int>(TargaFileHeader.Bpp);

	// ���ϱ����� 32��Ʈ ���� Ȯ��.
	if (Bpp != 32)
	{
		return false;
	}

	// 32bit �̹����� ũ�⸦ ���.
	int ImageSize = Width * Height * 4; // RGBA�� ������ �ֱ⿡ 4�� ����.

	// Targa�̹��� �����͸� �޸𸮿� �Ҵ�.
	TargaImage = new unsigned char[ImageSize];
	if (TargaImage == nullptr)
	{
		return false;
	}

	// Targa�̹��� �����͸� ����.
	Count = static_cast<unsigned int>(fread(TargaImage, 1, ImageSize, FilePtr));
	if (Count != ImageSize)
	{
		return false;
	}

	// ������ ����.
	Error = fclose(FilePtr);
	if (Error != 0)
	{
		return false;
	}

	// Targa�����Ϳ� �޸𸮸� �Ҵ�.
	mTargaData = new unsigned char[ImageSize];
	if (mTargaData == nullptr)
	{
		return false;
	}

	// Targa������ �迭�� �ε����� �ʱ�ȭ.
	int Index = 0;

	// Targa�̹��� �������� �ε����� �ʱ�ȭ.
	int k = (Width * Height * 4) - (Width * 4);

	// Targa������ �Ųٷ� ����Ǿ� �����Ƿ� �ùٸ� ������ ���� �� Targa������ �迭�� ������.
	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			mTargaData[Index + 0] = TargaImage[k + 2]; // Red
			mTargaData[Index + 1] = TargaImage[k + 1]; // Green
			mTargaData[Index + 2] = TargaImage[k + 0]; // Blue
			mTargaData[Index + 3] = TargaImage[k + 3]; // Alpha

			// Targa�������� �ε����� ����.
			k += 4;
			Index += 4;
		}
		// Targa�̹��� �����͸� �������� ���� �� ���� �ǵ���.
		k -= (Width * 8);
	}

	// �迭�� ���縦 �����Ƿ� Targa�̹��� �����͸� ����.
	delete[] TargaImage;
	TargaImage = nullptr;

	return true;
}
