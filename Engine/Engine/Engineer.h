#ifndef _ENGINEERCLASS_H_
#define _ENGINEERCLASS_H_
#include "building.h"
#include "unit.h"
#include "Supplies.h"

class EngineerClass : public UnitClass
{
private:
	Supply_stack*** supplieslist;
	int* n_supplies;
	UINT Engineer_status;
	InvObject** supply;
	Supply_stack* supply_closest;
	D2D1_POINT_2F building_point;

	enum
	{
		IDLE = 0,
		GOING_FOR_SUPPLIES = 1,
		GOING_TO_BUILD = 2
	};

public:
	void CalcPosition(int timeDiff);
	EngineerClass();
	void RepairTarget(Building * input, D2D1_POINT_2F place);
	void SetSuppliesList(Supply_stack*** supp, int* nsupply);
};

#endif