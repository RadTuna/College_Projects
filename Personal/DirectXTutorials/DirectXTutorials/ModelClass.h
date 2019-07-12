
// ModelClass는 3D모델의 기하구조를 캡슐화하는 클래스
// 삼각형을 만들기 위한 데이터와 정점버퍼, 인덱스버퍼를 구현함.

#pragma once


#include <d3d11.h>
#include <DirectXMath.h>


class ModelClass
{
private:

	// 버텍스 버퍼에 사용할 구조체를 선언. 버텍스 셰이더의 구조체와 구조가 같아야 함.
	struct VertexType
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

public:

	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*); // 초기화
	void Shutdown(); // 해제
	void Render(ID3D11DeviceContext*); 

	int GetIndexCount() const;

private:

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:

	// 각자 형식이 동일 하지만 Description으로 구분함.
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;
	int mVertexCount;
	int mIndexCount;

};


