#include "Soldier.h"

SoldierClass::SoldierClass()
{
	UnitType = OBJTYPES::SOLDIER;
	AmmountofTargets = 0;
	AttackedTarget = 0;
	IsShooting = false;
	BulletsInMagazine = 30;
	ShootsinSeries = 0;
	TargetsInView = 0;
	LastShootTime = 0;
	SeriesDelay = false;
}

void SoldierClass::CheckTargetsinView(int ammount, Object ** input)
{
	int I = 0;
	for (int i = 0; i < ammount; i++)
	{
		if (this == input[i])
			continue;
		if (input[i]->GetFraction() == Fraction)
			continue;
		D2D1_POINT_2F LocalDistance;
		LocalDistance.x = input[i]->GetPosition().x - Position.x;
		LocalDistance.y = input[i]->GetPosition().y - Position.y;
		int Distance = (int)sqrt(pow(LocalDistance.x, 2) + pow(LocalDistance.y, 2));
		if (Distance < 2000)
		{
			double azimut = atan2(LocalDistance.y, LocalDistance.x);
			azimut = azimut * 180 / 3.14159265358979323846;

			if (Rotation < 0)
				Rotation += 360;
			if (Rotation > 360)
				Rotation -= 360;
			int SubAzimut = (int)(azimut - Rotation);

			if (SubAzimut > 180)
				SubAzimut -= 360;
			if (SubAzimut < -180)
				SubAzimut += 360;

			if (SubAzimut < 30 && SubAzimut > -30)
			{
				I++;
			}
		}
	}

	AmmountofTargets = I;
	if (TargetsInView != 0)
	{
		delete[] TargetsInView;
		TargetsInView = 0;
	}
	if (I == 0)
		return;
	TargetsInView = new Object*[I];
	I = 0;

	for (int i = 0; i < ammount; i++)
	{
		if (this == input[i])
			continue;
		if (input[i]->GetFraction() == Fraction)
			continue;
		D2D1_POINT_2F LocalDistance;
		LocalDistance.x = input[i]->GetPosition().x - Position.x;
		LocalDistance.y = input[i]->GetPosition().y - Position.y;
		int Distance = (int)sqrt(pow(LocalDistance.x, 2) + pow(LocalDistance.y, 2));
		if (Distance < 2000)
		{
			double azimut = atan2(LocalDistance.y, LocalDistance.x);
			azimut = azimut * 180 / 3.14159265358979323846;

			if (Rotation < 0)
				Rotation += 360;
			if (Rotation > 360)
				Rotation -= 360;
			int SubAzimut = (int)(azimut - Rotation);

			if (SubAzimut > 180)
				SubAzimut -= 360;
			if (SubAzimut < -180)
				SubAzimut += 360;

			if (SubAzimut < 30 && SubAzimut > -30)
			{
				TargetsInView[I] = input[i];
				I++;
			}
		}
	}
}

void SoldierClass::SetTargetToShoot()
{
	if (AmmountofTargets == 0)
	{
		return;
	}
	double MinRange = -1;
	int subIndex;

	for (int i = 0; i < (int)AmmountofTargets; i++)
	{
		if (TargetsInView[i]->GetFraction() == Fraction)
			continue;
		D2D1_POINT_2F SubtractedVectror;
		D2D1_POINT_2F TargetPosition = TargetsInView[i]->GetPosition();
		SubtractedVectror.x = TargetPosition.x - Position.x;
		SubtractedVectror.y = TargetPosition.y - Position.y;
		double Range = sqrt(pow(SubtractedVectror.x, 2) + pow(SubtractedVectror.y, 2));
		if (Range < MinRange || MinRange == -1)
		{
			MinRange = Range;
			subIndex = i;
		}
	}

	if (MinRange == -1)
	{
		AttackedTarget = 0;
		IsShooting = false;
	}
	else
	{
		AttackedTarget = TargetsInView[subIndex];
		IsShooting = true;
	}
}

void SoldierClass::Shoot(float DeltaTime, SparksClass*& SparkInput)
{
	if (IsShooting)
	{
		LastShootTime += DeltaTime;
		D2D1_POINT_2F TargetPos = AttackedTarget->GetPosition();
		D2D1_POINT_2F SubtractPos;
		SubtractPos.x = TargetPos.x - Position.x;
		SubtractPos.y = TargetPos.y - Position.y;
		double Distance = sqrt(pow(SubtractPos.x, 2) + pow(SubtractPos.y, 2));
		double subrot = atan2(SubtractPos.y, SubtractPos.x);
		Rotation = (int)(subrot * 180 / 3.14159265358979323846);
		double Border = (Distance / 200) * 50;
		if (Distance > 2000)
		{
			AttackedTarget = 0;
			IsShooting = false;
			return;
		}
		if (SeriesDelay)
		{
			if (LastShootTime > 1900)
			{
				LastShootTime -= 1900;
				SeriesDelay = false;
			}
		}
		else if (LastShootTime > 100)
		{
			LastShootTime -= 100;
			double accuracyX = rand() % 101;
			accuracyX -= 50;
			accuracyX /= 100.0;
			accuracyX *= Border;

			double accuracyY = rand() % 101;
			accuracyY -= 50;
			accuracyY /= 100.0;
			accuracyY *= Border;

			Spark NewSpark;
			NewSpark.Position = Point2F((float)(AttackedTarget->GetPosition().x + accuracyX), (float)(AttackedTarget->GetPosition().y + accuracyY));
			NewSpark.time = 0;
			NewSpark.Rotation = rand() % 360;

			float range = (float)sqrt(pow(accuracyX, 2) + pow(accuracyY, 2));

			if (range < Range_Of_Object)
			{
				SparkInput[1] += NewSpark;
			}
			else
			{
				SparkInput[0] += NewSpark;
			}
			
			ShootsinSeries++;
			if (ShootsinSeries == 3)
			{
				ShootsinSeries = 0;
				SeriesDelay = true;
			}
		}
	}
	else
	{
		SetTargetToShoot();
	}
}
