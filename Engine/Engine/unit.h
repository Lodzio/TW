#ifndef  _UNITCLASS_H_
#define _UNITCLASS_H_
#define MAX_WEIGHT 50
#include "object.h"
#include "InventoryObjects.h"

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
	int Give_Products(InvObject* obj, int amm);
	UnitClass();
	void SetTarget(D2D1_POINT_2F tar);
	float MaxAcceleration;
	void SetMaxAcceleration(float input);
	void SetMaxSpeed(float input);
	virtual void CalcPosition(int timeDiff);
	void CalcCollide(Object* SecondObj);
	void Draw(ID2D1RenderTarget* RenderTarget, ID2D1SolidColorBrush* Brush);
	D2D1_POINT_2F GetVector();
	void SetUnitType(UINT input);
	void SetPosition(D2D1_POINT_2F Input);
	void SetPathfinder(PFindingclass* pmaker);
	int GiveEquipment(InvObject* in, int amm);
	InvObject* GetInvObject(UINT type);
protected:
	void Chceck_weight();
	double AccWeight;
	InvObject* Inventory[256];
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