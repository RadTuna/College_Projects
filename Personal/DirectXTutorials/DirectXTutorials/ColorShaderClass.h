
// GPU���� 3D���� �׸��� �� ����ϴ� HLSL���̴��� ȣ���ϴ� Ŭ����

#pragma once


#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>


class ColorShaderClass
{
private:

	// ���ؽ� ���̴��� cbuffer ����ü�� ������ �����ؾ� ��.
	struct MatrixBufferType
	{
		DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
	};

public:

	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::FXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX);

private:

	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::FXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* mMatrixBuffer;

};
