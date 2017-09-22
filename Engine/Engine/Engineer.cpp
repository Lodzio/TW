#include "Engineer.h"

EngineerClass::EngineerClass()
{
	UnitType == OBJTYPES::ENGINEERS;
}

void EngineerClass::RepairTarget(Building * input)
{
	Target = input->GetPosition();
	Mode.OBJTarget = input;
	Mode.UnitMode = Mode.MODE_REPARING;
}
