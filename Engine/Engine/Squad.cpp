#include "Squad.h"

void SquadClass::SetTargetToMove(pathstruct* target)
{
	int MaxRange = 250;

	for (int i = 0; i < Members; i++)
	{
		/*if (MembersList[i] == 0)
		{
			continue;
		}
		int Xrand = rand() % 100;
		int Yrand = rand() % 100;
		float X = (float)(MaxRange * 2 * (Xrand / 100.0) - MaxRange);
		MaxRange = (int)sqrt(pow(MaxRange, 2) - pow(X, 2));
		X += target.x;
		float Y = (float)((MaxRange * 2 * (Yrand / 100.0) - MaxRange) + target.y);*/
		MembersList[i]->SetTarget(target);
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
	if (MembersList[0]->GetUnitType() == OBJTYPES::ENGINEERS)
	{
		for (int i = 0; i < Members; i++)
		{
			((EngineerClass*)MembersList[i])->RepairTarget((Building*)input);
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
