#include "QuadTreeSystem.h"

AreaData QuadTreeClass::GetListToColide()
{
	AreaData output;
	output.Number = 0;
	AreaData GetListToCollideBuffor[ammount_of_areas][ammount_of_areas];
	Neighbours FinalInput;
	Neighbours North, South, East, West, NorthWest, NorthEast, SouthWest, SouthEast;
	int outputArrayPlace = 0;

	for (int i = 0; i < ammount_of_areas; i++)//X
	{
		for (int j = 0; j < ammount_of_areas; j++)//Y
		{
			if (Areas[i][j].GetAmmount() == 0)
			{
				continue;
			}
			FinalInput.numbers = 0;
			if (i != 0)
			{
				West = Areas[i - 1][j].GetList(Side::East);
				FinalInput.numbers += West.numbers;
			}
			if (j != 0)
			{
				North = Areas[i][j - 1].GetList(Side::South);
				FinalInput.numbers += North.numbers;
			}
			if (i != ammount_of_areas - 1)
			{
				East = Areas[i + 1][j].GetList(Side::West);
				FinalInput.numbers += East.numbers;
			}
			if (j != ammount_of_areas - 1)
			{
				South = Areas[i][j + 1].GetList(Side::North);
				FinalInput.numbers += South.numbers;
			}

			if (j != 0 && i != ammount_of_areas - 1)
			{
				NorthEast = Areas[i + 1][j - 1].GetList(Side::SouthWest);
				FinalInput.numbers += NorthEast.numbers;
			}

			if (j != 0 && i != 0)
			{
				NorthWest = Areas[i - 1][j - 1].GetList(Side::SouthEast);
				FinalInput.numbers += NorthWest.numbers;
			}

			if (j !=  ammount_of_areas - 1 && i != ammount_of_areas - 1)
			{
				SouthEast = Areas[i + 1][j + 1].GetList(Side::NorthWest);
				FinalInput.numbers += SouthEast.numbers;
			}

			if (j != ammount_of_areas - 1 && i != 0)
			{
				SouthWest = Areas[i - 1][j + 1].GetList(Side::NorthEast);
				FinalInput.numbers += SouthWest.numbers;
			}

			if (FinalInput.numbers)
			{
				FinalInput.NeighboursData = new AreaData[FinalInput.numbers];
			}

			int I = 0;

			if (i != 0)
			{
				for (int k = 0; k < West.numbers; k++)
				{
					FinalInput.NeighboursData[I] = West.NeighboursData[k];
					I++;
				}
				if (West.NeighboursData)
					delete[] West.NeighboursData;
			}

			if (j != 0)
			{
				for (int k = 0; k < North.numbers; k++)
				{
					FinalInput.NeighboursData[I] = North.NeighboursData[k];
					I++;
				}
				if (North.NeighboursData)
					delete[] North.NeighboursData;
			}

			if (j != ammount_of_areas - 1)
			{
				for (int k = 0; k < South.numbers; k++)
				{
					FinalInput.NeighboursData[I] = South.NeighboursData[k];
					I++;
				}
				if (South.NeighboursData)
					delete[] South.NeighboursData;
			}

			if (i != ammount_of_areas - 1)
			{
				for (int k = 0; k < East.numbers; k++)
				{
					FinalInput.NeighboursData[I] = East.NeighboursData[k];
					I++;
				}
				if (East.NeighboursData)
					delete[] East.NeighboursData;
			}

			if (j != 0 && i != ammount_of_areas - 1)
			{
				for (int k = 0; k < NorthEast.numbers; k++)
				{
					FinalInput.NeighboursData[I] = NorthEast.NeighboursData[k];
					I++;
				}
				if (NorthEast.NeighboursData)
					delete[] NorthEast.NeighboursData;
			}

			if (j != 0 && i != 0)
			{
				for (int k = 0; k < NorthWest.numbers; k++)
				{
					FinalInput.NeighboursData[I] = NorthWest.NeighboursData[k];
					I++;
				}
				if (NorthWest.NeighboursData)
					delete[] NorthWest.NeighboursData;
			}

			if (j != ammount_of_areas - 1 && i != ammount_of_areas - 1)
			{
				for (int k = 0; k < SouthEast.numbers; k++)
				{
					FinalInput.NeighboursData[I] = SouthEast.NeighboursData[k];
					I++;
				}
				if (SouthEast.NeighboursData)
					delete[] SouthEast.NeighboursData;
			}

			if (j != ammount_of_areas - 1 && i != 0)
			{
				for (int k = 0; k < SouthWest.numbers; k++)
				{
					FinalInput.NeighboursData[I] = SouthWest.NeighboursData[k];
					I++;
				}
				if (SouthWest.NeighboursData)
					delete[] SouthWest.NeighboursData;
			}

			GetListToCollideBuffor[i][j] = Areas[i][j].GetListToColide(FinalInput);
			output.Number += GetListToCollideBuffor[i][j].Number;
			if (FinalInput.numbers)
			{
				delete[] FinalInput.NeighboursData;
			}
		}
	}

	output.ObjectsList = new Object*[output.Number];

	for (int i = 0; i < ammount_of_areas; i++)
	{
		for (int j = 0; j < ammount_of_areas; j++)
		{
			if (Areas[i][j].GetAmmount())
			{
				for (int k = 0; k < GetListToCollideBuffor[i][j].Number; k++)
				{
					output.ObjectsList[outputArrayPlace] = GetListToCollideBuffor[i][j].ObjectsList[k];
					outputArrayPlace++;
				}
				delete[] GetListToCollideBuffor[i][j].ObjectsList;
			}
		}
	}

	return output;
}

void QuadTreeClass::Draw(ID2D1RenderTarget * RenderTarget, ID2D1SolidColorBrush * Brush)
{
	for (int i = 0; i < ammount_of_areas; i++)
	{
		for (int j = 0; j < ammount_of_areas; j++)
		{
			Areas[i][j].Draw(RenderTarget, Brush);
		}
	}
}

Object * QuadTreeClass::GetObjectByCoords(D2D1_POINT_2F input)
{
	Object* output;
	int Width = MapSize.width / ammount_of_areas;
	int Height = MapSize.height / ammount_of_areas;
	int X, Y;
	X = (int)input.x / Width;
	Y = (int)input.y / Height;
	output = Areas[X][Y].GetObjectByCoords(input);
	if (output == 0)
	{
		if (((int)input.x % Width) / (float)Width > 0.5)
		{
			if (X + 1 < ammount_of_areas)
			{
				output = Areas[X + 1][Y].GetObjectByCoords(input);
			}
			if (output == 0)
			{
				if (((int)input.y & Height) / (float)Height > 0.5)
				{
					if (Y + 1 < ammount_of_areas)
					{
						output = Areas[X][Y + 1].GetObjectByCoords(input);
						if (output == 0 && X + 1 < ammount_of_areas)
						{
							output = Areas[X +1][Y + 1].GetObjectByCoords(input);
						}
					}
				}
				else
				{
					if (Y > 0)
					{
						output = Areas[X][Y - 1].GetObjectByCoords(input);
						if (output == 0 && X + 1 < ammount_of_areas)
						{
							output = Areas[X + 1][Y - 1].GetObjectByCoords(input);
						}
					}
				}
			}
		}
		else
		{
			if (X > 0)
			{
				output = Areas[X - 1][Y].GetObjectByCoords(input);
			}
			if (output == 0)
			{
				if (((int)input.y & Height) / (float)Height > 0.5)
				{
					if (Y + 1 < ammount_of_areas)
					{
						output = Areas[X][Y + 1].GetObjectByCoords(input);
						if (output == 0 && X > 0)
						{
							output = Areas[X - 1][Y + 1].GetObjectByCoords(input);
						}
					}
				}
				else
				{
					if (Y > 0)
					{
						output = Areas[X][Y - 1].GetObjectByCoords(input);
						if (output == 0 && X > 0)
						{
							output = Areas[X - 1][Y - 1].GetObjectByCoords(input);
						}
					}
				}
			}
		}
	}
	return output;
}

void QuadTreeClass::Initialize(D2D1_SIZE_U mapsize)
{
	MapSize = mapsize;
	for (int i = 0; i < ammount_of_areas; i++)
	{
		for (int j = 0; j < ammount_of_areas; j++)
		{
			D2D1_RECT_U AreaSize;
			int Width = MapSize.width / ammount_of_areas;
			int Height = MapSize.height / ammount_of_areas;
			AreaSize.bottom = (j + 1) * Height;
			AreaSize.top = j * Height;
			AreaSize.left = i * Width;
			AreaSize.right = (i + 1) * Width;
			Areas[i][j].Initialize(SizeU(MapSize.width / 40, MapSize.height / 40), AreaSize, 5);
		}
	}
}

void QuadTreeClass::PutInObject(Object * input)
{
	int Width = MapSize.width / ammount_of_areas;
	int Height = MapSize.height / ammount_of_areas;
	int X, Y;
	X = (int)input->GetPosition().x / Width;
	Y = (int)input->GetPosition().y / Height;
	Areas[X][Y].PutInObject(input);
	return;
}

void QuadTreeClass::Reset()
{
	for (int i = 0; i < ammount_of_areas; i++)
	{
		for (int j = 0; j < ammount_of_areas; j++)
		{
			Areas[i][j].Reset();
		}
	}
}

void QuadTreeClass::SubArea::Initialize(D2D1_SIZE_U arealmit, D2D1_RECT_U size, UINT capacity)
{
	AreaLimit = arealmit;
	Area = size;
	Capacity = capacity;
	Ammount = 0;
	List = 0;
	IsDivided = false;
}

void QuadTreeClass::SubArea::PutInObject(Object* input)
{
	D2D1_SIZE_U SizeOfArea;
	SizeOfArea.height = Area.bottom - Area.top;
	SizeOfArea.width = Area.right - Area.left;
	if (Ammount + 1 > Capacity && SizeOfArea.height / 2 >= AreaLimit.height && SizeOfArea.width / 2 >= AreaLimit.width)
	{
		if (!IsDivided)
		{
			D2D1_RECT_U SmallerArea;

			NorthEast = new SubArea;
			SmallerArea.top = Area.top;
			SmallerArea.bottom = Area.bottom - (SizeOfArea.height / 2);
			SmallerArea.right = Area.right;
			SmallerArea.left = Area.left + (SizeOfArea.width / 2);
			NorthEast->Initialize(AreaLimit, SmallerArea, Capacity);

			NorthWest = new SubArea;
			SmallerArea.right = Area.right - (SizeOfArea.width / 2);
			SmallerArea.left = Area.left;
			NorthWest->Initialize(AreaLimit, SmallerArea, Capacity);

			SouthWest = new SubArea;
			SmallerArea.top = Area.top + (SizeOfArea.height / 2);
			SmallerArea.bottom = Area.bottom;
			SouthWest->Initialize(AreaLimit, SmallerArea, Capacity);

			SouthEast = new SubArea;
			SmallerArea.right = Area.right;
			SmallerArea.left = Area.left + (SizeOfArea.width / 2);
			SouthEast->Initialize(AreaLimit, SmallerArea, Capacity);

			for (int i = 0; i < (int)Ammount; i++)
			{
				if (List[i]->GetPosition().x > Area.left + (SizeOfArea.width / 2)) //East
				{
					if (List[i]->GetPosition().y > Area.top + (SizeOfArea.height / 2)) //South
					{
						SouthEast->PutInObject(List[i]);
					}
					else //North
					{
						NorthEast->PutInObject(List[i]);
					}
				}
				else //West
				{
					if (List[i]->GetPosition().y > Area.top + (SizeOfArea.height / 2)) //South
					{
						SouthWest->PutInObject(List[i]);
					}
					else //North
					{
						NorthWest->PutInObject(List[i]);
					}
				}
			}
			delete[] List;
			List = 0;
			IsDivided = true;
		}
		if (input->GetPosition().x > Area.left + (SizeOfArea.width / 2)) //East
		{
			if (input->GetPosition().y > Area.top + (SizeOfArea.height / 2)) //South
			{
				SouthEast->PutInObject(input);
			}
			else //North
			{
				NorthEast->PutInObject(input);
			}
		}
		else //West
		{
			if (input->GetPosition().y > Area.top + (SizeOfArea.height / 2)) //South
			{
				SouthWest->PutInObject(input);
			}
			else //North
			{
				NorthWest->PutInObject(input);
			}
		}
		Ammount++;
	}
	else
	{
		Object** Buffor;
		Buffor = new Object*[Ammount + 1];

		for (int i = 0; i < (int)Ammount; i++)
		{
			Buffor[i] = List[i];
		}
		Buffor[Ammount] = input;
		Ammount++;
		if (List != 0)
		{
			delete[] List;
		}
		List = Buffor;
	}
}

void QuadTreeClass::CalculateViewArea()
{
	for (int i = 0; i < ammount_of_areas; i++)
	{
		for (int j = 0; j < ammount_of_areas; j++)
		{
			Object** NewInput;
			int AmmountofObjectforNewInput = 0;
			AmmountofObjectforNewInput += Areas[i][j].GetAmmount();
			{
				if (i != 0)
				{
					if (j != 0)
					{
						AmmountofObjectforNewInput += Areas[i - 1][j - 1].GetAmmount();
					}
					AmmountofObjectforNewInput += Areas[i - 1][j].GetAmmount();
					if (j != ammount_of_areas - 1)
					{
						AmmountofObjectforNewInput += Areas[i - 1][j + 1].GetAmmount();
					}
				}
				if (j != 0)
				{
					AmmountofObjectforNewInput += Areas[i][j - 1].GetAmmount();
				}
				if (j != ammount_of_areas - 1)
				{
					AmmountofObjectforNewInput += Areas[i][j + 1].GetAmmount();
				}
				if (i != ammount_of_areas - 1)
				{
					if (j != 0)
					{
						AmmountofObjectforNewInput += Areas[i + 1][j - 1].GetAmmount();
					}
					AmmountofObjectforNewInput += Areas[i + 1][j].GetAmmount();
					if (j != ammount_of_areas - 1)
					{
						AmmountofObjectforNewInput += Areas[i + 1][j + 1].GetAmmount();
					}
				}
			}
			NewInput = new Object*[AmmountofObjectforNewInput];
			int I = 0;
			{


				Neighbours Buffor = Areas[i][j].GetList();
				for (int k = 0; k < Buffor.numbers; k++)
				{
					for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
					{
						NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
						I++;
					}
				}
				if (Buffor.numbers)
					delete[] Buffor.NeighboursData;


				if (i != 0)
				{


					if (j != 0)
					{
						Neighbours Buffor = Areas[i - 1][j - 1].GetList();
						for (int k = 0; k < Buffor.numbers; k++)
						{
							for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
							{
								NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
								I++;
							}
						}
						if (Buffor.numbers)
							delete[] Buffor.NeighboursData;
					}


					Neighbours Buffor = Areas[i - 1][j].GetList();
					for (int k = 0; k < Buffor.numbers; k++)
					{
						for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
						{
							NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
							I++;
						}
					}
					if (Buffor.numbers)
						delete[] Buffor.NeighboursData;


					if (j != ammount_of_areas - 1)
					{
						Neighbours Buffor = Areas[i - 1][j + 1].GetList();
						for (int k = 0; k < Buffor.numbers; k++)
						{
							for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
							{
								NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
								I++;
							}
						}
						if (Buffor.numbers)
							delete[] Buffor.NeighboursData;
					}
				}
				if (j != 0)
				{


					Neighbours Buffor = Areas[i][j - 1].GetList();
					for (int k = 0; k < Buffor.numbers; k++)
					{
						for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
						{
							NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
							I++;
						}
					}
					if (Buffor.numbers)
						delete[] Buffor.NeighboursData;


				}
				if (j != ammount_of_areas - 1)
				{


					Neighbours Buffor = Areas[i][j + 1].GetList();
					for (int k = 0; k < Buffor.numbers; k++)
					{
						for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
						{
							NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
							I++;
						}
					}
					if (Buffor.numbers)
						delete[] Buffor.NeighboursData;


				}
				if (i != ammount_of_areas - 1)
				{
					if (j != 0)
					{


						Neighbours Buffor = Areas[i + 1][j - 1].GetList();
						for (int k = 0; k < Buffor.numbers; k++)
						{
							for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
							{
								NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
								I++;
							}
						}
						if (Buffor.numbers)
							delete[] Buffor.NeighboursData;


					}


					Neighbours Buffor = Areas[i + 1][j].GetList();
					for (int k = 0; k < Buffor.numbers; k++)
					{
						for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
						{
							NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
							I++;
						}
					}
					if (Buffor.numbers)
						delete[] Buffor.NeighboursData;


					if (j != ammount_of_areas - 1)
					{
						Neighbours Buffor = Areas[i + 1][j + 1].GetList();
						for (int k = 0; k < Buffor.numbers; k++)
						{
							for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
							{
								NewInput[I] = Buffor.NeighboursData[k].ObjectsList[l];
								I++;
							}
						}

						if (Buffor.numbers)
							delete[] Buffor.NeighboursData;
					}
				}
			}
			Neighbours Buffor = Areas[i][j].GetList();
			for (int k = 0; k < Buffor.numbers; k++)
			{
				for (int l = 0; l < Buffor.NeighboursData[k].Number; l++)
				{
					if (Buffor.NeighboursData[k].ObjectsList[l]->GetUnitType() == OBJTYPES::SOLDIER)
					{
						((SoldierClass*)Buffor.NeighboursData[k].ObjectsList[l])->CheckTargetsinView(AmmountofObjectforNewInput, NewInput);
					}
				}
			}
			delete[] NewInput;
			if (Buffor.numbers)
				delete[] Buffor.NeighboursData;
		}
	}
}

Neighbours QuadTreeClass::SubArea::GetList(UINT Side)
{
	Neighbours output;
	if (!IsDivided)
	{
		
		Object** Buffor = 0;
		if (Ammount)
		{
			//Buffor = new Object*[Ammount];
			output.numbers = 1;
			output.NeighboursData = new AreaData[1];
			output.NeighboursData->Area = Area;
			output.NeighboursData->Number = Ammount;
		}
		else
		{
			output.numbers = 0;
			output.NeighboursData = 0;
		}
		/*for (int i = 0; i < Ammount; i++)
		{
			Buffor[i] = List[i];
		}*/
		if (Ammount)
		{
			output.NeighboursData->ObjectsList = List;
		}
		return output;
	}
	else
	{
		switch (Side)
		{

		case Side::All:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthEast = NorthEast->GetList();
			Neighbours res_NorthWest = NorthWest->GetList();
			Neighbours res_SouthEast = SouthEast->GetList();
			Neighbours res_SouthWest = SouthWest->GetList();
			
			output.numbers = res_NorthEast.numbers + res_NorthWest.numbers + res_SouthEast.numbers + res_SouthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthEast.NeighboursData;
			for (int i = 0; i < res_NorthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_NorthWest.NeighboursData;
			for (int i = 0; i < res_NorthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_SouthEast.NeighboursData;
			for (int i = 0; i < res_SouthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_SouthWest.NeighboursData;
			for (int i = 0; i < res_SouthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::North:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthEast = NorthEast->GetList();
			Neighbours res_NorthWest = NorthWest->GetList();

			output.numbers = res_NorthEast.numbers + res_NorthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthEast.NeighboursData;
			for (int i = 0; i < res_NorthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_NorthWest.NeighboursData;
			for (int i = 0; i < res_NorthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::East:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthEast = NorthEast->GetList();
			Neighbours res_SouthEast = SouthEast->GetList();

			output.numbers = res_NorthEast.numbers + res_SouthEast.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthEast.NeighboursData;
			for (int i = 0; i < res_NorthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_SouthEast.NeighboursData;
			for (int i = 0; i < res_SouthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::South:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_SouthEast = SouthEast->GetList();
			Neighbours res_SouthWest = SouthWest->GetList();

			output.numbers = res_SouthEast.numbers + res_SouthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_SouthEast.NeighboursData;
			for (int i = 0; i < res_SouthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_SouthWest.NeighboursData;
			for (int i = 0; i < res_SouthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::West:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthWest = NorthWest->GetList();
			Neighbours res_SouthWest = SouthWest->GetList();

			output.numbers = res_NorthWest.numbers + res_SouthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthWest.NeighboursData;
			for (int i = 0; i < res_NorthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			SubBuffor = res_SouthWest.NeighboursData;
			for (int i = 0; i < res_SouthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::NorthEast:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthEast = NorthEast->GetList();

			output.numbers = res_NorthEast.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthEast.NeighboursData;
			for (int i = 0; i < res_NorthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::NorthWest:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_NorthWest = NorthWest->GetList();

			output.numbers = res_NorthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_NorthWest.NeighboursData;
			for (int i = 0; i < res_NorthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::SouthEast:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_SouthEast = SouthEast->GetList();

			output.numbers = res_SouthEast.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_SouthEast.NeighboursData;
			for (int i = 0; i < res_SouthEast.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		case Side::SouthWest:
		{
			AreaData* Buffor = 0;
			AreaData* SubBuffor;
			int I = 0;
			Neighbours res_SouthWest = SouthWest->GetList();

			output.numbers = res_SouthWest.numbers;
			if (output.numbers)
			{
				Buffor = new AreaData[output.numbers];
			}

			SubBuffor = res_SouthWest.NeighboursData;
			for (int i = 0; i < res_SouthWest.numbers; i++)
			{
				Buffor[I] = SubBuffor[i];
				I++;
			}
			if (SubBuffor)
			{
				delete[] SubBuffor;
			}

			output.NeighboursData = Buffor;
			return output;
		}

		}
	}
}

AreaData QuadTreeClass::SubArea::GetListToColide(Neighbours input)
{
	AreaData output;
	int I = 0;
	if (IsDivided)
	{
		int I = 0;
		AreaData outputNorthWest, outputNorthEast, outputSouthWest, outputSouthEast;
		Neighbours NewInput;
		Neighbours North, South, West, East, subNorthWest, subNorthEast, subSouthEast, subSouthWest;
		D2D1_RECT_U  MainRect;
		int AmmountOfOutsiders = 0;
		D2D1_RECT_U SideRect;
		NewInput.NeighboursData = 0;


		/////////////////////////////////////////////////////NorthWest
		East = NorthEast->GetList(Side::West);
		South = SouthWest->GetList(Side::North);
		subSouthEast = SouthEast->GetList(Side::NorthWest);
		MainRect = NorthWest->Area;
		for (int i = 0; i < input.numbers; i++)
		{
			SideRect = input.NeighboursData[i].Area;
			if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
			{
				AmmountOfOutsiders++;
			}
		}

		NewInput.numbers = AmmountOfOutsiders + East.numbers + South.numbers + subSouthEast.numbers;
		if (NewInput.numbers)
		{
			NewInput.NeighboursData = new AreaData[NewInput.numbers];
			for (int i = 0; i < input.numbers; i++)
			{
				SideRect = input.NeighboursData[i].Area;
				if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
				{
					NewInput.NeighboursData[I] = input.NeighboursData[i];
					I++;
				}
			}

			for (int i = 0; i < East.numbers; i++)
			{
				NewInput.NeighboursData[I] = East.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < South.numbers; i++)
			{
				NewInput.NeighboursData[I] = South.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < subSouthEast.numbers; i++)
			{
				NewInput.NeighboursData[I] = subSouthEast.NeighboursData[i];
				I++;
			}

			if (South.NeighboursData)
			{
				delete[] South.NeighboursData;
			}

			if (East.NeighboursData)
			{
				delete[] East.NeighboursData;
			}

			if (subSouthEast.NeighboursData)
			{
				delete[] subSouthEast.NeighboursData;
			}
		}

		outputNorthWest = NorthWest->GetListToColide(NewInput);
		if (NewInput.NeighboursData)
		{
			delete[] NewInput.NeighboursData;
			NewInput.NeighboursData = 0;
			I = 0;
			AmmountOfOutsiders = 0;
		}


		/////////////////////////////////////////////////////NorthEast
		West = NorthWest->GetList(Side::East);
		South = SouthEast->GetList(Side::North);
		subSouthWest = SouthWest->GetList(Side::NorthEast);
		MainRect = NorthEast->Area;
		for (int i = 0; i < input.numbers; i++)
		{
			SideRect = input.NeighboursData[i].Area;
			if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
			{
				AmmountOfOutsiders++;
			}
		}

		NewInput.numbers = AmmountOfOutsiders + West.numbers + South.numbers + subSouthWest.numbers;
		if (NewInput.numbers)
		{
			NewInput.NeighboursData = new AreaData[NewInput.numbers];
			for (int i = 0; i < input.numbers; i++)
			{
				SideRect = input.NeighboursData[i].Area;
				if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
				{
					NewInput.NeighboursData[I] = input.NeighboursData[i];
					I++;
				}
			}

			for (int i = 0; i < West.numbers; i++)
			{
				NewInput.NeighboursData[I] = West.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < South.numbers; i++)
			{
				NewInput.NeighboursData[I] = South.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < subSouthWest.numbers; i++)
			{
				NewInput.NeighboursData[I] = subSouthWest.NeighboursData[i];
				I++;
			}

			if (South.NeighboursData)
			{
				delete[] South.NeighboursData;
			}

			if (West.NeighboursData)
			{
				delete[] West.NeighboursData;
			}

			if (subSouthWest.NeighboursData)
			{
				delete[] subSouthWest.NeighboursData;
			}
		}

		outputNorthEast = NorthEast->GetListToColide(NewInput);
		if (NewInput.NeighboursData)
		{
			delete[] NewInput.NeighboursData;
			NewInput.NeighboursData = 0;
			I = 0;
			AmmountOfOutsiders = 0;
		}


		/////////////////////////////////////////////////////SouthEast


		West = SouthWest->GetList(Side::East);
		North = NorthEast->GetList(Side::South);
		subNorthWest = NorthWest->GetList(Side::SouthEast);
		MainRect = SouthEast->Area;
		for (int i = 0; i < input.numbers; i++)
		{
			SideRect = input.NeighboursData[i].Area;
			if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
			{
				AmmountOfOutsiders++;
			}
		}

		NewInput.numbers = AmmountOfOutsiders + West.numbers + North.numbers + subNorthWest.numbers;
		if (NewInput.numbers)
		{
			NewInput.NeighboursData = new AreaData[NewInput.numbers];
			for (int i = 0; i < input.numbers; i++)
			{
				SideRect = input.NeighboursData[i].Area;
				if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.left <= MainRect.right && SideRect.right >= MainRect.left)
				{
					NewInput.NeighboursData[I] = input.NeighboursData[i];
					I++;
				}
			}

			for (int i = 0; i < West.numbers; i++)
			{
				NewInput.NeighboursData[I] = West.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < North.numbers; i++)
			{
				NewInput.NeighboursData[I] = North.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < subNorthWest.numbers; i++)
			{
				NewInput.NeighboursData[I] = subNorthWest.NeighboursData[i];
				I++;
			}

			if (North.NeighboursData)
			{
				delete[] North.NeighboursData;
			}

			if (West.NeighboursData)
			{
				delete[] West.NeighboursData;
			}

			if (subNorthWest.NeighboursData)
			{
				delete[] subNorthWest.NeighboursData;
			}
		}
		outputSouthEast = SouthEast->GetListToColide(NewInput);
		if (NewInput.NeighboursData)
		{
			delete[] NewInput.NeighboursData;
			NewInput.NeighboursData = 0;
			I = 0;
			AmmountOfOutsiders = 0;
		}

		/////////////////////////////////////////////////////SouthWest


		East = SouthEast->GetList(Side::West);
		North = NorthWest->GetList(Side::South);
		subNorthEast = NorthEast->GetList(Side::SouthWest);
		MainRect = SouthWest->Area;
		for (int i = 0; i < input.numbers; i++)
		{
			SideRect = input.NeighboursData[i].Area;
			if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.right >= MainRect.left && SideRect.left <= MainRect.right)
			{
				AmmountOfOutsiders++;
			}
		}

		NewInput.numbers = AmmountOfOutsiders + East.numbers + North.numbers + subNorthEast.numbers;
		if (NewInput.numbers)
		{
			NewInput.NeighboursData = new AreaData[NewInput.numbers];

			for (int i = 0; i < input.numbers; i++)
			{
				SideRect = input.NeighboursData[i].Area;
				if (SideRect.bottom >= MainRect.top && SideRect.top <= MainRect.bottom && SideRect.right >= MainRect.left && SideRect.left <= MainRect.right)
				{
					NewInput.NeighboursData[I] = input.NeighboursData[i];
					I++;
				}
			}

			for (int i = 0; i < East.numbers; i++)
			{
				NewInput.NeighboursData[I] = East.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < North.numbers; i++)
			{
				NewInput.NeighboursData[I] = North.NeighboursData[i];
				I++;
			}

			for (int i = 0; i < subNorthEast.numbers; i++)
			{
				NewInput.NeighboursData[I] = subNorthEast.NeighboursData[i];
				I++;
			}

			if (North.NeighboursData)
			{
				delete[] North.NeighboursData;
			}

			if (East.NeighboursData)
			{
				delete[] East.NeighboursData;
			}

			if (subNorthEast.NeighboursData)
			{
				delete[] subNorthEast.NeighboursData;
			}
		}
		outputSouthWest = SouthWest->GetListToColide(NewInput);
		if (NewInput.NeighboursData)
		{
			delete[] NewInput.NeighboursData;
			NewInput.NeighboursData = 0;
			I = 0;
			AmmountOfOutsiders = 0;
		}


		/////////////////////////////////////////////////////Sum up


		output.Number = outputNorthEast.Number + outputNorthWest.Number + outputSouthWest.Number + outputSouthEast.Number;
		output.ObjectsList = new Object*[output.Number];
		I = 0;
		for (int i = 0; i < outputNorthEast.Number; i++)
		{
			output.ObjectsList[I] = outputNorthEast.ObjectsList[i];
			I++;
		}
		if (outputNorthEast.Number)
			delete[] outputNorthEast.ObjectsList;

		for (int i = 0; i < outputNorthWest.Number; i++)
		{
			output.ObjectsList[I] = outputNorthWest.ObjectsList[i];
			I++;
		}
		if (outputNorthWest.Number)
			delete[] outputNorthWest.ObjectsList;

		for (int i = 0; i < outputSouthEast.Number; i++)
		{
			output.ObjectsList[I] = outputSouthEast.ObjectsList[i];
			I++;
		}
		if (outputSouthEast.Number)
			delete[] outputSouthEast.ObjectsList;

		for (int i = 0; i < outputSouthWest.Number; i++)
		{
			output.ObjectsList[I] = outputSouthWest.ObjectsList[i];
			I++;
		}
		if (outputSouthWest.Number)
			delete[] outputSouthWest.ObjectsList;

		return output;
	}
	else
	{
		Object** resultObjects = 0;
		int NumberofNeighbours = 0;
		for (int i = 0; i < input.numbers; i++)
		{
			NumberofNeighbours += input.NeighboursData[i].Number;
		}
		int ArraySize = Ammount * (Ammount - 1 + NumberofNeighbours) * 2;
		if (ArraySize)
		{
			resultObjects = new Object*[ArraySize];
		}
		for (int i = 0; i < (int)Ammount; i++)
		{
			for (int j = 0; j < (int)Ammount; j++)
			{
				if (i == j)
				{
					continue;
				}
				resultObjects[I] = List[i];
				I++;
				resultObjects[I] = List[j];
				I++;
			}

			for (int j = 0; j < input.numbers; j++)
			{
				for (int k = 0; k < input.NeighboursData[j].Number; k++)
				{
					resultObjects[I] = List[i];
					I++;
					resultObjects[I] = input.NeighboursData[j].ObjectsList[k];
					I++;
				}
			}
		}
		output.ObjectsList = resultObjects;
		output.Number = I;
	}
	return output;
}

UINT QuadTreeClass::SubArea::GetAmmount()
{
	return Ammount;
}

void QuadTreeClass::SubArea::Reset()
{
	if (IsDivided)
	{
		NorthEast->Reset();
		delete NorthEast;
		NorthWest->Reset();
		delete NorthWest;
		SouthWest->Reset();
		delete SouthWest;
		SouthEast->Reset();
		delete SouthEast;
		IsDivided = false;
	}
	if (List)
	{
		delete[] List;
		List = 0;
	}
	Ammount = 0;
}

void QuadTreeClass::SubArea::Draw(ID2D1RenderTarget * RenderTarget, ID2D1SolidColorBrush * Brush)
{
	D2D1_RECT_F rectf;
	rectf.bottom = (float)Area.bottom;
	rectf.left = (float)Area.left;
	rectf.right = (float)Area.right;
	rectf.top = (float)Area.top;
	RenderTarget->DrawRectangle(rectf, Brush);
	if (IsDivided)
	{
		NorthWest->Draw(RenderTarget, Brush);
		NorthEast->Draw(RenderTarget, Brush);
		SouthWest->Draw(RenderTarget, Brush);
		SouthEast->Draw(RenderTarget, Brush);
	}
}

Object * QuadTreeClass::SubArea::GetObjectByCoords(D2D1_POINT_2F input)
{
	if (IsDivided)
	{
		short checked_side;
		Object* output;
		if (Area.left + ((Area.right - Area.left) / 2) > input.x)
		{
			if (Area.top + ((Area.bottom - Area.top) / 2) > input.y)
			{
				output = NorthWest->GetObjectByCoords(input);
				checked_side = Side::NorthWest;
			}
			else
			{
				output = SouthWest->GetObjectByCoords(input);
				checked_side = Side::SouthWest;
			}
		}
		else
		{
			if (Area.top + ((Area.bottom - Area.top) / 2) > input.y)
			{
				output = NorthEast->GetObjectByCoords(input);
				checked_side = Side::NorthEast;
			}
			else
			{
				output = SouthEast->GetObjectByCoords(input);
				checked_side = Side::SouthEast;
			}
		}

		if (output == 0)
		{
			if (checked_side != Side::NorthWest)
			{
				output = NorthWest->GetObjectByCoords(input);
			}

			if (output == 0 && checked_side != Side::SouthWest)
			{
				output = SouthWest->GetObjectByCoords(input);
			}

			if (output == 0 && checked_side != Side::NorthEast)
			{
				output = NorthEast->GetObjectByCoords(input);
			}

			if (output == 0 && checked_side != Side::SouthEast)
			{
				output = SouthEast->GetObjectByCoords(input);
			}
		}
		return output;
	}
	else
	{
		for (int i = 0; i < (int)Ammount; i++)
		{
			if (List[i]->GetStartCorner().x < input.x && List[i]->GetStartCorner().y < input.y)
			{
				if (List[i]->GetStartCorner().x + List[i]->GetSize().width > input.x)
				{
					if (List[i]->GetStartCorner().y + List[i]->GetSize().height > input.y)
					{
						return List[i];
					}
				}
			}
		}
	}
	return 0;
}
