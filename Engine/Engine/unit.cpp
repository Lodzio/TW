#include "unit.h"
#include "Pathfinding.h"

int UnitClass::Give_Products(InvObject * obj)
{
	double free_weight = MAX_WEIGHT - AccWeight;
	int possible_ammount = free_weight / obj->weight;
	if (!possible_ammount)
		return 0;

	for (int i = 0; i < 256; i++)
	{
		if (Inventory[i])
		{
			if (Inventory[i]->type == obj->type)
			{
				Inventory[i]->ammount += possible_ammount;
				obj->ammount -= possible_ammount;
				return possible_ammount;
			}
		}
	}

	for (int i = 0; i < 256; i++)
	{
		if (!Inventory[i])
		{
			if (Inventory[i]->type == obj->type)
			{
				Inventory[i]->ammount += possible_ammount;
				
			}
		}
	}
}

UnitClass::UnitClass()
{
	AccWeight = 0;
	speed.x = 0;
	speed.y = 0;
	resistance.x = 0;
	resistance.y = 0;
	Mode.UnitMode = Mode.MODE_IDLE;
	Range_Of_Object = 40;
	for (int i = 0; i < 256; i++)
		Inventory[i] = 0;
}

void UnitClass::SetTarget(D2D1_POINT_2F tar)
{
	Path = pathmaker->RequestPath(Position, tar);
	Target = Path->GetPoint();
	Mode.UnitMode = Mode.MODE_MOVING;
}

void UnitClass::SetMaxAcceleration(float input)
{
	MaxAcceleration = input;
}

void UnitClass::SetMaxSpeed(float input)
{
	MaxSpeed = input;
}

void UnitClass::CalcPosition(int timeDiff)
{
	StartCorner.x = Position.x - (Axis.x * (Texcord.right - Texcord.left) * Scale.width);
	StartCorner.y = Position.y - (Axis.y * (Texcord.bottom - Texcord.top) * Scale.height);
	
	if (Mode.UnitMode == Mode.MODE_MOVING || Mode.UnitMode == Mode.MODE_REPARING)
	{
		double Speed = sqrt(pow(speed.x, 2) + pow(speed.y, 2));
		D2D1_POINT_2F Subtract_Pos;
		Subtract_Pos.x = Target.x - Position.x;
		Subtract_Pos.y = Target.y - Position.y;
		double subrot = atan2(Subtract_Pos.y, Subtract_Pos.x);
		Rotation = (int)(subrot * 180 / 3.14159265358979323846);
		double distance = sqrt(pow(Subtract_Pos.x, 2) + pow(Subtract_Pos.y, 2));
		D2D1_POINT_2F SpeedNeeded;

		if (distance < 2 * Size.height)
		{
			if (distance < Size.height / 3.0)
			{
				Target = Path->GetPoint();
				SpeedNeeded.x = 0;
				SpeedNeeded.y = 0;
				if (Target.x == -1 && Target.y == -1 && Mode.UnitMode == Mode.MODE_MOVING)
				{
					Mode.UnitMode = false;
					delete Path;
					Path = 0;
				}
			}
			else if (Path->isempty())
			{
				SpeedNeeded.x = (float)(Subtract_Pos.x / distance * MaxSpeed * distance / (2 * Size.height));
				SpeedNeeded.y = (float)(Subtract_Pos.y / distance * MaxSpeed * distance / (2 * Size.height));
			}
			else
			{
				SpeedNeeded.x = (float)(Subtract_Pos.x / distance * MaxSpeed);
				SpeedNeeded.y = (float)(Subtract_Pos.y / distance * MaxSpeed);
			}
		}
		else
		{
			SpeedNeeded.x = (float)(Subtract_Pos.x / distance * MaxSpeed);
			SpeedNeeded.y = (float)(Subtract_Pos.y / distance * MaxSpeed);
		}

		D2D1_POINT_2F SpeedToCorrect;
		SpeedToCorrect.x = SpeedNeeded.x - speed.x;
		SpeedToCorrect.y = SpeedNeeded.y - speed.y;
		double SpeedToCorrect_Sum = sqrt(pow(SpeedToCorrect.x, 2) + pow(SpeedToCorrect.y, 2));
		acceleration.x = (float)(SpeedToCorrect.x * MaxAcceleration / SpeedToCorrect_Sum);
		acceleration.y = (float)(SpeedToCorrect.y * MaxAcceleration / SpeedToCorrect_Sum);

		speed.x += acceleration.x * timeDiff / 1000.0f;
		speed.y += acceleration.y * timeDiff / 1000.0f;

	}
	else
	{
		double Speed = sqrt(pow(speed.x, 2) + pow(speed.y, 2));
		if (Speed > 2)
		{
			acceleration.x = (float)(0.2 * -speed.x * MaxAcceleration / Speed);
			acceleration.y = (float)(0.2 * -speed.y * MaxAcceleration / Speed);
		}
		else
		{
			acceleration.x = -speed.x;
			acceleration.y = -speed.y;
		}
		speed.x += (float)(acceleration.x * timeDiff / 1000.0);
		speed.y += (float)(acceleration.y * timeDiff / 1000.0);
		double subrot = atan2(speed.y, speed.x);
		Rotation = (int)(subrot * 180 / 3.14159265358979323846);
	}

	speed.x += resistance.x  * timeDiff / 1000.0f;
	speed.y += resistance.y  * timeDiff / 1000.0f;

	Position.x += speed.x * timeDiff / 1000.0f;
	Position.y += speed.y * timeDiff / 1000.0f;

	resistance.x = 0;
	resistance.y = 0;

	speed.x /= 1.0001f;
	speed.y /= 1.0001f;

	return;
}

void UnitClass::CalcCollide(Object * SecondObj)
{
	D2D1_POINT_2F Range;
	float RangeDis;
	Range.x = SecondObj->GetPosition().x - Position.x;
	Range.y = SecondObj->GetPosition().y - Position.y;
	RangeDis = (float)sqrt(pow(Range.x, 2) + pow(Range.y, 2));
	if (RangeDis < Range_Of_Object)
	{
		resistance.x -= Range.x * (1 - (RangeDis / 40)) * 100;
		resistance.y -= Range.y * (1 - (RangeDis / 40)) * 100;
	}
}

void UnitClass::Draw(ID2D1RenderTarget * RenderTarget, ID2D1SolidColorBrush * Brush)
{
	RenderTarget->DrawLine(Position, Point2F(Position.x + resistance.x, Position.y + resistance.y), Brush);
}

D2D1_POINT_2F UnitClass::GetVector()
{
	D2D1_POINT_2F output;
	double Speed = sqrt(pow(speed.x, 2) + pow(speed.y, 2));
	output.x = (float)(speed.x / Speed);
	output.y = (float)(speed.y / Speed);
	return output;
}

void UnitClass::SetUnitType(UINT input)
{
	UnitType = input;
}

void UnitClass::SetPosition(D2D1_POINT_2F Input)
{
	if (Position.x == Input.x)
		speed.y = 0;
	if (Position.y == Input.y)
		speed.x = 0;
	Position = Input;
}

void UnitClass::SetPathfinder(PFindingclass * pmaker)
{
	pathmaker = pmaker;
}
