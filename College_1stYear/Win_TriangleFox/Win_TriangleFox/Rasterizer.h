#pragma once

#include "CommonHeader.h"

class Rasterizer
{
public:

	Rasterizer();
	~Rasterizer();

	void Initialize(class GDIManager* InGDIManager);
	void Release();

	void SetTriangleList();
	void DrawTriangleList();
	void TriangleRasterize(struct Triangle& InTriangle);
	void TopTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);
	void BottomTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3);
	void DrawFlatLine(struct Vertex& StartVertex, struct Vertex& EndVertex);

	void SortVertexByY(struct Triangle* InTriangle);

private:

	struct Triangle* TriangleList;
	class GDIManager* WinGDIManager;
	int TriangleListCapacity;
	int TriangleListIndex;

};
