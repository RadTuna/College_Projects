#pragma once


#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"


// �̹����� ǥ���ϴ� Ŭ���� // ��� 2D �̹����� �ش� �ν��Ͻ��� ��������.
// ModelClass�� ����.
class BitmapClass
{
private:
	
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 Texture;
	};

public:

	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, const char*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

private:

	bool InitializeBuffers(ID3D11Device*);
	void ShutdonwBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	void ReleaseTexture();

private:

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;
	TextureClass* mTexture;
	int mScreenWidth, mScreenHeight;
	int mBitmapWidth, mBitmapHeight;
	int mPrevPosX, mPrevPosY;

public:

	inline int GetIndexCount() { return mIndexCount; }
	inline ID3D11ShaderResourceView* GetTexture() { return mTexture->GetTexture(); }

};