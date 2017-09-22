#ifndef  _SOLDIERCLASS_H_
#define _SOLDIERCLASS_H_
#include "unit.h"

class SoldierClass : public UnitClass
{
public:
	SoldierClass();
	void CheckTargetsinView(int ammount, Object** input);
	void SetTargetToShoot();
	void Shoot(float DeltaTime, SparksClass*& SparkInput);
private:
	Object** TargetsInView;
	Object* AttackedTarget;
	UINT AmmountofTargets;
	bool IsShooting;
	int ShootsinSeries;
	int BulletsInMagazine;
	bool SeriesDelay;
	float LastShootTime;
};

#endif // ! _SOLDIERCLASS_H_