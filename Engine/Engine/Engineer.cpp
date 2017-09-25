#include "Engineer.h"

EngineerClass::EngineerClass()
{
	UnitType == OBJTYPES::ENGINEERS;
}

void EngineerClass::RepairTarget(Building * input, D2D1_POINT_2F place)
{
	SetTarget(place);
	Mode.OBJTarget = input;
	Mode.UnitMode = Mode.MODE_REPARING;
}
