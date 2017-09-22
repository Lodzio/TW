#include "Sparks.h"

void SparksClass::UpdateTime(double input)
{
	for (int i = 0; i < Ammount; i++)
	{
		MainSparksArray[i].time += input;
		if (Type == SparksClass::Blood)
		{
			if (MainSparksArray[i].time > 50000)
			{
				operator-=(MainSparksArray[i]);
				i--;
			}
		}
		else if (MainSparksArray[i].time > 5000)
		{
			operator-=(MainSparksArray[i]);
			i--;
		}
	}
}

void SparksClass::initialize(UINT typein)
{
	Type = typein;
	Ammount = 0;
	MainSparksArray = 0;
}

void SparksClass::Render(ID2D1DeviceContext* Target, ID2D1Effect* Effect)
{
	HRESULT HR;
	ID2D1BitmapBrush* Brush = 0;
	for (int i = 0; i < Ammount; i++)
	{
		if (Type == SparksClass::Blood)
			HR = Effect->SetValue(0, Matrix5x4F(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, (float)( 1 - (MainSparksArray[i].time / 50000)), 0, 0, 0, 0));
		else
			HR = Effect->SetValue(0, Matrix5x4F(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, (float)(1 - (MainSparksArray[i].time / 5000)), 0, 0, 0, 0));
		if (HR != S_OK)
		{
			exit(0);
		}
		ID2D1Image* Image = 0;
		Matrix3x2F PrimaryMatrix;
		D2D1_POINT_2F vector;
		D2D1_RECT_F ImageRect;
		D2D1_SIZE_F Scale;
		Matrix3x2F Rotation;
		Target->GetTransform(&PrimaryMatrix);
		Effect->GetOutput(&Image);
		if (Type == SparksClass::Blood)
		{
			Scale = SizeF((float)(0.00002 * MainSparksArray[i].time + 0.1), (float)(0.00002 * MainSparksArray[i].time + 0.1));
			ImageRect.left = 100;
			ImageRect.right = 200;
			vector = Point2F((float)(MainSparksArray[i].Position.x - (50 * Scale.width)), (float)(MainSparksArray[i].Position.y - (50 * Scale.height)));
		}
		else
		{
			Scale = SizeF((float)(0.0001 * MainSparksArray[i].time), (float)(0.0001 * MainSparksArray[i].time));
			ImageRect.left = 0;
			ImageRect.right = 100;
			vector = Point2F(MainSparksArray[i].Position.x - (50 * Scale.width), MainSparksArray[i].Position.y - (50 * Scale.height));
		}
		Rotation = Matrix3x2F::Rotation((float)MainSparksArray[i].Rotation, Point2F(MainSparksArray[i].Position.x, MainSparksArray[i].Position.y));
		ImageRect.top = 0;
		ImageRect.bottom = 100;
		if (Type != SparksClass::Blood)
		{
			Target->SetTransform(Matrix3x2F::Translation(SizeF(vector.x, vector.y)) * Rotation * Matrix3x2F::Translation(SizeF((float)(0.1 * MainSparksArray[i].time), (float)(0.1 * MainSparksArray[i].time))) * Matrix3x2F::Scale(Scale, vector)* PrimaryMatrix);
		}
		else
		{
			Target->SetTransform(Matrix3x2F::Translation(SizeF(vector.x, vector.y)) * Rotation * Matrix3x2F::Scale(Scale, vector)* PrimaryMatrix);
		}
		
		//Brush->SetTransform(Matrix3x2F::Translation(vector) * Matrix3x2F::Scale(SizeF(0.0002 * MainSparksArray[i].time, 0.0002 * MainSparksArray[i].time), MainSparksArray[i].Position));
		Target->DrawImage(Image, Point2F() ,ImageRect, D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
		Target->SetTransform(PrimaryMatrix);
	}
}

SparksClass SparksClass::operator+=(Spark input)
{
	Spark* Buffor;
	Buffor = new Spark[Ammount + 1];
	for (int i = 0; i < Ammount; i++)
	{
		Buffor[i] = MainSparksArray[i];
	}
	Buffor[Ammount] = input;
	if (Ammount != 0)
	{
		delete[] MainSparksArray;
	}
	Ammount++;
	MainSparksArray = Buffor;
	return *this;
}

SparksClass SparksClass::operator-=(Spark input)
{
	int index = -1;
	for (int i = 0; i < Ammount; i++)
	{
		if (MainSparksArray[i].Position.x == input.Position.x && MainSparksArray[i].Position.y == input.Position.y && MainSparksArray[i].time == input.time)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		return *this;
	}

	if (Ammount - 1 == 0)
	{
		delete[] MainSparksArray;
		MainSparksArray = 0;
		Ammount = 0;
		return *this;
	}

	Spark* Buffor;
	Buffor = new Spark[Ammount - 1];
	bool Missed = false;

	for (int i = 0; i < Ammount; i++)
	{
		if (Missed == false)
		{
			if (i == index)
			{
				Missed = true;
				continue;
			}
			Buffor[i] = MainSparksArray[i];
		}
		else
		{
			Buffor[i - 1] = MainSparksArray[i];
		}
	}

	delete[] MainSparksArray;
	MainSparksArray = Buffor;
	Ammount--;
	return *this;
}
