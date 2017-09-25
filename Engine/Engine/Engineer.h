#ifndef _ENGINEERCLASS_H_
#define _ENGINEERCLASS_H_
#include "building.h"
#include "unit.h"

class EngineerClass : public UnitClass
{
public:
	EngineerClass();
	void RepairTarget(Building * input, D2D1_POINT_2F place);
};

#endif