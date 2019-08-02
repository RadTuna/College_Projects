
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

	// 메모리에서 Targa이미지 데이터를 불러옴.
	Result = LoadTarga(FileName, Height, Width);
	if (Result == false)
	{
		return false;
	}

	// 텍스쳐 Description을 작성.
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

	// 공백 텍스쳐를 생성.
	hResult = Device->CreateTexture2D(&TextureDesc, nullptr, &mTexture);
	if (FAILED(hResult))
	{
		return false;
	}

	// Targa이미지 데이터의 행 높이를 지정.
	unsigned __int32 RowPitch = (Width * 4) * sizeof(unsigned char);

	// Targa이미지 데이터를 텍스쳐에 복사.
	DeviceContext->UpdateSubresource(mTexture, 0, nullptr, mTargaData, RowPitch, 0);

	// 셰이더 리소스 뷰 Description을 작성.
	SrvDesc.Format = TextureDesc.Format;
	SrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SrvDesc.Texture2D.MostDetailedMip = 0;
	SrvDesc.Texture2D.MipLevels = -1;

	// 텍스쳐를 위한 셰이더 리소스 뷰를 생성.
	hResult = Device->CreateShaderResourceView(mTexture, &SrvDesc, &mTextureView);
	if (FAILED(hResult))
	{
		return false;
	}

	// 텍스쳐를 위한 Mipmap을 생성.
	DeviceContext->GenerateMips(mTextureView);

	// 이미지가 텍스쳐에 로드 되었으므로 Targa데이터를 해제.
	delete[] mTargaData;
	mTargaData = nullptr;

	return true;
}

void TextureClass::Shutdown()
{
	// 텍스쳐 뷰 리소스를 해제.
	if (mTextureView != nullptr)
	{
		mTextureView->Release();
		mTextureView = nullptr;
	}

	// 텍스쳐를 해제.
	if (mTexture != nullptr)
	{
		mTexture->Release();
		mTexture = nullptr;
	}

	// Targa데이터를 해제.
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

	// 바이너리 모드로 Targa파일을 염.
	Error = fopen_s(&FilePtr, FileName, "rb");
	if (Error != 0)
	{
		return false;
	}

	// 헤더 파일을 읽음.
	Count = static_cast<unsigned int>(fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr));
	if (Count != 1)
	{
		return false;
	}

	// 헤더의 중요정보를 읽음.
	Height = static_cast<int>(TargaFileHeader.Height);
	Width = static_cast<int>(TargaFileHeader.Width);
	Bpp = static_cast<int>(TargaFileHeader.Bpp);

	// 파일구조가 32비트 인지 확인.
	if (Bpp != 32)
	{
		return false;
	}

	// 32bit 이미지의 크기를 계산.
	int ImageSize = Width * Height * 4; // RGBA를 가지고 있기에 4를 곱함.

	// Targa이미지 데이터를 메모리에 할당.
	TargaImage = new unsigned char[ImageSize];
	if (TargaImage == nullptr)
	{
		return false;
	}

	// Targa이미지 데이터를 읽음.
	Count = static_cast<unsigned int>(fread(TargaImage, 1, ImageSize, FilePtr));
	if (Count != ImageSize)
	{
		return false;
	}

	// 파일을 닫음.
	Error = fclose(FilePtr);
	if (Error != 0)
	{
		return false;
	}

	// Targa데이터에 메모리를 할당.
	mTargaData = new unsigned char[ImageSize];
	if (mTargaData == nullptr)
	{
		return false;
	}

	// Targa데이터 배열의 인덱스를 초기화.
	int Index = 0;

	// Targa이미지 데이터의 인덱스를 초기화.
	int k = (Width * Height * 4) - (Width * 4);

	// Targa파일은 거꾸로 저장되어 있으므로 올바른 순서로 만든 후 Targa데이터 배열에 복사함.
	for (int i = 0; i < Height; ++i)
	{
		for (int j = 0; j < Width; ++j)
		{
			mTargaData[Index + 0] = TargaImage[k + 2]; // Red
			mTargaData[Index + 1] = TargaImage[k + 1]; // Green
			mTargaData[Index + 2] = TargaImage[k + 0]; // Blue
			mTargaData[Index + 3] = TargaImage[k + 3]; // Alpha

			// Targa데이터의 인덱스를 증가.
			k += 4;
			Index += 4;
		}
		// Targa이미지 데이터를 역순으로 읽은 후 행을 되돌림.
		k -= (Width * 8);
	}

	// 배열에 복사를 했으므로 Targa이미지 데이터를 해제.
	delete[] TargaImage;
	TargaImage = nullptr;

	return true;
}
