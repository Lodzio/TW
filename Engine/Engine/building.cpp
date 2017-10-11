#include "building.h"
#include "unit.h"
#include "InventoryObjects.h"

void Building::Collide(Object * input)
{
	if (input->IsMovable())
	{
		D2D1_POINT_2F ObjectPosition = input->GetPosition();
		if (ObjectPosition.x > StartCorner.x && ObjectPosition.y > StartCorner.y)
		{
			if (ObjectPosition.x < StartCorner.x + Size.width)
			{
				if (ObjectPosition.y < StartCorner.y + Size.height)
				{
					D2D1_POINT_2F vector = ((UnitClass*)input)->GetVector();
					D2D1_POINT_2F DeltaX, DeltaY;
					if (vector.x > 0)
					{
						DeltaX.x = ObjectPosition.x - StartCorner.x;
					}
					else
					{
						DeltaX.x = ObjectPosition.x - StartCorner.x - Size.width;
					}
					DeltaX.y = DeltaX.x * (vector.y / vector.x);

					if (vector.y > 0)
					{
						DeltaY.y = ObjectPosition.y - StartCorner.y;
					}
					else
					{
						DeltaY.y = ObjectPosition.y - StartCorner.y -Size.height;
					}
					DeltaY.x = DeltaY.y * (vector.x / vector.y);

					double SizeOfDeltaX, SizeOfDeltaY;
					SizeOfDeltaX = sqrt(pow(DeltaX.x, 2) + pow(DeltaX.y, 2));
					SizeOfDeltaY = sqrt(pow(DeltaY.x, 2) + pow(DeltaY.y, 2));
					if (SizeOfDeltaX > SizeOfDeltaY)
					{
						input->SetPosition(Point2F(ObjectPosition.x, ObjectPosition.y - DeltaY.y));
					}
					else
					{
						input->SetPosition(Point2F(ObjectPosition.x - DeltaX.x, ObjectPosition.y));
					}
				}
			}
		}
	}
}

int Building::build(InvObject* supplies)
{
	int amm_of_supplies = supplies->ammount();
	int delta;
	Changecarring(-amm_of_supplies);
	int neededsup = req_supplies - act_supplies;
	if (amm_of_supplies > neededsup)
	{
		act_supplies = req_supplies;
		Health = MaxHealth;
		delta = -neededsup;
		amm_of_supplies -= neededsup;
	}
	else
	{
		act_supplies += amm_of_supplies;
		Health = ((float)act_supplies / req_supplies) * MaxHealth;
		delta = -amm_of_supplies;
		amm_of_supplies = 0;
	}
	supplies->change_ammount(delta);
	return amm_of_supplies;
}

Building::Building()
{
	supplies_in_way = 0;
	act_supplies = 0;
	Movable = false;
}

void Building::Changecarring(int i)
{
	supplies_in_way += i;
}

int Building::How_many_supplies_needed()
{
	return req_supplies - act_supplies - supplies_in_way;
}
