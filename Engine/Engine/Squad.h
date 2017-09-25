#ifndef  _SQUADCLASS_H_
#define _SQUADCLASS_H_
#include "Engineer.h"
#include "Soldier.h"
#include <cstdlib>

class SquadClass
{
public:
	void SetTargetToMove(D2D1_POINT_2F target);
	UnitClass* GetUnit(int index);
	void initialize(int capacity);
	bool AddMember(UnitClass* input);
	void CalcPosition(int timer);
	bool IsObjectMember(UnitClass* input);
	void Select(bool whatselect);
	void SetTargetToRepair(Object* input);
	void CalcShooting(float timer, SparksClass*& Sparks);
	int GetMembers();
	UINT GetType();
	D2D1_POINT_2F GetPosition();
private:
	int MaxSquadMembers;
	UnitClass** MembersList;
	int Members;
	UINT SquadType;
};
#endif 