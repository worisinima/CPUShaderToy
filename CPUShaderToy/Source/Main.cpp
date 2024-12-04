
#include "Common.h"
#include "Rect.h"
#include "QuadTree.h"

class FItem;

class FCell
{
public:
	FCell()
	{
		P = FVector2D(0.0f, 0.0f);
		ItemRef = nullptr;
	}
	FCell(FVector2D p)
	{
		P = p;
		ItemRef = nullptr;
	}
	FItem* ItemRef;
	FVector2D P;
};

class FItem
{
public:

	FItem()
	{
		P = FVector2D(0.0f, 0.0f);
		XY = FVector2D(1, 1);
		Color = FColor(255, 255, 255);
		ItemCellMask = { 1 };
	}
	FItem(FVector2D p, FVector2D xy, FColor color, const vector<unsigned int>& mask)
	{
		P = p;
		XY = xy;
		Color = color;
		ItemCellMask = mask;
	}

	FVector2D Min() { return P; }
	FVector2D Max() { return P + XY; }
	FVector2D GetCenter() { return P + XY / 2.0f; }
	FVector2D GetHalfSize() { return XY / 2.0f; }
	void SetPosition(FVector2D newP)
	{
		P = newP;
	}

	FVector2D P;
	FVector2D XY;
	FColor Color;
	vector<unsigned int> ItemCellMask;
	vector<FCell*> ItemOccupyCell;
};

class FContainer
{
public:

	FContainer(int ix, int iy)
	{
		XY = FVector2D(ix, iy);
		for (int y = 0; y < iy; y++)
		{
			for (int x = 0; x < ix; x++)
			{
				FCell* cell = new FCell(FVector2D(x, y));
				ContainerCellPool.push_back(cell);
			}
		}
	}
	
	void AddItem(FItem* NewItem)
	{
		for (int y = 0; y < XY.Y; y++)
		{
			for (int x = 0; x < XY.Y; x++)
			{
				NewItem->SetPosition(FVector2D(x, y));
				if (CanAdd(NewItem))
				{
					//添加物体
					ItemPool.push_back(NewItem);
					//更新背包格子掩码
					for (int iy = 0; iy < NewItem->XY.Y; iy++)
					{
						for (int ix = 0; ix < NewItem->XY.X; ix++)
						{
							int index = NewItem->P.X + ix + (NewItem->P.Y + iy) * XY.X;
							FCell* cell = ContainerCellPool[index];

							if (NewItem->ItemCellMask[ix + iy * NewItem->XY.X] == 0)
							{
								cell->ItemRef = nullptr;
							}
							else
							{
								NewItem->ItemOccupyCell.push_back(cell);
								cell->ItemRef = NewItem;
							}
							
						}
					}
					//添加成功
					return;
				}
			}
		}
	}

	void RemoveItem(FItem* ItemToBeRemove)
	{
		bool bFind = false;
		for (int i = 0; i < ItemPool.size(); i++)
		{
			if (ItemPool[i] == ItemToBeRemove)
			{
				bFind = true;
				break;
			}
		}

		if (bFind)
		{
			ItemPool.erase(find(ItemPool.begin(), ItemPool.end(), ItemToBeRemove));
			for (int i = 0; i < ItemToBeRemove->ItemOccupyCell.size(); i++)
			{
				ItemToBeRemove->ItemOccupyCell[i]->ItemRef = nullptr;
			}
		}
	}

	bool CanAdd(FItem* NewItem)
	{
		if (InBoundary(NewItem) == true)
		{
			for (int iy = 0; iy < NewItem->XY.Y; iy++)
			{
				for (int ix = 0; ix < NewItem->XY.X; ix++)
				{
					int index = NewItem->P.X + ix + (NewItem->P.Y + iy) * XY.X;
					FCell* cell = ContainerCellPool[index];
					if (cell->ItemRef != nullptr)
					{
						if (NewItem->ItemCellMask[ix + iy * NewItem->XY.X] != 0)
						{
							return false;
						}
					}
				}
			}
			return true;
		}
	}

	bool InBoundary(FItem* NewItem)
	{
		return NewItem->Min().X >= 0 && NewItem->Min().Y >= 0 && NewItem->Max().X <= XY.X && NewItem->Max().Y <= XY.Y;
	}
	
	void Rasterization(vector<FColor>& OutData)
	{
		OutData.clear();
		Rect2D* A = new Rect2D(FVector2D(0, 0), FVector2D(1, 1), FColor(0, 0, 0));

		for (int y = 0; y < XY.Y; y++)
		{
			for (int x = 0; x < XY.Y; x++)
			{
				FColor color = FColor(0);
				if (ContainerCellPool[x + y * XY.X]->ItemRef != nullptr)
				{
					color = ContainerCellPool[x + y * XY.X]->ItemRef->Color;
				}
				OutData.push_back(color);
			}
		}
	}

	FVector2D XY;
	vector<FCell*> ContainerCellPool;
	vector<FItem*> ItemPool;
};

int main()
{
	srand((int)time(NULL));
	//srand(0);

	FContainer* Container = new FContainer(32, 32);
	FItem* item0 = nullptr;
	{
		vector<unsigned int> mask = {
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1
		};
		item0 = new FItem(FVector2D(0, 0), FVector2D(5, 3), FColor(255, 0, 255), mask);
		Container->AddItem(item0);
	}

	for (int i = 0; i < 8; i++)
	{
		int sx = RandInRange(2, 10);
		int sy = RandInRange(2, 10);
		int cr = RandInRange(0, 255);
		int cg = RandInRange(0, 255);
		int cb = RandInRange(0, 255);
		vector<unsigned int>mask;
		for (int y = 0; y < sy; y++)
		{
			for (int x = 0; x < sx; x++)
			{
				mask.push_back(1);
}
		}
		FItem* nr = new FItem(FVector2D(0, 0), FVector2D(sx, sy), FColor(cr, cg, cb), mask);
		Container->AddItem(nr);
	}

	FItem* item1 = nullptr;
	{
		vector<unsigned int> mask = {1};
		item1 = new FItem(FVector2D(0, 0), FVector2D(1, 1), FColor(255, 255, 255), mask);
		Container->AddItem(item1);
	}
	
	vector<FColor> RasterizaContainer;
	CImg<uint8_t> DestSaveTexture(Container->XY.X, Container->XY.Y, 1, 3);

	Container->Rasterization(RasterizaContainer);
	//Save the texture
	cimg_forXY(DestSaveTexture, x, y)
	{
		FColor NewColor;
		DestSaveTexture(x, y, 0) = RasterizaContainer[y * Container->XY.Y + x].R;
		DestSaveTexture(x, y, 1) = RasterizaContainer[y * Container->XY.Y + x].G;
		DestSaveTexture(x, y, 2) = RasterizaContainer[y * Container->XY.Y + x].B;
	}
	DestSaveTexture.display("Output0");

	Container->RemoveItem(item0);

	Container->Rasterization(RasterizaContainer);
	//Save the texture
	cimg_forXY(DestSaveTexture, x, y)
	{
		FColor NewColor;
		DestSaveTexture(x, y, 0) = RasterizaContainer[y * Container->XY.Y + x].R;
		DestSaveTexture(x, y, 1) = RasterizaContainer[y * Container->XY.Y + x].G;
		DestSaveTexture(x, y, 2) = RasterizaContainer[y * Container->XY.Y + x].B;
	}
	DestSaveTexture.display("Output1");

	Container->RemoveItem(item1);

	Container->Rasterization(RasterizaContainer);
	//Save the texture
	cimg_forXY(DestSaveTexture, x, y)
	{
		FColor NewColor;
		DestSaveTexture(x, y, 0) = RasterizaContainer[y * Container->XY.Y + x].R;
		DestSaveTexture(x, y, 1) = RasterizaContainer[y * Container->XY.Y + x].G;
		DestSaveTexture(x, y, 2) = RasterizaContainer[y * Container->XY.Y + x].B;
	}
	DestSaveTexture.display("Output2");

	Container->AddItem(item0);

	Container->Rasterization(RasterizaContainer);
	//Save the texture
	cimg_forXY(DestSaveTexture, x, y)
	{
		FColor NewColor;
		DestSaveTexture(x, y, 0) = RasterizaContainer[y * Container->XY.Y + x].R;
		DestSaveTexture(x, y, 1) = RasterizaContainer[y * Container->XY.Y + x].G;
		DestSaveTexture(x, y, 2) = RasterizaContainer[y * Container->XY.Y + x].B;
	}
	DestSaveTexture.display("Output3");

	return 0;
}