
#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include<random>
#include<stdlib.h>
#include<time.h>
#include <cmath>

#include <windows.h>
#include <io.h>
#include <wrl.h>
#include <array>
#include <direct.h>
#include "shlobj.h" 

#include "Common.h"

class Rect2D
{
public:
	Rect2D()
	{
		P = FVector2D(0.0f, 0.0f);
		XY = FVector2D(1.0f, 1.0f);
		Color = FColor(255, 0, 0);
	}
	Rect2D(FVector2D p, FVector2D xy, FColor color)
	{
		P = p;
		XY = xy;
		Color = color;
	}

	FVector2D Min() { return P; }
	FVector2D Max() { return P + XY; }
	FVector2D GetCenter() { return P + XY / 2.0f; }
	FVector2D GetHalfSize() { return XY / 2.0f; }
	void SetPosition(FVector2D newP)
	{
		P = newP;
	}

	bool CanContain(Rect2D* IR)
	{
		const FVector2D& InnerMin = IR->Min();
		const FVector2D& InnerMax = IR->Max();
		const FVector2D& SelfMin = Min();
		const FVector2D& SelfMax = Max();
		return InnerMin.X >= SelfMin.X && InnerMin.Y >= SelfMin.Y && InnerMax.X <= SelfMax.X && InnerMax.Y <= SelfMax.Y;
	}

	FVector2D P;
	FVector2D XY;
	FColor Color;
};

bool RectIntersect(Rect2D* A, Rect2D* B)
{
	if (A->Min().X > B->Max().X) return false;
	if (A->Min().Y > B->Max().Y) return false;
	if (A->Max().X < B->Min().X) return false;
	if (A->Max().Y < B->Min().Y) return false;

	return true;
}

//两个AABB包围盒a 和 b 相交的充分必要条件为 二者的中心位置的距离小于两个的 hafl_size 的和。
bool AABBIntersectionTest(Rect2D* A, Rect2D* B)
{
	FVector2D delta = B->GetCenter() - A->GetCenter();
	FVector2D totalSize = A->GetHalfSize() + B->GetHalfSize();
	if (abs(delta.X) < totalSize.X && abs(delta.Y) < totalSize.Y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

class FContainerRect
{
public:
	FContainerRect()
	{
		ContainerRect.P = FVector2D(0.0f, 0.0f);
		ContainerRect.XY = FVector2D(64, 64);
		RectPool.clear();
	}

	void AddRect(Rect2D* NewRect)
	{
		for (int y = 0; y < ContainerRect.XY.Y; y++)
		{
			for (int x = 0; x < ContainerRect.XY.Y; x++)
			{
				FVector2D AddPosition = FVector2D(x, y);
				NewRect->SetPosition(AddPosition);
				if (RectPool.size() == 0 && ContainerRect.CanContain(NewRect) == true)
				{
					RectPool.push_back(NewRect);
					return;
				}
				else
				{
					bool bAdd = true;
					for (int i = 0; i < RectPool.size(); i++)
					{
						Rect2D* B = RectPool[i];
						if (AABBIntersectionTest(NewRect, B) == true)
						{
							bAdd = false;
							break;
						}
					}

					if (bAdd == true && ContainerRect.CanContain(NewRect) == true)
					{
						RectPool.push_back(NewRect);
						return;
					}
				}
			}
		}
	}

	void Rasterization(vector<FColor>& OutData)
	{
		OutData.clear();
		Rect2D* A = new Rect2D(FVector2D(0, 0), FVector2D(1, 1), FColor(0, 0, 0));

		for (int y = 0; y < ContainerRect.XY.Y; y++)
		{
			for (int x = 0; x < ContainerRect.XY.Y; x++)
			{
				A->SetPosition(FVector2D(x, y));
				FColor Color = FColor(0.0f);
				for (int i = 0; i < RectPool.size(); i++)
				{
					Rect2D* B = RectPool[i];
					if (AABBIntersectionTest(A, B) == true)
					{
						Color = (Color + B->Color) / 2.0f;
					}
				}
				OutData.push_back(Color);
			}
		}
	}

	Rect2D ContainerRect;
	vector<Rect2D*> RectPool;
};

void TestRect()
{
	//srand((int)time(NULL));
	srand(0);

	FContainerRect* RectContainer = new FContainerRect();
	for (int i = 0; i < 128; i++)
	{
		int x = RandInRange(0, RectContainer->ContainerRect.XY.X);
		int y = RandInRange(0, RectContainer->ContainerRect.XY.Y);
		int sx = RandInRange(2, 10);
		int sy = RandInRange(2, 10);
		int cr = RandInRange(0, 255);
		int cg = RandInRange(0, 255);
		int cb = RandInRange(0, 255);
		Rect2D* nr = new Rect2D(FVector2D(x, y), FVector2D(sx, sy), FColor(cr, cg, cb));
		RectContainer->AddRect(nr);
	}

	vector<FColor> RasterizaContainer;
	RectContainer->Rasterization(RasterizaContainer);

	//Save the texture
	CImg<uint8_t> DestSaveTexture(RectContainer->ContainerRect.XY.X, RectContainer->ContainerRect.XY.Y, 1, 3);
	cimg_forXY(DestSaveTexture, x, y)
	{
		FColor NewColor;
		DestSaveTexture(x, y, 0) = RasterizaContainer[y * RectContainer->ContainerRect.XY.Y + x].R;
		DestSaveTexture(x, y, 1) = RasterizaContainer[y * RectContainer->ContainerRect.XY.Y + x].G;
		DestSaveTexture(x, y, 2) = RasterizaContainer[y * RectContainer->ContainerRect.XY.Y + x].B;
	}
	DestSaveTexture.display("Output");
	//const string& Path = FileHelper::GetDesktopPath();
	//DestSaveTexture.save((Path + "\\Text.bmp").c_str());

	//TestQuadTree();
}
