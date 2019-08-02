
#pragma once

#include "FontClass.h"
#include "FontShaderClass.h"


class TextClass
{
private:

	struct SentenceType
	{
		ID3D11Buffer* VertexBuffer;
		ID3D11Buffer* IndexBuffer;
		int VertexCount;
		int IndexCount;
		int MaxLength;
		float Red;
		float Green;
		float Blue;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};

public:

	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, DirectX::FXMMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&);
	bool SetFPS(int, ID3D11DeviceContext*);
	bool SetCPU(int, ID3D11DeviceContext*);

private:

	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, const char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, DirectX::XMMATRIX&, DirectX::XMMATRIX&);

private:

	FontClass* mFont;
	FontShaderClass* mFontShader;
	int mScreenWidth;
	int mScreenHeight;
	DirectX::XMFLOAT4X4 mBaseViewMatrix;
	SentenceType* mSentence1;
	SentenceType* mSentence2;

};
