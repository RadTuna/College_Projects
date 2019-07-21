
#include "FBXParser.h"

FBXParser::FBXParser()
{
	mFbxManager = nullptr;
	mFbxMesh = nullptr;
	mFbxNode = nullptr;
	mFbxScene = nullptr;
}

FBXParser::FBXParser(const FBXParser&)
{
}

FBXParser::~FBXParser()
{
}

bool FBXParser::Initialize(const char* FBXFilename)
{
	// FBX 관리자 객체를 생성.
	mFbxManager = FbxManager::Create();
	if (mFbxManager == nullptr)
	{
		return false;
	}

	// FBX 관리자 객체를 이용하여 씬 객체 생성.
	mFbxScene = FbxScene::Create(mFbxManager, "Scene");
	if (mFbxScene == nullptr)
	{
		return false;
	}

	// FBX 노드 객체를 생성.
	mFbxNode = FbxNode::Create(mFbxScene, "FBXNode");
	if (mFbxNode == nullptr)
	{
		return false;
	}

	// FBX 메쉬 객체를 생성.
	mFbxMesh = FbxMesh::Create(mFbxScene, "FBXMesh");
	if (mFbxMesh == nullptr)
	{
		return false;
	}

	// IOSettings 객체 생성.
	mIOSettings = FbxIOSettings::Create(mFbxManager, "IOSROOT");
	if (mIOSettings == nullptr)
	{
		return false;
	}

	// IOSetting을 관리자 객체에 등록.
	mFbxManager->SetIOSettings(mIOSettings);

	// FBXImporter 객체 생성.
	mImporter = FbxImporter::Create(mFbxManager, "FBXImporter");
	if (mImporter == nullptr)
	{
		return false;
	}
	
	// FBXImporter 객체 초기화.
	bool Result = mImporter->Initialize(FBXFilename, -1, mIOSettings);
	if (Result == false)
	{
		return false;
	}

	return true;
}

bool FBXParser::ImportScene()
{
	bool Result;

	// FBX 파일 내용을 씬으로 가져옴.
	Result = mImporter->Import(mFbxScene);
	if (Result == false)
	{
		return false;
	}

	FbxNode* RootNode = mFbxScene->GetRootNode();
	if (RootNode == nullptr)
	{
		return false;
	}

	LoadNode(RootNode);

	// 좌표계를 가져옴.
	FbxAxisSystem SceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();

	// DirectX에 맞춰서 좌표계를 변환함.
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	// 씬에서 트라이앵글로 변환한 수 있는 모든 노드를 변환함.
	FbxGeometryConverter GerometryConverter(mFbxManager);
	GerometryConverter.Triangulate(mFbxScene, true);

	return true;

}

// 재귀적으로 하위의 노드를 탐색하는 함수.
void FBXParser::LoadNode(FbxNode* InNode)
{

	FbxNodeAttribute* NodeAttribute = InNode->GetNodeAttribute();
	if (NodeAttribute != nullptr)
	{
		// 메쉬에 대한 작업을 수행.
		if (NodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FBXMesh로 캐스팅 된 노드 속성의 포인터를 가져옴.
			FbxMesh* Mesh = InNode->GetMesh();
			ProcessControlPoints(Mesh);

			int TriCount = Mesh->GetPolygonCount();
			int VertexCount = 0;

			// 버텍스 데이터를 저장하기 위한 Vertices 객체 생성.
			mFBXVertices = new FBXVertex[TriCount * 3];
			if (mFBXVertices == nullptr)
			{
				return;
			}

			for (int i = 0; i < TriCount; ++i) // 트라이앵글의 개수.
			{
				for (int j = 0; j < 3; ++j) // 트라이앵글은 3개의 버텍스로 구성됨.
				{
					FBXVertex TempVertex;

					// ControlPoint 인덱스를 가져옴.
					int ControlPointIndex = Mesh->GetPolygonVertex(i, j);

					// 현재 버텍스의 위치.
					TempVertex.Position = mPositions[ControlPointIndex];

					// 현재 노말 벡터
					DirectX::XMStoreFloat3(&TempVertex.Normal, ReadNormal(Mesh, ControlPointIndex, VertexCount));
					DirectX::XMStoreFloat3(&TempVertex.Binormal, ReadBinormal(Mesh, ControlPointIndex, VertexCount));
					DirectX::XMStoreFloat3(&TempVertex.Tangnet, ReadTangent(Mesh, ControlPointIndex, VertexCount));

					// 현재 텍스쳐 UV
					DirectX::XMStoreFloat2(&TempVertex.UV, ReadUV(Mesh, ControlPointIndex, Mesh->GetTextureUVIndex(i, j)));

					// 버텍스 데이터를 저장함.
					StoreVertexData(TempVertex, VertexCount);

					// 버텍스의 개수.
					VertexCount++;
				}
			}
		}
	}

	const int ChildCount = InNode->GetChildCount();

	for (int i = 0; i < ChildCount; ++i)
	{
		LoadNode(InNode->GetChild(i));
	}
}

void FBXParser::ProcessControlPoints(FbxMesh* InMesh)
{
	int Count = InMesh->GetControlPointsCount();
	
	mPositions = new DirectX::XMFLOAT3[Count];

	for (int i = 0; i < Count; ++i)
	{
		mPositions[i].x = static_cast<float>(InMesh->GetControlPointAt(i).mData[0]); // X좌표
		mPositions[i].y = static_cast<float>(InMesh->GetControlPointAt(i).mData[1]); // Y좌표
		mPositions[i].z = static_cast<float>(InMesh->GetControlPointAt(i).mData[2]); // Z좌표
	}
}

DirectX::XMVECTOR FBXParser::ReadNormal(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter)
{
	// 노말을 얻음.
	const FbxGeometryElementNormal* VertexNormal = InMesh->GetElementNormal(0);
	if (VertexNormal == nullptr)
	{
		return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// 노말 벡터를 저장할 벡터
	DirectX::XMVECTOR ResultNormal;

	switch (VertexNormal->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (VertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					ResultNormal = DirectX::XMVectorSet(
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(ControlPointIndex).mData[0]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(ControlPointIndex).mData[1]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(ControlPointIndex).mData[2]), 0.0f);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int Index = VertexNormal->GetIndexArray().GetAt(ControlPointIndex);
					ResultNormal = DirectX::XMVectorSet(
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[0]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[1]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
					break;
				}
			}
			break;
		}
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (VertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					ResultNormal = DirectX::XMVectorSet(
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(VertexCounter).mData[0]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(VertexCounter).mData[1]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(VertexCounter).mData[2]), 0.0f);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int Index = VertexNormal->GetIndexArray().GetAt(VertexCounter);
					ResultNormal = DirectX::XMVectorSet(
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[0]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[1]),
						static_cast<float>(VertexNormal->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
					break;
				}
			}
			break;
		}
	}

	return ResultNormal;
}

DirectX::XMVECTOR FBXParser::ReadBinormal(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter)
{
	const FbxGeometryElementBinormal* VertexBinormal = InMesh->GetElementBinormal(0);
	if (VertexBinormal == nullptr)
	{
		return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	DirectX::XMVECTOR ResultBinormal;

	switch (VertexBinormal->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (VertexBinormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					ResultBinormal = DirectX::XMVectorSet(
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(ControlPointIndex).mData[0]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(ControlPointIndex).mData[1]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(ControlPointIndex).mData[2]), 0.0f);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int Index = VertexBinormal->GetIndexArray().GetAt(ControlPointIndex);
					ResultBinormal = DirectX::XMVectorSet(
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[0]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[1]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
					break;
				}
			}
			break;
		}
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (VertexBinormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					ResultBinormal = DirectX::XMVectorSet(
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(VertexCounter).mData[0]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(VertexCounter).mData[1]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(VertexCounter).mData[2]), 0.0f);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int Index = VertexBinormal->GetIndexArray().GetAt(VertexCounter);
					ResultBinormal = DirectX::XMVectorSet(
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[0]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[1]),
						static_cast<float>(VertexBinormal->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
					break;
				}
			}
			break;
		}
	}

	return ResultBinormal;
}

DirectX::XMVECTOR FBXParser::ReadTangent(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter)
{
	const FbxGeometryElementTangent* VertexTangent = InMesh->GetElementTangent(0);
	if (VertexTangent == nullptr)
	{
		return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	DirectX::XMVECTOR ResultTangent;

	switch (VertexTangent->GetMappingMode())
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (VertexTangent->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			{
				ResultTangent = DirectX::XMVectorSet(
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(ControlPointIndex).mData[0]),
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(ControlPointIndex).mData[1]),
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(ControlPointIndex).mData[2]), 0.0f);
				break;
			}
			case FbxGeometryElement::eIndexToDirect:
			{
				int Index = VertexTangent->GetIndexArray().GetAt(ControlPointIndex);
				ResultTangent = DirectX::XMVectorSet(
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[0]),
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[1]),
					static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
				break;
			}
			}
			break;
		}
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (VertexTangent->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					ResultTangent = DirectX::XMVectorSet(
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(VertexCounter).mData[0]),
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(VertexCounter).mData[1]),
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(VertexCounter).mData[2]), 0.0f);
					break;
				}
				case FbxGeometryElement::eIndexToDirect:
				{
					int Index = VertexTangent->GetIndexArray().GetAt(VertexCounter);
					ResultTangent = DirectX::XMVectorSet(
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[0]),
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[1]),
						static_cast<float>(VertexTangent->GetDirectArray().GetAt(Index).mData[2]), 0.0f);
					break;
				}
			}
			break;
		}
	}

	return ResultTangent;
}

DirectX::XMVECTOR FBXParser::ReadUV(const FbxMesh* InMesh, int ControlPointIndex, int TextureCounter)
{
	const FbxGeometryElementUV* TextureUV = InMesh->GetElementUV(0);

	DirectX::XMVECTOR ResultUV;

	switch (TextureUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
	{
		switch (TextureUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			ResultUV = DirectX::XMVectorSet(
				static_cast<float>(TextureUV->GetDirectArray().GetAt(ControlPointIndex).mData[0]),
				static_cast<float>(TextureUV->GetDirectArray().GetAt(ControlPointIndex).mData[1]), 0.0f, 0.0f);
			break;
		}
		case FbxGeometryElement::eIndexToDirect:
		{
			int Index = TextureUV->GetIndexArray().GetAt(ControlPointIndex);
			ResultUV = DirectX::XMVectorSet(
				static_cast<float>(TextureUV->GetDirectArray().GetAt(Index).mData[0]),
				static_cast<float>(TextureUV->GetDirectArray().GetAt(Index).mData[1]), 0.0f, 0.0f);
			break;
		}
		}
		break;
	}
	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (TextureUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			ResultUV = DirectX::XMVectorSet(
				static_cast<float>(TextureUV->GetDirectArray().GetAt(TextureCounter).mData[0]),
				static_cast<float>(TextureUV->GetDirectArray().GetAt(TextureCounter).mData[1]), 0.0f, 0.0f);
			break;
		}
		case FbxGeometryElement::eIndexToDirect:
		{
			int Index = TextureUV->GetIndexArray().GetAt(TextureCounter);
			ResultUV = DirectX::XMVectorSet(
				static_cast<float>(TextureUV->GetDirectArray().GetAt(Index).mData[0]),
				static_cast<float>(TextureUV->GetDirectArray().GetAt(Index).mData[1]), 0.0f, 0.0f);
			break;
		}
		}
		break;
	}
	}

	return ResultUV;
}

// 획득한 FBX정보를 DirectX 프로그램에서 필요한 정보로 변환하는 함수.
// 내부에 사용하는 정보에 변경점이 생기면 이 부분을 수정하면 됨.
void FBXParser::StoreVertexData(FBXVertex& InVertex, int VertexCount)
{
	mFBXVertices[VertexCount].Position = InVertex.Position;
	mFBXVertices[VertexCount].Normal = InVertex.Normal;
	mFBXVertices[VertexCount].Binormal = InVertex.Binormal;
	mFBXVertices[VertexCount].Tangnet = InVertex.Tangnet;
	mFBXVertices[VertexCount].UV = InVertex.UV;

	mVertexCount = mIndexCount = VertexCount;
}

void FBXParser::Release()
{
	// 위치 벡터 배열을 해제.
	if (mPositions != nullptr)
	{
		delete[] mPositions;
		mPositions = nullptr;
	}

	// 메쉬 객체를 해제.
	if (mFbxMesh != nullptr)
	{
		mFbxMesh->Destroy();
		mFbxMesh = nullptr;
	}

	// 노드 객체를 해제.
	if (mFbxNode != nullptr)
	{
		mFbxNode->Destroy();
		mFbxNode = nullptr;
	}

	// 씬 객체 해제.
	if (mFbxScene != nullptr)
	{
		mFbxScene->Destroy();
		mFbxScene = nullptr;
	}

	// 관리자 객체 해제.
	if (mFbxScene != nullptr)
	{
		mFbxManager->Destroy();
	}
}