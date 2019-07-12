
// ModelClass�� 3D���� ���ϱ����� ĸ��ȭ�ϴ� Ŭ����
// �ﰢ���� ����� ���� �����Ϳ� ��������, �ε������۸� ������.

#pragma once


#include <d3d11.h>
#include <DirectXMath.h>


class ModelClass
{
private:

	// ���ؽ� ���ۿ� ����� ����ü�� ����. ���ؽ� ���̴��� ����ü�� ������ ���ƾ� ��.
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

public:

	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*); // �ʱ�ȭ
	void Shutdown(); // ����
	void Render(ID3D11DeviceContext*); 

	int GetIndexCount() const;

private:

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:

	// ���� ������ ���� ������ Description���� ������.
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;

};


