
#define ThrowIfFailed

#include <comdef.h>
#include "D3DUtil.h"

using Microsoft::WRL::ComPtr;

DxException::DxException(HRESULT HR, const std::wstring& FunctionName, const std::wstring& FileName, int LineNumber)
	: ErrorCode(HR),
	FunctionName(FunctionName),
	FileName(FileName),
	LineNumber(LineNumber)
{
}

bool D3DUtil::IsKeyDown(int VKeyCode)
{
	return (GetAsyncKeyState(VKeyCode) & 0x8000) != 0;
}

ComPtr<ID3DBlob> D3DUtil::LoadBinary(const std::wstring& FileName)
{
	std::ifstream Fin(FileName, std::ios::binary);

	Fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type Size = static_cast<int>(Fin.tellg());
	Fin.seekg(0, std::ios_base::beg);

	ComPtr<ID3DBlob> Blob;
	ThrowIfFailed(D3DCreateBlob(Size, Blob.GetAddressOf()));

	Fin.read(reinterpret_cast<char*>(Blob->GetBufferPointer()), Size);
	Fin.close();

	return Blob;
}

ComPtr<ID3D12Resource> D3DUtil::CreateDefaultBuffer(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList,
	const void* InitData, UINT64 ByteSize, Microsoft::WRL::ComPtr<ID3D12Resource>& UploadBuffer)
{
	ComPtr<ID3D12Resource> DefaultBuffer;

	ThrowIfFailed(Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ByteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(DefaultBuffer.GetAddressOf())));

	ThrowIfFailed(Device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(UploadBuffer.GetAddressOf())));

	D3D12_SUBRESOURCE_DATA SubResourceData = { };
	SubResourceData.pData = InitData;
	SubResourceData.RowPitch = ByteSize;
	SubResourceData.SlicePitch = SubResourceData.RowPitch;

	CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(CmdList, DefaultBuffer.Get(), UploadBuffer.Get(), 0, 0, 1, &SubResourceData);
	CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	return DefaultBuffer;
}

ComPtr<ID3DBlob> D3DUtil::CompileShader(const std::wstring& FileName, const D3D_SHADER_MACRO* Defines,
	const std::string& EntryPoint, const std::string& Target)
{
	UINT CompileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT HR = S_OK;

	ComPtr<ID3DBlob> ByteCode = nullptr;
	ComPtr<ID3DBlob> Errors;
	HR = D3DCompileFromFile(FileName.c_str(), Defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		EntryPoint.c_str(), Target.c_str(), CompileFlags, 0, &ByteCode, &Errors);
	if (Errors != nullptr)
	{
		OutputDebugStringA(reinterpret_cast<char*>(Errors->GetBufferPointer()));
	}

	ThrowIfFailed(HR);
	
	return ByteCode;
}

std::wstring DxException::ToString() const
{
	_com_error Err(ErrorCode);
	std::wstring Msg = Err.ErrorMessage();

	return FunctionName + L"Failed in" + FileName + L"; line" + std::to_string(LineNumber) + L"; error:" + Msg;
}

