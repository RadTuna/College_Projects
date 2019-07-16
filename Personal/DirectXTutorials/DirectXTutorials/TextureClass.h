#pragma once


#include <d3d11.h>
#include <stdio.h>

class TextureClass
{
private:

	struct TargaHeader
	{
		unsigned char Data1[12];
		unsigned short Width;
		unsigned short Height;
		unsigned char Bpp;
		unsigned char Data2;
	};

public:

	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();
	bool LoadTarga(const char*, int&, int&);

private:

	unsigned char* mTargaData;
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mTextureView;

};
