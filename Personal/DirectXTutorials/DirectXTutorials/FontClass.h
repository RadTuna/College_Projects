#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "TextureClass.h"


class FontClass
{
private:

	struct FontType
	{
		float Left;
		float Right;
		int Size;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};

public:

	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const char*);
	void Shutdown();

	void BuildVertexArray(void*, const char*, float, float);

private:

	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ReleaseTexture();

private:

	FontType* mFont;
	TextureClass* mTexture;

public:

	inline ID3D11ShaderResourceView* GetTexture() const { return mTexture->GetTexture(); }
};
