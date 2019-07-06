#pragma once

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "Lib/d3dx12.h"
#include "Lib/DDSTextureLoader.h"
#include "Lib/MathHelper.h"

extern const int gNumFrameResources;

inline void D3DSetDebugName(IDXGIObject* Obj, const char* Name)
{
	if (Obj != nullptr)
	{
		Obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(Name), Name);
	}
}

inline void D3DSetDebugName(ID3D12Device* Obj, const char* Name)
{
	if (Obj != nullptr)
	{
		Obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(Name), Name);
	}
}

inline void D3DSetDebugName(ID3D12DeviceChild* Obj, const char* Name)
{
	if (Obj != nullptr)
	{
		Obj->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(Name), Name);
	}
}

inline std::wstring AnsiToWString(const std::string& Str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, Str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}


class D3DUtil
{
public:

	static bool IsKeyDown(int VKeyCode);

	static std::string ToString(HRESULT HR);

	static UINT CalcConstantBufferByteSize(UINT ByteSize)
	{
		return (ByteSize + 255) & ~255;
	}

	static Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& FileName);

	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device* Device,
		ID3D12GraphicsCommandList* CmdList,
		const void* InitData,
		UINT64 ByteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& UploadBuffer);

	static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(
		const std::wstring& FileName,
		const D3D_SHADER_MACRO* Defines,
		const std::string& EntryPoint,
		const std::string& Target);
};

class DxException
{
public:
	DxException() = default;
	DxException(HRESULT HR, const std::wstring& FunctionName, const std::wstring& FileName, int LineNumber);

	std::wstring ToString() const;

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring FileName;
	int LineNumber = -1;
};

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	DirectX::BoundingBox Bounds;
};

struct MeshGeometry
{
	std::string Name;

	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;


	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW VBV;
		VBV.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		VBV.SizeInBytes = VertexBufferByteSize;
		VBV.StrideInBytes = VertexByteStride;

		return VBV;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW IBV;
		IBV.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		IBV.Format = IndexFormat;
		IBV.SizeInBytes = IndexBufferByteSize;

		return IBV;
	}

	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}
};

struct Light
{
	DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };
	float FalloffStart = 1.0f;
	DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };
	float FalloffEnd = 10.0f;
	DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };
	float SpotPower = 64.0f;
};

#define MAX_LIGHTS 16

struct MaterialConstants
{
	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;

	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Material
{
	std::string Name;

	int MatCBIndex = -1;
	int DiffuseSrvHeapIndex = -1;
	int NormalSrvHeapIndex = -1;
	int NumFramesDirty = gNumFrameResources;

	DirectX::XMFLOAT4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 FresnelR0 = { 0.01f, 0.01, 0.01 };
	float Roughness = 0.25f;
	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();
};

struct Texture
{
	std::string Name;
	std::wstring FileName;

	Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
};

#ifdef ThrowIfFailed
#define ThrowIfFailed(x)												\
{																		\
	HRESULT HR__ = (x);													\
	std::wstring WFN = AnsiToWString(__FILE__);							\
	if (FAILED(HR__)) { throw DxException(HR__, L#x, WFN, __LINE__); }	\
}
#endif

#ifdef ReleaseCom
#define ReleaseCom(x) { if (x) {x->Release(); x = nullptr; } }
#endif

