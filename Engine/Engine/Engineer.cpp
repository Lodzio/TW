#include "Engineer.h"
#include "Pathfinding.h"

void EngineerClass::CalcPosition(int timeDiff)
{
	if (Mode.UnitMode == Mode.MODE_REPARING)
	{
		if (Target.x == -1 && Target.y == -1)
		{
			if (Engineer_status == GOING_FOR_SUPPLIES)
			{
				int needed = ((Building*)Mode.OBJTarget)->How_many_supplies_needed();
				if (needed == 0)
				{
					Mode.UnitMode = Mode.MODE_IDLE;
				}
				else
				{
					int carring = GiveEquipment(supply_closest->Get_Suplies(), needed);
					for (int i = 0; i < 256; i++)
						if (Inventory[i] != 0)
						{
							if (Inventory[i]->type() == supply_closest->Get_Suplies()->type())
							{
								supply = &Inventory[i];
								break;
							}
						}
					((Building*)(Mode.OBJTarget))->Changecarring(carring);
					Engineer_status = GOING_TO_BUILD;
					Path = pathmaker->RequestPath(Position, building_point);
					Target = Path->GetPoint();
				}
			}
			else if (Engineer_status == GOING_TO_BUILD)
			{
				((Building*)Mode.OBJTarget)->build((*supply)->ammount());
				delete *supply;
				*supply = 0;
				if (((Building*)Mode.OBJTarget)->How_many_supplies_needed())
				{
					RepairTarget((Building*)(Mode.OBJTarget), building_point);
				}
				else
				{
					Mode.UnitMode = Mode.MODE_IDLE;
				}
			}
		}
	}
	UnitClass::CalcPosition(timeDiff);
}

EngineerClass::EngineerClass()
{
	supply = 0;
	Engineer_status = IDLE;
	UnitType == OBJTYPES::ENGINEERS;
}

void EngineerClass::RepairTarget(Building * input, D2D1_POINT_2F place)
{
	building_point = place;
	Mode.OBJTarget = input;
	Mode.UnitMode = Mode.MODE_REPARING;
	if (supply)
	{
		if (*supply)
		{
			Engineer_status = GOING_TO_BUILD;
			return;
		}
	}
	Engineer_status = GOING_FOR_SUPPLIES;
	double smallest_range = 100000;
	for (int i = 0; i < *n_supplies; i++)
	{
		pathstruct* path = pathmaker->RequestPath(Position, (*supplieslist)[i]->GetPosition());
		if (smallest_range > path->GetRange())
		{
			smallest_range = path->GetRange();
		}
		delete path;
	}

	for (int i = 0; i < *n_supplies; i++)
	{
		pathstruct* path = pathmaker->RequestPath(Position, (*supplieslist)[i]->GetPosition());
		if (smallest_range == path->GetRange())
		{
			Path = path;
			Target = Path->GetPoint();
			supply_closest = ((*supplieslist)[i]);
			break;
		}
		delete path;
	}
}

void EngineerClass::SetSuppliesList(Supply_stack *** supp, int * nsupply)
{
	supplieslist = supp;
	n_supplies = nsupply;
}
