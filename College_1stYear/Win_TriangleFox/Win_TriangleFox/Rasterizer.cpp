#include "Rasterizer.h"
#include "Triangle.h"
#include "GDIManager.h"

Rasterizer::Rasterizer()
{
	
}

Rasterizer::~Rasterizer()
{

}

void Rasterizer::Initialize(GDIManager* InGDIManager)
{
	WinGDIManager = InGDIManager;

	TriangleListCapacity = 24;
}

void Rasterizer::Release()
{
	// Pass
}

void Rasterizer::SetTriangleList()
{
	TriangleList = new Triangle[TriangleListCapacity];
	if (TriangleList == nullptr) return;

	// 포인트의 순서는 Y값 크기대로.

	// 1
	TriangleList[0].Point1.Position = TMath::Vector2Set(244.0f,54.0f);
	TriangleList[0].Point1.VertexColor = TMath::ColorSet(125, 34, 07);
	TriangleList[0].Point2.Position = TMath::Vector2Set(303.0f, 101.0f);
	TriangleList[0].Point2.VertexColor = TMath::ColorSet(145, 54, 27);
	TriangleList[0].Point3.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[0].Point3.VertexColor = TMath::ColorSet(165, 74, 47);
	
	// 2
	TriangleList[1].Point1.Position = TMath::Vector2Set(244.0f, 54.0f);
	TriangleList[1].Point1.VertexColor = TMath::ColorSet(208, 19, 0);
	TriangleList[1].Point2.Position = TMath::Vector2Set(251.0f, 115.0f);
	TriangleList[1].Point2.VertexColor = TMath::ColorSet(228, 39, 17);
	TriangleList[1].Point3.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[1].Point3.VertexColor = TMath::ColorSet(248, 59, 37);

	// 3
	TriangleList[2].Point1.Position = TMath::Vector2Set(251.0f, 115.0f);
	TriangleList[2].Point1.VertexColor = TMath::ColorSet(235, 63, 27);
	TriangleList[2].Point2.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[2].Point2.VertexColor = TMath::ColorSet(255, 83, 47);
	TriangleList[2].Point3.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[2].Point3.VertexColor = TMath::ColorSet(255, 103, 67);

	// 4
	TriangleList[3].Point1.Position = TMath::Vector2Set(251.0f, 115.0f);
	TriangleList[3].Point1.VertexColor = TMath::ColorSet(235, 68, 39);
	TriangleList[3].Point2.Position = TMath::Vector2Set(228.0f, 137.0f);
	TriangleList[3].Point2.VertexColor = TMath::ColorSet(255, 88, 59);
	TriangleList[3].Point3.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[3].Point3.VertexColor = TMath::ColorSet(255, 108, 79);

	// 5
	TriangleList[4].Point1.Position = TMath::Vector2Set(228.0f, 137.0f);
	TriangleList[4].Point1.VertexColor = TMath::ColorSet(235, 63, 27);
	TriangleList[4].Point2.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[4].Point2.VertexColor = TMath::ColorSet(255, 83, 47);
	TriangleList[4].Point3.Position = TMath::Vector2Set(200.0f, 192.0f);
	TriangleList[4].Point3.VertexColor = TMath::ColorSet(255, 103, 67);

	// 6
	TriangleList[5].Point1.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[5].Point1.VertexColor = TMath::ColorSet(235, 113, 98);
	TriangleList[5].Point2.Position = TMath::Vector2Set(200.0f, 192.0f);
	TriangleList[5].Point2.VertexColor = TMath::ColorSet(255, 133, 118);
	TriangleList[5].Point3.Position = TMath::Vector2Set(160.0f, 234.0f);
	TriangleList[5].Point3.VertexColor = TMath::ColorSet(255, 143, 138);

	// 7
	TriangleList[6].Point1.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[6].Point1.VertexColor = TMath::ColorSet(223, 54, 15);
	TriangleList[6].Point2.Position = TMath::Vector2Set(160.0f, 234.0f);
	TriangleList[6].Point2.VertexColor = TMath::ColorSet(243, 74, 35);
	TriangleList[6].Point3.Position = TMath::Vector2Set(190.0f, 236.0f);
	TriangleList[6].Point3.VertexColor = TMath::ColorSet(255, 94, 55);

	// 8
	TriangleList[7].Point1.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[7].Point1.VertexColor = TMath::ColorSet(224, 50, 15);
	TriangleList[7].Point2.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[7].Point2.VertexColor = TMath::ColorSet(234, 70, 35);
	TriangleList[7].Point3.Position = TMath::Vector2Set(190.0f, 236.0f);
	TriangleList[7].Point3.VertexColor = TMath::ColorSet(254, 90, 55);

	// 9
	TriangleList[8].Point1.Position = TMath::Vector2Set(160.0f, 234.0f);
	TriangleList[8].Point1.VertexColor = TMath::ColorSet(228, 67, 25);
	TriangleList[8].Point2.Position = TMath::Vector2Set(190.0f, 236.0f);
	TriangleList[8].Point2.VertexColor = TMath::ColorSet(248, 87, 45);
	TriangleList[8].Point3.Position = TMath::Vector2Set(164.0f, 261.0f);
	TriangleList[8].Point3.VertexColor = TMath::ColorSet(255, 107, 65);

	// 10
	TriangleList[9].Point1.Position = TMath::Vector2Set(160.0f, 234.0f);
	TriangleList[9].Point1.VertexColor = TMath::ColorSet(33, 34, 38);
	TriangleList[9].Point2.Position = TMath::Vector2Set(148.0f, 247.0f);
	TriangleList[9].Point2.VertexColor = TMath::ColorSet(53, 54, 58);
	TriangleList[9].Point3.Position = TMath::Vector2Set(164.0f, 261.0f);
	TriangleList[9].Point3.VertexColor = TMath::ColorSet(73, 74, 78);

	// 11
	TriangleList[10].Point1.Position = TMath::Vector2Set(190.0f, 236.0f);
	TriangleList[10].Point1.VertexColor = TMath::ColorSet(234, 169, 139);
	TriangleList[10].Point2.Position = TMath::Vector2Set(272.0f, 253.0f);
	TriangleList[10].Point2.VertexColor = TMath::ColorSet(254, 189, 159);
	TriangleList[10].Point3.Position = TMath::Vector2Set(164.0f, 261.0f);
	TriangleList[10].Point3.VertexColor = TMath::ColorSet(255, 209, 179);

	// 12
	TriangleList[11].Point1.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[11].Point1.VertexColor = TMath::ColorSet(234, 155, 124);
	TriangleList[11].Point2.Position = TMath::Vector2Set(190.0f, 236.0f);
	TriangleList[11].Point2.VertexColor = TMath::ColorSet(254, 175, 144);
	TriangleList[11].Point3.Position = TMath::Vector2Set(318.0f, 267.0f);
	TriangleList[11].Point3.VertexColor = TMath::ColorSet(255, 195, 164);

	// 13
	TriangleList[12].Point1.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[12].Point1.VertexColor = TMath::ColorSet(235, 147, 112);
	TriangleList[12].Point2.Position = TMath::Vector2Set(272.0f, 253.0f);
	TriangleList[12].Point2.VertexColor = TMath::ColorSet(255, 167, 132);
	TriangleList[12].Point3.Position = TMath::Vector2Set(317.0f, 267.0f);
	TriangleList[12].Point3.VertexColor = TMath::ColorSet(255, 187, 152);

	// 14
	TriangleList[13].Point1.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[13].Point1.VertexColor = TMath::ColorSet(233, 152, 125);
	TriangleList[13].Point2.Position = TMath::Vector2Set(310.0f, 226.0f);
	TriangleList[13].Point2.VertexColor = TMath::ColorSet(253, 172, 145);
	TriangleList[13].Point3.Position = TMath::Vector2Set(318.0f, 267.0f);
	TriangleList[13].Point3.VertexColor = TMath::ColorSet(255, 192, 165);

	// 15
	TriangleList[14].Point1.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[14].Point1.VertexColor = TMath::ColorSet(211, 41, 8);
	TriangleList[14].Point2.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[14].Point2.VertexColor = TMath::ColorSet(231, 61, 28);
	TriangleList[14].Point3.Position = TMath::Vector2Set(310.0f, 226.0f);
	TriangleList[14].Point3.VertexColor = TMath::ColorSet(251, 81, 48);

	// 16
	TriangleList[15].Point1.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[15].Point1.VertexColor = TMath::ColorSet(223, 53, 17);
	TriangleList[15].Point2.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[15].Point2.VertexColor = TMath::ColorSet(243, 73, 37);
	TriangleList[15].Point3.Position = TMath::Vector2Set(253.0f, 226.0f);
	TriangleList[15].Point3.VertexColor = TMath::ColorSet(255, 93, 57);

	// 17
	TriangleList[16].Point1.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[16].Point1.VertexColor = TMath::ColorSet(224, 50, 13);
	TriangleList[16].Point2.Position = TMath::Vector2Set(228.0f, 181.0f);
	TriangleList[16].Point2.VertexColor = TMath::ColorSet(244, 70, 33);
	TriangleList[16].Point3.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[16].Point3.VertexColor = TMath::ColorSet(255, 90, 53);

	// 18
	TriangleList[17].Point1.Position = TMath::Vector2Set(303.0f, 101.0f);
	TriangleList[17].Point1.VertexColor = TMath::ColorSet(174, 28, 5);
	TriangleList[17].Point2.Position = TMath::Vector2Set(348.0f, 116.0f);
	TriangleList[17].Point2.VertexColor = TMath::ColorSet(194, 48, 25);
	TriangleList[17].Point3.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[17].Point3.VertexColor = TMath::ColorSet(214, 68, 45);

	// 19
	TriangleList[18].Point1.Position = TMath::Vector2Set(348.0f, 116.0f);
	TriangleList[18].Point1.VertexColor = TMath::ColorSet(234, 55, 23);
	TriangleList[18].Point2.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[18].Point2.VertexColor = TMath::ColorSet(254, 75, 43);
	TriangleList[18].Point3.Position = TMath::Vector2Set(408.0f, 143.0f);
	TriangleList[18].Point3.VertexColor = TMath::ColorSet(255, 95, 63);

	// 20
	TriangleList[19].Point1.Position = TMath::Vector2Set(282.0f, 137.0f);
	TriangleList[19].Point1.VertexColor = TMath::ColorSet(219, 31, 4);
	TriangleList[19].Point2.Position = TMath::Vector2Set(408.0f, 143.0f);
	TriangleList[19].Point2.VertexColor = TMath::ColorSet(239, 51, 24);
	TriangleList[19].Point3.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[19].Point3.VertexColor = TMath::ColorSet(255, 71, 44);

	// 21
	TriangleList[20].Point1.Position = TMath::Vector2Set(408.0f, 143.0f);
	TriangleList[20].Point1.VertexColor = TMath::ColorSet(200, 0, 0);
	TriangleList[20].Point2.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[20].Point2.VertexColor = TMath::ColorSet(220, 17, 10);
	TriangleList[20].Point3.Position = TMath::Vector2Set(415.0f, 226.0f);
	TriangleList[20].Point3.VertexColor = TMath::ColorSet(240, 37, 30);

	// 22
	TriangleList[21].Point1.Position = TMath::Vector2Set(304.0f, 183.0f);
	TriangleList[21].Point1.VertexColor = TMath::ColorSet(211, 27, 1);
	TriangleList[21].Point2.Position = TMath::Vector2Set(310.0f, 226.0f);
	TriangleList[21].Point2.VertexColor = TMath::ColorSet(231, 47, 21);
	TriangleList[21].Point3.Position = TMath::Vector2Set(415.0f, 226.0f);
	TriangleList[21].Point3.VertexColor = TMath::ColorSet(251, 67, 41);

	// 23
	TriangleList[22].Point1.Position = TMath::Vector2Set(310.0f, 226.0f);
	TriangleList[22].Point1.VertexColor = TMath::ColorSet(233, 79, 41);
	TriangleList[22].Point2.Position = TMath::Vector2Set(415.0f, 226.0f);
	TriangleList[22].Point2.VertexColor = TMath::ColorSet(253, 99, 61);
	TriangleList[22].Point3.Position = TMath::Vector2Set(374.0f, 259.0f);
	TriangleList[22].Point3.VertexColor = TMath::ColorSet(255, 119, 81);

	// 24
	TriangleList[23].Point1.Position = TMath::Vector2Set(310.0f, 226.0f);
	TriangleList[23].Point1.VertexColor = TMath::ColorSet(235, 130, 108);
	TriangleList[23].Point2.Position = TMath::Vector2Set(374.0f, 259.0f);
	TriangleList[23].Point2.VertexColor = TMath::ColorSet(255, 150, 128);
	TriangleList[23].Point3.Position = TMath::Vector2Set(318.0f, 267.0f);
	TriangleList[23].Point3.VertexColor = TMath::ColorSet(255, 170, 148);
	
}

void Rasterizer::DrawTriangleList()
{
	for (TriangleListIndex = 0; TriangleListIndex < TriangleListCapacity; ++TriangleListIndex)
	{
		TriangleRasterize(TriangleList[TriangleListIndex]);
	}
}

void Rasterizer::TriangleRasterize(struct Triangle& InTriangle)
{
	SortVertexByY(&InTriangle);

	InTriangle.Initialize();

	if (InTriangle.Point2.Position.y == InTriangle.Point3.Position.y)
	{
		BottomTriangleRasterize(InTriangle.Point1, InTriangle.Point2, InTriangle.Point3);
	}
	else if (InTriangle.Point1.Position.y == InTriangle.Point2.Position.y)
	{
		TopTriangleRasterize(InTriangle.Point1, InTriangle.Point2, InTriangle.Point3);
	}
	else
	{
		Vertex NewVertex;
		NewVertex.Position.y = InTriangle.Point2.Position.y;
		NewVertex.Position.x = (InTriangle.Point1.Position.x + ((InTriangle.Point2.Position.y - InTriangle.Point1.Position.y) /
			(InTriangle.Point3.Position.y - InTriangle.Point1.Position.y)) * (InTriangle.Point3.Position.x - InTriangle.Point1.Position.x));
		BottomTriangleRasterize(InTriangle.Point1, InTriangle.Point2, NewVertex);
		TopTriangleRasterize(InTriangle.Point2, NewVertex, InTriangle.Point3);
	}
}

void Rasterizer::TopTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3)
{
	if (InVertex1.Position.y != InVertex2.Position.y)
	{
		return;
	}

	float inclinationL = (InVertex3.Position.x - InVertex1.Position.x) / (InVertex3.Position.y - InVertex1.Position.y);
	float inclinationR = (InVertex3.Position.x - InVertex2.Position.x) / (InVertex3.Position.y - InVertex2.Position.y);

	float StartPosX = InVertex3.Position.x;
	float EndPosX = InVertex3.Position.x;

	for (int ScanLineY = (int)InVertex3.Position.y; ScanLineY <= InVertex1.Position.y; ++ScanLineY)
	{
		Vertex TempVertex1;
		Vertex TempVertex2;

		TempVertex1.Position = TMath::Vector2Set(StartPosX, ScanLineY);
		TempVertex2.Position = TMath::Vector2Set(EndPosX, ScanLineY);

		DrawFlatLine(TempVertex1, TempVertex2);

		StartPosX += inclinationL;
		EndPosX += inclinationR;
	}
}

void Rasterizer::BottomTriangleRasterize(struct Vertex& InVertex1, struct Vertex& InVertex2, struct Vertex& InVertex3)
{
	if (InVertex2.Position.y != InVertex3.Position.y)
	{
		return;
	}

	float inclinationL = (InVertex2.Position.x - InVertex1.Position.x) / (InVertex2.Position.y - InVertex1.Position.y);
	float inclinationR = (InVertex3.Position.x - InVertex1.Position.x) / (InVertex3.Position.y - InVertex1.Position.y);

	float StartPosX = InVertex1.Position.x;
	float EndPosX = InVertex1.Position.x;

	for (int ScanLineY = (int)InVertex1.Position.y; ScanLineY >= InVertex2.Position.y; --ScanLineY)
	{
		Vertex TempVertex1;
		Vertex TempVertex2;

		TempVertex1.Position = TMath::Vector2Set(StartPosX, ScanLineY);
		TempVertex2.Position = TMath::Vector2Set(EndPosX, ScanLineY);

		DrawFlatLine(TempVertex1, TempVertex2);

		StartPosX -= inclinationL;
		EndPosX -= inclinationR;
	}
}

void Rasterizer::DrawFlatLine(struct Vertex& StartVertex, struct Vertex& EndVertex)
{
	if (StartVertex.Position.y != StartVertex.Position.y)
	{
		return;
	}

	for (int i = (int)min(StartVertex.Position.x, EndVertex.Position.x); i <= max(StartVertex.Position.x, EndVertex.Position.x); ++i)
	{
		Vector2 CurrentPoint = Vector2(i, StartVertex.Position.y);

		Vector3 VertexWeight = TriangleList[TriangleListIndex].GetVertexWeight(CurrentPoint);
		ColorRGB CurrentColor =
			TriangleList[TriangleListIndex].Point1.VertexColor * VertexWeight.x +
			TriangleList[TriangleListIndex].Point2.VertexColor * VertexWeight.y +
			TriangleList[TriangleListIndex].Point3.VertexColor * VertexWeight.z;

		WinGDIManager->SetColor(CurrentColor);

		WinGDIManager->PutPixel((int)CurrentPoint.x, (int)CurrentPoint.y);
	}
}

void Rasterizer::SortVertexByY(Triangle* InTriangle)
{
	if (!((InTriangle->Point1.Position.y > InTriangle->Point2.Position.y) && (InTriangle->Point1.Position.y > InTriangle->Point3.Position.y)))
	{
		if (InTriangle->Point2.Position.y > InTriangle->Point3.Position.y)
		{
			std::swap(InTriangle->Point1, InTriangle->Point2);
		}
		else
		{
			std::swap(InTriangle->Point1, InTriangle->Point3);
		}
	}

	if (InTriangle->Point2.Position.y < InTriangle->Point3.Position.y)
	{
		std::swap(InTriangle->Point2, InTriangle->Point3);
	}
}
