#include "Supplies.h"

int Supply_stack::Get_Suplies(int in)
{
	Supplies -= in;
	if (Supplies < 0)
	{
		in -= Supplies;
		Supplies = 0;
	}
	return in;
}

void Supply_stack::init_supply(int supp)
{
	Supplies = supp;
}
