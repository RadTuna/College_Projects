
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
	// FBX ������ ��ü�� ����.
	mFbxManager = FbxManager::Create();
	if (mFbxManager == nullptr)
	{
		return false;
	}

	// FBX ������ ��ü�� �̿��Ͽ� �� ��ü ����.
	mFbxScene = FbxScene::Create(mFbxManager, "Scene");
	if (mFbxScene == nullptr)
	{
		return false;
	}

	// FBX ��� ��ü�� ����.
	mFbxNode = FbxNode::Create(mFbxScene, "FBXNode");
	if (mFbxNode == nullptr)
	{
		return false;
	}

	// FBX �޽� ��ü�� ����.
	mFbxMesh = FbxMesh::Create(mFbxScene, "FBXMesh");
	if (mFbxMesh == nullptr)
	{
		return false;
	}

	// IOSettings ��ü ����.
	mIOSettings = FbxIOSettings::Create(mFbxManager, "IOSROOT");
	if (mIOSettings == nullptr)
	{
		return false;
	}

	// IOSetting�� ������ ��ü�� ���.
	mFbxManager->SetIOSettings(mIOSettings);

	// FBXImporter ��ü ����.
	mImporter = FbxImporter::Create(mFbxManager, "FBXImporter");
	if (mImporter == nullptr)
	{
		return false;
	}
	
	// FBXImporter ��ü �ʱ�ȭ.
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

	// FBX ���� ������ ������ ������.
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

	// ��ǥ�踦 ������.
	FbxAxisSystem SceneAxisSystem = mFbxScene->GetGlobalSettings().GetAxisSystem();

	// DirectX�� ���缭 ��ǥ�踦 ��ȯ��.
	FbxAxisSystem::DirectX.ConvertScene(mFbxScene);

	// ������ Ʈ���̾ޱ۷� ��ȯ�� �� �ִ� ��� ��带 ��ȯ��.
	FbxGeometryConverter GerometryConverter(mFbxManager);
	GerometryConverter.Triangulate(mFbxScene, true);

	return true;

}

// ��������� ������ ��带 Ž���ϴ� �Լ�.
void FBXParser::LoadNode(FbxNode* InNode)
{

	FbxNodeAttribute* NodeAttribute = InNode->GetNodeAttribute();
	if (NodeAttribute != nullptr)
	{
		// �޽��� ���� �۾��� ����.
		if (NodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// FBXMesh�� ĳ���� �� ��� �Ӽ��� �����͸� ������.
			FbxMesh* Mesh = InNode->GetMesh();
			ProcessControlPoints(Mesh);

			int TriCount = Mesh->GetPolygonCount();
			int VertexCount = 0;

			// ���ؽ� �����͸� �����ϱ� ���� Vertices ��ü ����.
			mFBXVertices = new FBXVertex[TriCount * 3];
			if (mFBXVertices == nullptr)
			{
				return;
			}

			for (int i = 0; i < TriCount; ++i) // Ʈ���̾ޱ��� ����.
			{
				for (int j = 0; j < 3; ++j) // Ʈ���̾ޱ��� 3���� ���ؽ��� ������.
				{
					FBXVertex TempVertex;

					// ControlPoint �ε����� ������.
					int ControlPointIndex = Mesh->GetPolygonVertex(i, j);

					// ���� ���ؽ��� ��ġ.
					TempVertex.Position = mPositions[ControlPointIndex];

					// ���� �븻 ����
					DirectX::XMStoreFloat3(&TempVertex.Normal, ReadNormal(Mesh, ControlPointIndex, VertexCount));
					DirectX::XMStoreFloat3(&TempVertex.Binormal, ReadBinormal(Mesh, ControlPointIndex, VertexCount));
					DirectX::XMStoreFloat3(&TempVertex.Tangnet, ReadTangent(Mesh, ControlPointIndex, VertexCount));

					// ���� �ؽ��� UV
					DirectX::XMStoreFloat2(&TempVertex.UV, ReadUV(Mesh, ControlPointIndex, Mesh->GetTextureUVIndex(i, j)));

					// ���ؽ� �����͸� ������.
					StoreVertexData(TempVertex, VertexCount);

					// ���ؽ��� ����.
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
		mPositions[i].x = static_cast<float>(InMesh->GetControlPointAt(i).mData[0]); // X��ǥ
		mPositions[i].y = static_cast<float>(InMesh->GetControlPointAt(i).mData[1]); // Y��ǥ
		mPositions[i].z = static_cast<float>(InMesh->GetControlPointAt(i).mData[2]); // Z��ǥ
	}
}

DirectX::XMVECTOR FBXParser::ReadNormal(const FbxMesh* InMesh, int ControlPointIndex, int VertexCounter)
{
	// �븻�� ����.
	const FbxGeometryElementNormal* VertexNormal = InMesh->GetElementNormal(0);
	if (VertexNormal == nullptr)
	{
		return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// �븻 ���͸� ������ ����
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

// ȹ���� FBX������ DirectX ���α׷����� �ʿ��� ������ ��ȯ�ϴ� �Լ�.
// ���ο� ����ϴ� ������ �������� ����� �� �κ��� �����ϸ� ��.
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
	// ��ġ ���� �迭�� ����.
	if (mPositions != nullptr)
	{
		delete[] mPositions;
		mPositions = nullptr;
	}

	// �޽� ��ü�� ����.
	if (mFbxMesh != nullptr)
	{
		mFbxMesh->Destroy();
		mFbxMesh = nullptr;
	}

	// ��� ��ü�� ����.
	if (mFbxNode != nullptr)
	{
		mFbxNode->Destroy();
		mFbxNode = nullptr;
	}

	// �� ��ü ����.
	if (mFbxScene != nullptr)
	{
		mFbxScene->Destroy();
		mFbxScene = nullptr;
	}

	// ������ ��ü ����.
	if (mFbxScene != nullptr)
	{
		mFbxManager->Destroy();
	}
}