#ifndef  _INVOBJECTCLASS_H_
#define _INVOBJECTCLASS_H_
#include <d2d1.h>

#define SUPPLY_WEIGHT 5
#define CONCRETE_WEIGHT 5
#define WEAPON_WEIGHT 15

//extern class UINT;

struct InvObject
{
	enum
	{
		WEAPON = 0,
		SUPPLY = 1,
		CONCRETE = 2
	};

	int type;
	int weight;
	int ammount;
};

struct Weapon: public InvObject
{
	enum
	{
		WEIGHT = WEAPON_WEIGHT,
	};
	enum gun
	{
		M4 = 0
	};
	int shots_in_one_salve;
	int delay_between_salves;
	int delay_between_shoots;
	int rounds_in_magazine;

	Weapon(UINT Type);
};

struct Supply : public InvObject
{
	enum
	{
		WEIGHT = SUPPLY_WEIGHT
	};
	Supply();
};

struct Concrete : public InvObject
{
	enum
	{
		WEIGHT = CONCRETE_WEIGHT
	};
	Concrete();
};

#endif