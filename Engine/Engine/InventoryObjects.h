#ifndef  _INVOBJECTCLASS_H_
#define _INVOBJECTCLASS_H_
#include <d2d1.h>

#define INVOBJ_SUPPLY_WEIGHT 5
#define INVOBJ_CONCRETE_WEIGHT 5
#define INVOBJ_WEAPON_WEIGHT 15

//extern class UINT;

struct InvObject
{
	enum TYPE
	{
		Null = 0,
		Wheat = 1,
		Flour = 2,
		Bread = 3,
		Iron_ore = 4,
		Steel = 5,
		cutlery = 6,
		concrete = 7,
		cement = 8,
		weapon = 9,
		supply = 10
	};
protected:
	UINT g_type;
	int g_ammount;
public:
	UINT type();
	int ammount();
	int transfer_to(InvObject* obj, int amm);
	double weight();
	InvObject();
	void change_ammount(int i);
};

struct Weapon: public InvObject
{
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

struct Resource : public InvObject
{
	Resource(UINT Type);
};

#endif