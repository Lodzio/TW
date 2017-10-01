#include "InventoryObjects.h"

Weapon::Weapon(UINT Type)
{
	type = WEAPON;
	weight = WEIGHT;

	switch(Type)
	{
	case gun::M4:
	{
		shots_in_one_salve = 3;
		delay_between_salves = 1900;
		delay_between_shoots = 100;
		rounds_in_magazine = 30;
		break;
	}
	}
}


Supply::Supply()
{
	type = SUPPLY;
	weight = WEIGHT;
}

Concrete::Concrete()
{
	type = CONCRETE;
	weight = WEIGHT;
}
