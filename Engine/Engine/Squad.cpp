#include "Squad.h"
#include "Pathfinding.h"

#define PLACE_FOR_ENGENEERS 60

void SquadClass::SetTargetToMove(D2D1_POINT_2F target)
{
	int MaxRange = 250;

	for (int i = 0; i < Members; i++)
	{
		if (MembersList[i] == 0)
		{
			continue;
		}
		int Xrand = rand() % 100;
		int Yrand = rand() % 100;
		float X = (float)(MaxRange * 2 * (Xrand / 100.0) - MaxRange);
		MaxRange = (int)sqrt(pow(MaxRange, 2) - pow(X, 2));
		X += target.x;
		float Y = (float)((MaxRange * 2 * (Yrand / 100.0) - MaxRange) + target.y);
		MembersList[i]->SetTarget(Point2F(X, Y));
	}
}

UnitClass* SquadClass::GetUnit(int index)
{
	MembersList[index];
	return MembersList[index];
}

void SquadClass::initialize(int capacity)
{
	MaxSquadMembers = capacity;
	MembersList = new UnitClass*[capacity];
	Members = 0;
}

bool SquadClass::AddMember(UnitClass* input)
{
	if (Members < MaxSquadMembers)
	{
		MembersList[Members] = input;
		Members++;
		return true;
	}
	return false;
}

void SquadClass::CalcPosition(int timer)
{
	for (int i = 0; i < Members; i++)
	{
		if (MembersList[i] != 0)
		{
			MembersList[i]->CalcPosition(timer);
		}
	}
}

bool SquadClass::IsObjectMember(UnitClass * input)
{
	for (int i = 0; i < Members; i++)
	{
		if (input == MembersList[i])
			return true;
	}
	return false;
}

void SquadClass::Select(bool whatselect)
{
	for (int i = 0; i < Members; i++)
	{
		MembersList[i]->Selected = whatselect;
	}
}

void SquadClass::SetTargetToRepair(Object * input)
{
	D2D1_SIZE_F BuildSize = input->GetSize();
	int places = 0;
	bool* place = 0;
	places += BuildSize.height / PLACE_FOR_ENGENEERS;
	places += BuildSize.width / PLACE_FOR_ENGENEERS;
	places *= 2;
	place = new bool[places];
	for (int i = 0; i < places; i++)
		place[i] = false;

	if (MembersList[0]->GetUnitType() == OBJTYPES::ENGINEERS)
	{
		for (int i = 0; i < Members; i++)
		{
			int submiejsce;
			do
			{
				submiejsce = rand() % places;
			} while (place[submiejsce] == true);
			place[submiejsce] = true;
			D2D1_POINT_2F Point = ((Building*)input)->GetStartCorner();

			if (submiejsce < BuildSize.width / PLACE_FOR_ENGENEERS)
			{
				Point.x += submiejsce * PLACE_FOR_ENGENEERS;
				Point.x += ((int)BuildSize.width % PLACE_FOR_ENGENEERS) / 2;
				Point.x += PLACE_FOR_ENGENEERS;
			}
			else
			{
				submiejsce -= BuildSize.width / PLACE_FOR_ENGENEERS;
				if (submiejsce < BuildSize.height / PLACE_FOR_ENGENEERS)
				{
					Point.x += BuildSize.width;
					Point.y += submiejsce * PLACE_FOR_ENGENEERS;
					Point.y += ((int)BuildSize.height % PLACE_FOR_ENGENEERS) / 2;
					Point.y += PLACE_FOR_ENGENEERS / 2;
				}
				else
				{
					submiejsce -= BuildSize.height / PLACE_FOR_ENGENEERS;
					if (submiejsce < BuildSize.width / PLACE_FOR_ENGENEERS)
					{
						Point.x += BuildSize.width / PLACE_FOR_ENGENEERS;
						Point.x += ((int)BuildSize.width % PLACE_FOR_ENGENEERS) / 2;
						Point.x += PLACE_FOR_ENGENEERS / 2;
						Point.y += BuildSize.height;
					}
					else
					{
						submiejsce -= BuildSize.width / PLACE_FOR_ENGENEERS;
						Point.y += submiejsce * PLACE_FOR_ENGENEERS;
						Point.y += ((int)BuildSize.height % PLACE_FOR_ENGENEERS) / 2;
						Point.y += PLACE_FOR_ENGENEERS / 2;
					}
				}
			}
			((EngineerClass*)MembersList[i])->RepairTarget((Building*)input, Point);
		}
		
	}
}

void SquadClass::CalcShooting(float timer, SparksClass*& Sparks)
{
	for (int i = 0; i < Members; i++)
	{
		if (MembersList[i]->GetUnitType() == OBJTYPES::SOLDIER)
		{
			((SoldierClass*)(MembersList[i]))->SetTargetToShoot();
			((SoldierClass*)(MembersList[i]))->Shoot(timer, Sparks);
		}
	}
}

int SquadClass::GetMembers()
{
	return Members;
}

UINT SquadClass::GetType()
{
	return SquadType;
}

D2D1_POINT_2F SquadClass::GetPosition()
{
	D2D1_POINT_2F output;
	output.x = 0;
	output.y = 0;
	for (int i = 0; i < Members; i++)
	{
		output.x += MembersList[i]->GetPosition().x;
		output.y += MembersList[i]->GetPosition().y;
	}

	output.x /= Members;
	output.y /= Members;

	return output;
}
