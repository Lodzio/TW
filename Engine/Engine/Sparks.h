#ifndef _SPARKSCLASS_H_
#define _SPARKSCLASS_H_

#include <d2d1.h>
#include <D2d1_1.h>
#include <d2d1effects.h>
#include <d2d1helper.h>
using namespace D2D1;

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxguid.lib")

struct Spark
{
	D2D1_POINT_2F Position;
	int Rotation;
	double time;
};

class SparksClass
{
public:
	enum
	{
		ShootFire = 1,
		Blood = 2
	};
	void UpdateTime(double input);
	void initialize(UINT typein);	
	void Render(ID2D1DeviceContext* Target, ID2D1Effect* Effect);
	SparksClass operator+=(Spark input);
	SparksClass operator-=(Spark input);
private:
	UINT Type;
	int Ammount;
	Spark* MainSparksArray;
};

#endif // !_SPARKSCLASS_H_