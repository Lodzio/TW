#include "Supplies.h"

InvObject* Supply_stack::Get_Suplies()
{
	return object;
}

void Supply_stack::init_supply(int supp)
{
	object = new Resource(InvObject::supply);
	object->change_ammount(supp);
}
