
// GPU���� 3D���� �׸��� �� ����ϴ� HLSL���̴��� ȣ���ϴ� Ŭ����

#pragma once


#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>


class LightShaderClass
{
private:

	// ���ؽ� ���̴��� cbuffer ����ü�� ������ �����ؾ� ��.
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
	};
	
	struct LightBufferType
	{
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT3 LightDirection;
		float Padding;
	};

public:

	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, DirectX::FXMVECTOR, DirectX::FXMVECTOR);

private:

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, DirectX::FXMVECTOR, DirectX::FXMVECTOR);
	void RenderShader(ID3D11DeviceContext*, int);

private:

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;
	ID3D11SamplerState* mSampleState;
	ID3D11Buffer* mLightBuffer;

};
