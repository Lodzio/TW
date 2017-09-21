#ifndef  _OBJECTCLASS_H_
#define  _OBJECTCLASS_H_

#include <d2d1.h>
#include <d2d1helper.h>
#include <math.h>
#include "Sparks.h"
using namespace D2D1;

enum OBJTYPES
{
	BUILDING = 1,
	UNIT = 2,
	COMMANDCENTER = 3,
	SOLDIER = 4,
	ENGINEERS = 5,
	COMMANDER = 6,
	FACTORY = 7,
	EMPLOYEE = 8
};

enum Fractions
{
	FIRSTARMY = 1,
	GUERILLA = 2
};

class Object
{
public:
	bool initialize(int Index, D2D1_POINT_2F pos, int rot, D2D1_SIZE_F siz, D2D1_SIZE_F scal, int health, int maxhealth, D2D1_RECT_U Texture, D2D1_POINT_2F rotaxis, UINT TextureType);
	D2D1_RECT_U GetTexture();
	int GetRotation();
	D2D1_POINT_2F GetPosition();
	D2D1_POINT_2F GetStartCorner();
	UINT GetTextureType();
	virtual void SetPosition(D2D1_POINT_2F Input);
	D2D1_SIZE_F GetSize();
	D2D1_SIZE_F GetScale();
	int GetMaxHealth();
	bool Selected;
	bool IsMovable();
	double Health;
	UINT GetUnitType();
	void SetFractions(UINT input);
	UINT GetFraction();
protected:
	int MaxHealth;
	int index;
	D2D1_POINT_2F StartCorner;
	D2D1_POINT_2F Axis;
	int Rotation;
	D2D1_SIZE_F Size;
	D2D1_SIZE_F Scale;
	D2D1_RECT_U Texcord;
	D2D1_POINT_2F Position;
	UINT TextType;
	bool Movable;
	UINT UnitType;
	UINT Fraction;
};

#endif 