#pragma once

#include "CommonHeader.h"

class Rasterizer
{
public:

	Rasterizer();
	~Rasterizer();

	void Initialize(class GDIManager* InGDIManager);
	void Release();


	// �ﰢ���� TriangleList������ ����ϴ� �Լ�.
	void SetTriangleList();

	// TriangleList�� �׸��� �Լ�.
	void DrawTriangleList();

	// 1���� Triangle�� Rasterize�� �ϴ� �Լ�.
	void TriangleRasterize(struct Triangle& InTriangle);

	// ���� ���ؽ��� Y���� ������ Triangle�� Rasterize�� �ϴ� �Լ�.
	void TopTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);

	// �Ʒ��� ���ؽ��� Y���� ������ Triangle�� Rasterize�� �ϴ� �Լ�.
	void BottomTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);

	// Rasterize�������� Scanline������� ���� �׸��� �Լ�.
	void DrawFlatLine(struct Vertex& StartVertex, struct Vertex& EndVertex);

	// Rasterize �� �ܰ迡�� ���ؽ��� Y���� ���� �����ϴ� �Լ�.
	void SortVertexByY(struct Triangle* InTriangle);

private:

	struct Triangle* TriangleList;
	class GDIManager* WinGDIManager;
	int TriangleListCapacity;
	int TriangleListIndex;

};
