#ifndef _SUPPLY_STACK_CLASS_H_
#define _SUPPLY_STACK_CLASS_H_
#include "object.h"

class Supply_stack : public Object
{
private:
	int Supplies;
public:
	int Get_Suplies(int in);
	void init_supply(int supp);
};

#endif