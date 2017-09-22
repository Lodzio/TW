#ifndef  _QUADTREECLASS_H_
#define _QUADTREECLASS_H_
#include "object.h"
#include "Soldier.h"
using namespace D2D1;

struct AreaData
{
	Object** ObjectsList;
	int Number;
	D2D1_RECT_U Area;
};

struct Neighbours
{
public:
	AreaData* NeighboursData;
	int numbers;
};

class QuadTreeClass
{
private:

	enum Side
	{
		All = 0,
		North = 1,
		South = 2,
		West = 3,
		East = 4,
		NorthEast = 5,
		NorthWest = 6,
		SouthEast = 7,
		SouthWest = 8
	};

	class SubArea
	{
	public:
		void Initialize(D2D1_SIZE_U arealimit, D2D1_RECT_U size, UINT capacity);
		void PutInObject(Object* input);
		
		Neighbours GetList(UINT Side = 0);
		AreaData GetListToColide(Neighbours input);
		UINT GetAmmount();
		void Reset();
		void Draw(ID2D1RenderTarget* RenderTarget, ID2D1SolidColorBrush* Brush);
		Object* GetObjectByCoords(D2D1_POINT_2F input);

	private:
		D2D1_SIZE_U AreaLimit;
		D2D1_RECT_U Area;
		bool IsDivided;
		Object** List;
		SubArea* NorthEast;
		SubArea* NorthWest;
		SubArea* SouthEast;
		SubArea* SouthWest;
		UINT Capacity;
		UINT Ammount;

	};


private:
	const static UINT ammount_of_areas = 5;
	D2D1_SIZE_U MapSize;
	SubArea Areas[ammount_of_areas][ammount_of_areas];
public:
	void CalculateViewArea();
	AreaData GetListToColide();
	void Draw(ID2D1RenderTarget* RenderTarget, ID2D1SolidColorBrush* Brush);
	Object* GetObjectByCoords(D2D1_POINT_2F input);
	void Initialize(D2D1_SIZE_U mapsize);
	void PutInObject(Object* input);
	void Reset();
};

#endif 