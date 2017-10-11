#include "InventoryObjects.h"

Weapon::Weapon(UINT Type)
{
	g_type = weapon;

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


Resource::Resource(UINT Type)
{
	g_type = Type;
}

UINT InvObject::type()
{
	return g_type;
}

int InvObject::ammount()
{
	return g_ammount;
}

int InvObject::transfer_to(InvObject * obj, int amm)
{
	if (obj == 0)
		return -1;
	if (g_type != obj->g_type)
		return -1;

	if (g_ammount < amm)
		amm = g_ammount;

	change_ammount(-amm);
	obj->change_ammount(amm);
	return amm;
}

double InvObject::weight()
{
	switch (g_type)
	{

	case TYPE::supply:
	{
		return g_ammount * INVOBJ_SUPPLY_WEIGHT;
	}

	case TYPE::concrete:
	{
		return g_ammount * INVOBJ_CONCRETE_WEIGHT;
	}

	case TYPE::weapon:
	{
		return g_ammount * INVOBJ_WEAPON_WEIGHT;
	}
	default:
	{
		return 0;
	}
	}
}

InvObject::InvObject()
{
	g_ammount = 0;
}

void InvObject::change_ammount(int i)
{
	g_ammount += i;
}
