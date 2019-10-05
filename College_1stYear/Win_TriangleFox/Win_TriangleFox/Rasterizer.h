#pragma once

#include "CommonHeader.h"

class Rasterizer
{
public:

	Rasterizer();
	~Rasterizer();

	void Initialize(class GDIManager* InGDIManager);
	void Release();


	// 삼각형을 TriangleList변수에 등록하는 함수.
	void SetTriangleList();

	// TriangleList를 그리는 함수.
	void DrawTriangleList();

	// 1개의 Triangle의 Rasterize를 하는 함수.
	void TriangleRasterize(struct Triangle& InTriangle);

	// 위쪽 버텍스의 Y값이 동일한 Triangle의 Rasterize를 하는 함수.
	void TopTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);

	// 아래쪽 버텍스의 Y값이 동일한 Triangle의 Rasterize를 하는 함수.
	void BottomTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);

	// Rasterize과정에서 Scanline방식으로 선을 그리는 함수.
	void DrawFlatLine(struct Vertex& StartVertex, struct Vertex& EndVertex);

	// Rasterize 전 단계에서 버텍스를 Y값에 따라 정렬하는 함수.
	void SortVertexByY(struct Triangle* InTriangle);

private:

	struct Triangle* TriangleList;
	class GDIManager* WinGDIManager;
	int TriangleListCapacity;
	int TriangleListIndex;

};
