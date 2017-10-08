#ifndef _SUPPLY_STACK_CLASS_H_
#define _SUPPLY_STACK_CLASS_H_
#include "object.h"
#include "InventoryObjects.h"

class Supply_stack : public Object
{
private:
	Resource* object;
public:
	InvObject* Supply_stack::Get_Suplies();
	void init_supply(int supp);
};

#endif