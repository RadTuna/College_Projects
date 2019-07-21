#pragma once

#include <fbxsdk.h>
#include <fbxsdk/scene/fbxaxissystem.h>
#include <DirectXMath.h>


struct FBXVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Binormal;
	DirectX::XMFLOAT3 Tangnet;
};

class FBXParser
{
public:

	FBXParser();
	FBXParser(const FBXParser&);
	~FBXParser();

	bool Initialize(const char* FBXFilename);
	bool ImportScene();
	void Release();

private:

	void LoadNode(FbxNode* InNode);
	void ProcessControlPoints(FbxMesh* InMesh);
	void StoreVertexData(FBXVertex& InVertex, int VertexCount);

	DirectX::XMVECTOR ReadNormal(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter);
	DirectX::XMVECTOR ReadBinormal(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter);
	DirectX::XMVECTOR ReadTangent(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter);
	DirectX::XMVECTOR ReadUV(const FbxMesh* InMesh, int ControlPointIndex, int TextureCounter);

private:

	FbxManager* mFbxManager;
	FbxIOSettings* mIOSettings;
	FbxScene* mFbxScene;
	FbxNode* mFbxNode;
	FbxMesh* mFbxMesh;
	FbxImporter* mImporter;
	DirectX::XMFLOAT3* mPositions;

	FBXVertex* mFBXVertices;
	int mVertexCount;
	int mIndexCount;

public:

	inline FBXVertex* GetFBXVertices() const { return mFBXVertices; }
	inline int GetVertexCount() const { return mVertexCount; }
	inline int GetIndexCount() const { return mIndexCount; }

};
