#include "building.h"
#include "unit.h"

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

Building::Building()
{
	Movable = false;
}