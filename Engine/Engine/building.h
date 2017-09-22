#ifndef _BUILDINGCLASS_H_
#define  _BUILDINGCLASS_H_
#include "object.h"

class Building : public Object
{
public:
	Building();
	void Collide(Object* input);
};
#endif 