#ifndef  _UNITCLASS_H_
#define _UNITCLASS_H_
#include "object.h"

struct pathstruct;
class PFindingclass;

class UnitClass : public Object
{
protected:
	struct InClassModeClass
	{
		enum
		{
			MODE_IDLE = 0,
			MODE_REPARING = 1,
			MODE_MOVING = 2
		};
		Object* OBJTarget;
		UINT UnitMode;
	};
public:
	UnitClass();
	void SetTarget(D2D1_POINT_2F tar);
	float MaxAcceleration;
	void SetMaxAcceleration(float input);
	void SetMaxSpeed(float input);
	void CalcPosition(int timeDiff);
	void CalcCollide(Object* SecondObj);
	void Draw(ID2D1RenderTarget* RenderTarget, ID2D1SolidColorBrush* Brush);
	D2D1_POINT_2F GetVector();
	void SetUnitType(UINT input);
	void SetPosition(D2D1_POINT_2F Input);
	void SetPathfinder(PFindingclass* pmaker);
protected:
	PFindingclass* pathmaker;
	InClassModeClass Mode;
	D2D1_POINT_2F speed;
	float MaxSpeed;
	D2D1_POINT_2F Target;
	pathstruct* Path;
	D2D1_POINT_2F acceleration;
	D2D1_POINT_2F resistance;
	int Range_Of_Object;
};

#endif // ! _UNITCLASS_H_