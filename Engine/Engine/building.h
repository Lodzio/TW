#ifndef _BUILDINGCLASS_H_
#define  _BUILDINGCLASS_H_
#include "object.h"

class Building : public Object
{
protected:
	int req_supplies;
	int act_supplies;
	int supplies_in_way;
public:
	int build(int amm_of_supplies);
	Building();
	void Changecarring(int i);
	int How_many_supplies_needed();
	void Collide(Object* input);
};
#endif 