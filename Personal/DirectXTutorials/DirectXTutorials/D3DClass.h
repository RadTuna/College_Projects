#pragma once


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")


#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>


// Ʃ�丮��� �޸� d3dx10Math.h�� SDK�� ���ԵǾ� ��������.
// ����, MS���� �����ϴ� ��� DirectXMath.h�� ����ϱ�� ������.
class D3DClass
{
public:

	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	DirectX::XMMATRIX GetProjectionMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetOrthoMatrix() const;

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

private:

	bool mVSyncEnabled;
	int mVideoCardMemory;
	char mVideoCardDescription[128];
	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11RasterizerState* mRasterState;
	DirectX::XMFLOAT4X4 mProjectionMatrix;
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mOrthoMatrix;
	ID3D11DepthStencilState* mDepthDisabledStencilState;
	ID3D11BlendState* mAlphaEnableBlendingState;
	ID3D11BlendState* mAlphaDisableBlendingState;

};

