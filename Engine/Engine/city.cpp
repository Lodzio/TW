#include "city.h"

void Cityclass::AddStreet(D2D1_POINT_2L firstcoord, D2D1_POINT_2L secondcoord)
{
	Street* buffor;
	buffor = new Street[n_Streets + 1];
	for (int i = 0; i < n_Streets; i++)
	{
		buffor[i] = Streets[i];
	}
	buffor[n_Streets].firstcor = firstcoord;
	buffor[n_Streets].secondcor = secondcoord;
	if (n_Streets)
		delete[] Streets;
	n_Streets++;
	Streets = buffor;
}

FactoryClass* Cityclass::LookForByIndex(D2D1_POINT_2L input)
{
	FactoryClass* buffor = 0;
	for (int i = 0; i < n_Factory; i++)
	{
		if (input.x == Factory[i]->GetIndex().x && input.y == Factory[i]->GetIndex().y)
		{
			buffor = Factory[i];
		}
	}
	return buffor;
}

void Cityclass::AddNeighbour(D2D1_POINT_2L nwcor, D2D1_POINT_2L secor)
{
	Neighbour* buffor = 0;
	buffor = new Neighbour[n_Neighbours + 1];
	for (int i = 0; i < n_Neighbours; i++)
	{
		buffor[i] = Neighbours[i];
	}

	buffor[n_Neighbours].NWcorn = nwcor;
	buffor[n_Neighbours].SEcorn = secor;

	if (n_Neighbours)
		delete[] Neighbours;
	n_Neighbours++;

	Neighbours = buffor;
}

bool Cityclass::IsCrossing(Street input)
{

	if (input.firstcor.x == input.secondcor.x)
	{
		for (int i = 0; i < n_Streets; i++)
		{
			if (Streets[i].firstcor.y == Streets[i].secondcor.y && Streets[i].firstcor.x < input.firstcor.x && Streets[i].secondcor.x > input.firstcor.x)
			{
				int x = input.firstcor.y - Streets[i].firstcor.y;
				x *= input.secondcor.y - Streets[i].firstcor.y;
				if (x < 0)
					return true;
			}
		}
	}
	else
	{
		for (int i = 0; i < n_Streets; i++)
		{
			if (Streets[i].firstcor.x == Streets[i].secondcor.x && Streets[i].firstcor.y < input.firstcor.y && Streets[i].secondcor.y > input.firstcor.y)
			{
				int x = input.firstcor.x - Streets[i].firstcor.x;
				x *= input.secondcor.x - Streets[i].firstcor.x;
				if (x < 0)
					return true;
			}
		}
	}
	return false;
}

void Cityclass::CompliteRoads(int index)
{
	int ind;
	if (index)
		ind = index;
	else
		ind = n_Neighbours - 1;
	for (int j = 0; j < 4; j++) //wedle wskazówek zegara, zaczynaj¹c od pólnocnej ulicy.
	{
		Street x;
		switch (j)
		{

		case 0:
		{
			x.firstcor = Neighbours[ind].NWcorn;
			x.secondcor = Point2L(Neighbours[ind].SEcorn.x, Neighbours[ind].NWcorn.y);
			break;
		}

		case 1:
		{
			x.firstcor = Point2L(Neighbours[ind].SEcorn.x, Neighbours[ind].NWcorn.y);
			x.secondcor = Neighbours[ind].SEcorn;
			break;
		}

		case 2:
		{
			x.firstcor = Point2L(Neighbours[ind].NWcorn.x, Neighbours[ind].SEcorn.y);
			x.secondcor = Neighbours[ind].SEcorn;
			break;
		}

		case 3:
		{
			x.firstcor = Neighbours[ind].NWcorn;
			x.secondcor = Point2L(Neighbours[ind].NWcorn.x, Neighbours[ind].SEcorn.y);
			break;
		}

		}

		bool existing = false;
		int closest_index = -1;
		int closest_value = -1;
		bool side; // 0 - smaller, 1 - bigger

		struct filled
		{
			int smaller;
			int bigger;
		} filled;

		filled.bigger = -1;
		filled.smaller = -1;

		if (x.firstcor.x == x.secondcor.x)
		{
			for (int i = 0; i < n_Streets; i++)
			{
				if (Streets[i].firstcor.x == Streets[i].secondcor.x && Streets[i].firstcor.x == x.firstcor.x)
				{

					if (Streets[i].firstcor.y < filled.smaller || filled.smaller == -1)
					{
						filled.smaller = Streets[i].firstcor.y;
						if (filled.smaller > x.firstcor.y)
						{
							closest_index = i;
							side = 0;
						}
					}

					if (Streets[i].secondcor.y > filled.bigger || filled.bigger == -1)
					{
						filled.bigger = Streets[i].secondcor.y;
						if (filled.bigger < x.secondcor.y)
						{
							closest_index = i;
							side = 1;
						}
					}

					if (x.firstcor.y >= filled.smaller && x.secondcor.y <= filled.bigger)
					{
						existing = true;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < n_Streets; i++)
			{
				if (Streets[i].firstcor.y == Streets[i].secondcor.y && Streets[i].firstcor.y == x.firstcor.y)
				{

					if (Streets[i].firstcor.x < filled.smaller || filled.smaller == -1)
					{
						filled.smaller = Streets[i].firstcor.x;
						if (filled.smaller > x.firstcor.x)
						{
							closest_index = i;
							side = 0;
						}
					}

					if (Streets[i].secondcor.x > filled.bigger || filled.bigger == -1)
					{
						filled.bigger = Streets[i].secondcor.x;
						if (filled.bigger < x.secondcor.x)
						{
							closest_index = i;
							side = 1;
						}
					}

					if (x.secondcor.x <= filled.bigger && x.firstcor.x >= filled.smaller)
					{
						existing = true;
					}
				}
			}
		}

		if (existing == false && closest_index != -1)
		{
			if (j == 0 || j == 2)
			{
				if (side == 1)
				{
					AddStreet(Streets[closest_index].secondcor, x.secondcor);
				}
				else
				{
					AddStreet(x.firstcor, Streets[closest_index].firstcor);
				}
			}
			else
			{
				if (side == 1)
				{
					AddStreet(Streets[closest_index].secondcor, x.secondcor);
				}
				else
				{
					AddStreet(x.firstcor, Streets[closest_index].firstcor);
				}
			}
		}
	}
}

void Cityclass::RemoveDoubles()
{
	int points[4];
	bool check_again;
	do
	{
		check_again = false;
		for (int i = 0; i < n_Streets; i++)
		{
			for (int j = 0; j < n_Streets; j++)
			{
				if (i == j)
					continue;
				if (Streets[i].firstcor.x == Streets[i].secondcor.x && Streets[j].firstcor.x == Streets[j].secondcor.x && Streets[i].firstcor.x == Streets[j].firstcor.x)
				{
					if (Streets[i].firstcor.y < Streets[j].secondcor.y && Streets[i].secondcor.y > Streets[j].firstcor.y)
					{
						if (Streets[i].secondcor.y == Streets[j].secondcor.y)
						{
							points[2] = Streets[j].secondcor.y;
							if (Streets[i].firstcor.y > Streets[j].firstcor.y)
							{
								points[0] = Streets[j].firstcor.y;
								points[1] = Streets[i].firstcor.y;
							}
							else
							{
								points[0] = Streets[i].firstcor.y;
								points[1] = Streets[j].firstcor.y;
							}
						}
						else if (Streets[i].firstcor.y == Streets[j].firstcor.y)
						{
							points[0] = Streets[i].firstcor.y;
							if (Streets[i].secondcor.y > Streets[j].secondcor.y)
							{
								points[1] = Streets[j].secondcor.y;
								points[2] = Streets[i].secondcor.y;
							}
							else
							{
								points[1] = Streets[i].secondcor.y;
								points[2] = Streets[j].secondcor.y;
							}
						}
						else
						{
							points[0] = Streets[i].firstcor.y;
							points[1] = Streets[j].firstcor.y;
							points[2] = Streets[i].secondcor.y;
							points[3] = Streets[j].secondcor.y;

							if (Streets[i].firstcor.y < points[0])
								points[0] = Streets[i].firstcor.y;
							if (Streets[j].firstcor.y < points[0])
								points[0] = Streets[j].firstcor.y;
							if (Streets[i].secondcor.y < points[0])
								points[0] = Streets[i].secondcor.y;
							if (Streets[j].secondcor.y < points[0])
								points[0] = Streets[j].secondcor.y;
							
							for (int k = 1; k < 4; k++)
							{
								if (Streets[i].firstcor.y < points[k] && Streets[i].firstcor.y > points[k - 1])
									points[k] = Streets[i].firstcor.y;
								if (Streets[j].firstcor.y < points[k] && Streets[j].firstcor.y > points[k - 1])
									points[k] = Streets[j].firstcor.y;
								if (Streets[i].secondcor.y < points[k] && Streets[i].secondcor.y > points[k - 1])
									points[k] = Streets[i].secondcor.y;
								if (Streets[j].secondcor.y < points[k] && Streets[j].secondcor.y > points[k - 1])
									points[k] = Streets[j].secondcor.y;
							}

							AddStreet(Point2L(Streets[i].firstcor.x, points[2]), Point2L(Streets[i].firstcor.x, points[3]));
						}
						Streets[i].firstcor.y = points[0];
						Streets[i].secondcor.y = points[1];
						Streets[j].firstcor.y = points[1];
						Streets[j].secondcor.y = points[2];
						check_again = true;
					}
				}
				else if (Streets[i].firstcor.y == Streets[i].secondcor.y && Streets[j].firstcor.y == Streets[j].secondcor.y && Streets[i].firstcor.y == Streets[j].firstcor.y)
				{
					if (Streets[i].firstcor.x < Streets[j].secondcor.x && Streets[i].secondcor.x > Streets[j].firstcor.x)
					{
						if (Streets[i].secondcor.x == Streets[j].secondcor.x)
						{
							points[2] = Streets[j].secondcor.x;
							if (Streets[i].firstcor.x > Streets[j].firstcor.x)
							{
								points[0] = Streets[j].firstcor.x;
								points[1] = Streets[i].firstcor.x;
							}
							else
							{
								points[0] = Streets[i].firstcor.x;
								points[1] = Streets[j].firstcor.x;
							}
						}
						else if (Streets[i].firstcor.x == Streets[j].firstcor.x)
						{
							points[0] = Streets[i].firstcor.x;
							if (Streets[i].secondcor.x > Streets[j].secondcor.x)
							{
								points[1] = Streets[j].secondcor.x;
								points[2] = Streets[i].secondcor.x;
							}
							else
							{
								points[1] = Streets[i].secondcor.x;
								points[2] = Streets[j].secondcor.x;
							}
						}
						else
						{
							points[0] = Streets[i].firstcor.x;
							points[1] = Streets[j].firstcor.x;
							points[2] = Streets[i].secondcor.x;
							points[3] = Streets[j].secondcor.x;
							if (Streets[i].firstcor.x < points[0])
								points[0] = Streets[i].firstcor.x;
							if (Streets[j].firstcor.x < points[0])
								points[0] = Streets[j].firstcor.x;
							if (Streets[i].secondcor.x < points[0])
								points[0] = Streets[i].secondcor.x;
							if (Streets[j].secondcor.x < points[0])
								points[0] = Streets[j].secondcor.x;

							for (int k = 1; k < 4; k++)
							{
								if (Streets[i].firstcor.x < points[k] && Streets[i].firstcor.x > points[k - 1])
									points[k] = Streets[i].firstcor.x;
								if (Streets[j].firstcor.x < points[k] && Streets[j].firstcor.x > points[k - 1])
									points[k] = Streets[j].firstcor.x;
								if (Streets[i].secondcor.x < points[k] && Streets[i].secondcor.x > points[k - 1])
									points[k] = Streets[i].secondcor.x;
								if (Streets[j].secondcor.x < points[k] && Streets[j].secondcor.x > points[k - 1])
									points[k] = Streets[j].secondcor.x;
							}
							AddStreet(Point2L(points[2], Streets[i].firstcor.y), Point2L(points[3], Streets[i].firstcor.y));
						}
						Streets[i].firstcor.x = points[0];
						Streets[i].secondcor.x = points[1];
						Streets[j].firstcor.x = points[1];
						Streets[j].secondcor.x = points[2];
						check_again = true;
					}
				}
			}
		}
	}
	while (check_again);
}

void Cityclass::CalculatePosition(D2D1_POINT_2L & input, D2D1_SIZE_F building_size, D2D1_POINT_2L input_index)
{
	bool check_again;
	bool primary_change = true;
	bool *b_streets = new bool[n_Streets];
	for (int i = 0; i < n_Streets; i++)
		b_streets[i] = false;

	do
	{
		check_again = false;
		if (input_index.x > 0)
		{
			if (input_index.y > 0)
			{
				if (primary_change)
				{
					input.x += 50;
					input.y -= 50;
					input.x += (input_index.x * building_size.width) - (building_size.width / 2);
					input.y -= (input_index.y * building_size.height) - (building_size.height / 2);
					primary_change = false;
				}
				for (int i = 0; i < n_Streets; i++)
				{
					if (Streets[i].firstcor.x > Center.x)
					{
						if (Streets[i].firstcor.y < Center.y)
						{
							if (Streets[i].firstcor.x == Streets[i].secondcor.x)
							{
								if (Streets[i].firstcor.x <= input.x)
								{
									if (input.y > Streets[i].firstcor.y && input.y <= Streets[i].secondcor.y)
									{
										if (!b_streets[i])
										{
											input.x += 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.x -= 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.x -= 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
							else
							{
								if (Streets[i].firstcor.y >= input.y)
								{
									if (input.x > Streets[i].firstcor.x && input.x <= Streets[i].secondcor.x)
									{
										if (!b_streets[i])
										{
											input.y -= 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.y += 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.y += 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				if (primary_change)
				{
					input.x += 50;
					input.y += 50;
					input.x += (input_index.x * building_size.width) - (building_size.width / 2);
					input.y -= (input_index.y * building_size.height) + (building_size.width / 2);
					primary_change = false;
				}
				for (int i = 0; i < n_Streets; i++)
				{
					if (Streets[i].firstcor.x > Center.x)
					{
						if (Streets[i].firstcor.y > Center.y)
						{
							if (Streets[i].firstcor.x == Streets[i].secondcor.x)
							{
								if (Streets[i].firstcor.x <= input.x)
								{
									if (input.y > Streets[i].firstcor.y && input.y <= Streets[i].secondcor.y)
									{
										if (!b_streets[i])
										{
											input.x += 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.x -= 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.x -= 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
							else
							{
								if (Streets[i].firstcor.y <= input.y)
								{
									if (input.x > Streets[i].firstcor.x && input.x <= Streets[i].secondcor.x)
									{
										if (!b_streets[i])
										{
											input.y += 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.y -= 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.y -= 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			if (input_index.y > 0)
			{
				if (primary_change)
				{
					input.x -= 50;
					input.y -= 50;
					input.x += (input_index.x * building_size.width) + (building_size.width / 2);
					input.y -= (input_index.y * building_size.height) - (building_size.width / 2);
					primary_change = false;
				}
				for (int i = 0; i < n_Streets; i++)
				{
					if (Streets[i].firstcor.x < Center.x)
					{
						if (Streets[i].firstcor.y < Center.y)
						{
							if (Streets[i].firstcor.x == Streets[i].secondcor.x)
							{
								if (Streets[i].firstcor.x >= input.x)
								{
									if (input.y > Streets[i].firstcor.y && input.y <= Streets[i].secondcor.y)
									{
										if (!b_streets[i])
										{
											input.x -= 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.x += 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.x += 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
							else
							{
								if (Streets[i].firstcor.y >= input.y)
								{
									if (input.x > Streets[i].firstcor.x && input.x <= Streets[i].secondcor.x)
									{
										if (!b_streets[i])
										{
											input.y -= 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.y += 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.y += 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				if (primary_change)
				{
					input.x -= 50;
					input.y += 50;
					input.x += (input_index.x * building_size.width) + (building_size.width / 2);
					input.y -= (input_index.y * building_size.height) + (building_size.height / 2);
					primary_change = false;
				}
				for (int i = 0; i < n_Streets; i++)
				{
					if (Streets[i].firstcor.x < Center.x)
					{
						if (Streets[i].firstcor.y > Center.y)
						{
							if (Streets[i].firstcor.x == Streets[i].secondcor.x)
							{
								if (Streets[i].firstcor.x >= input.x)
								{
									if (input.y > Streets[i].firstcor.y && input.y <= Streets[i].secondcor.y)
									{
										if (!b_streets[i])
										{
											input.x -= 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.x += 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.x += 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
							else
							{
								if (Streets[i].firstcor.y <= input.y)
								{
									if (input.x > Streets[i].firstcor.x && input.x <= Streets[i].secondcor.x)
									{
										if (!b_streets[i])
										{
											input.y += 60;
											b_streets[i] = true;
											check_again = true;
										}
									}
									else
									{
										if (b_streets[i])
										{
											input.y -= 60;
											b_streets[i] = false;
											check_again = true;
										}
									}
								}
								else
								{
									if (b_streets[i])
									{
										input.y -= 60;
										b_streets[i] = false;
										check_again = true;
									}
								}
							}
						}
					}
				}
			}
		}
	} while (check_again);
	if (n_Streets)
		delete[] b_streets;
	return;
}

bool Cityclass::CheckSmallestNeighbour(D2D1_POINT_2L input_index, D2D1_SIZE_F building_size)
{
	D2D1_POINT_2L position;
	D2D1_POINT_2L nwcorner, secorner;
	Street S1, S2, S3, S4;
	position.x = Center.x;
	position.y = Center.y;
	CalculatePosition(position, building_size, input_index);

	if (input_index.x > 0)
	{
		if (input_index.y > 0)
		{
			D2D1_POINT_2L existing_corner;
			existing_corner.x = position.x - 30 - (building_size.width / 2);
			existing_corner.y = position.y + 30 + (building_size.height / 2);

			nwcorner.x = existing_corner.x;
			secorner.y = existing_corner.y;
			nwcorner.y = existing_corner.y - 60 - (2 * building_size.height);
			secorner.x = existing_corner.x + 60 + (2 * building_size.width);

			S1.firstcor = nwcorner;
			S1.secondcor = Point2L(secorner.x, nwcorner.y);
			S2.firstcor = nwcorner;
			S2.secondcor = Point2L(nwcorner.x, secorner.y);
			S3.firstcor = Point2L(nwcorner.x, secorner.y);
			S3.secondcor = secorner;
			S4.firstcor = Point2L(secorner.x, nwcorner.y);
			S4.secondcor = secorner;
		}
		else
		{
			D2D1_POINT_2L existing_corner;
			existing_corner.x = (LONG)(position.x - 30 - (building_size.height / 2));
			existing_corner.y = (LONG)(position.y - 30 - (building_size.height / 2));
			nwcorner = existing_corner;
			secorner.y = (LONG)(existing_corner.y + 60 + (2 * building_size.height));
			secorner.x = (LONG)(existing_corner.x + 60 + (2 * building_size.width));

			S1.firstcor = nwcorner;
			S1.secondcor = Point2L(secorner.x, nwcorner.y);
			S2.firstcor = nwcorner;
			S2.secondcor = Point2L(nwcorner.x, secorner.y);
			S3.firstcor = Point2L(nwcorner.x, secorner.y);
			S3.secondcor = secorner;
			S4.firstcor = Point2L(secorner.x, nwcorner.y);
			S4.secondcor = secorner;
		}
	}
	else
	{
		if (input_index.y > 0)
		{
			D2D1_POINT_2L existing_corner;
			existing_corner.x = position.x + 30 + (building_size.height / 2);
			existing_corner.y = position.y + 30 + (building_size.height / 2);
			secorner = existing_corner;
			nwcorner.y = existing_corner.y - 60 - (2 * building_size.height);
			nwcorner.x = existing_corner.x - 60 - (2 * building_size.width);

			S1.firstcor = nwcorner;
			S1.secondcor = Point2L(secorner.x, nwcorner.y);
			S2.firstcor = nwcorner;
			S2.secondcor = Point2L(nwcorner.x, secorner.y);
			S3.firstcor = Point2L(nwcorner.x, secorner.y);
			S3.secondcor = secorner;
			S4.firstcor = Point2L(secorner.x, nwcorner.y);
			S4.secondcor = secorner;
		}
		else
		{
			D2D1_POINT_2L existing_corner;
			existing_corner.x = position.x + 30 + (building_size.height / 2);
			existing_corner.y = position.y - 30 - (building_size.height / 2);
			nwcorner.y = existing_corner.y;
			secorner.x = existing_corner.x;
			secorner.y = existing_corner.y + 60 + (2 * building_size.height);
			nwcorner.x = existing_corner.x - 60 - (2 * building_size.width);

			S1.firstcor = nwcorner;
			S1.secondcor = Point2L(secorner.x, nwcorner.y);
			S2.firstcor = nwcorner;
			S2.secondcor = Point2L(nwcorner.x, secorner.y);
			S3.firstcor = Point2L(nwcorner.x, secorner.y);
			S3.secondcor = secorner;
			S4.firstcor = Point2L(secorner.x, nwcorner.y);
			S4.secondcor = secorner;
		}
	}
	if (IsCrossing(S1) || IsCrossing(S2) || IsCrossing(S3) || IsCrossing(S4))
		return false;

	return true;
}

bool Cityclass::Checkisneighbourcorrect(D2D1_POINT_2L nw, D2D1_POINT_2L se, D2D1_SIZE_F building_size)
{
	bool is_empty_space_between_neighbours;

	for (int i = 0; i < n_Neighbours; i++)
	{
		if ((Neighbours[i].NWcorn.x - Center.x) * (nw.x - Center.x) < 0)
			continue;
		if ((Neighbours[i].NWcorn.y - Center.y) * (nw.y - Center.y) < 0)
			continue;
		if (Neighbours[i].NWcorn.x < se.x && nw.x < Neighbours[i].SEcorn.x)
		{
			int distance = 2 * building_size.height + size_of_small_road;
			if (Neighbours[i].NWcorn.y > nw.y)
			{
				is_empty_space_between_neighbours = true;
				float smaller_corner = -1;
				float bigger_corner = -1;
				for (int j = 0; j < n_Neighbours; j++)
				{
					if (i == j)
						continue;
					if (smaller_corner == -1)
					{
						if (Neighbours[i].NWcorn.x <= Neighbours[j].SEcorn.x && Neighbours[i].SEcorn.x >= Neighbours[j].NWcorn.x && Neighbours[i].NWcorn.y >= Neighbours[j].SEcorn.y)
						{
							smaller_corner = (float)Neighbours[j].NWcorn.x;
						}
					}
					else
					{
						if (Neighbours[j].NWcorn.x < smaller_corner && Neighbours[j].SEcorn.x >= Neighbours[i].NWcorn.x && Neighbours[i].NWcorn.y >= Neighbours[j].SEcorn.y)
						{
							smaller_corner = (float)Neighbours[j].NWcorn.x;
						}
					}


					if (bigger_corner == -1)
					{
						if (Neighbours[i].NWcorn.x <= Neighbours[j].SEcorn.x && Neighbours[i].SEcorn.x >= Neighbours[j].NWcorn.x && Neighbours[i].NWcorn.y >= Neighbours[j].SEcorn.y)
						{
							bigger_corner = (float)Neighbours[j].SEcorn.x;
						}
					}
					else
					{
						if (Neighbours[j].SEcorn.x > bigger_corner && Neighbours[j].NWcorn.x < Neighbours[i].SEcorn.x && Neighbours[i].NWcorn.y >= Neighbours[j].SEcorn.y)
						{
							bigger_corner = (float)Neighbours[j].SEcorn.x;
						}
					}

					if (smaller_corner <= nw.x && bigger_corner >= se.x)
					{
						is_empty_space_between_neighbours = false;
						break;
					}
				}
				if (is_empty_space_between_neighbours && (Neighbours[i].NWcorn.y - se.y) % distance)
					return false;
			}
			else
			{
				is_empty_space_between_neighbours = true;
				float smaller_corner = -1;
				float bigger_corner = -1;
				for (int j = 0; j < n_Neighbours; j++)
				{
					if (i == j)
						continue;
					if (smaller_corner == -1)
					{
						if (Neighbours[i].NWcorn.x <= Neighbours[j].SEcorn.x && Neighbours[i].SEcorn.x >= Neighbours[j].NWcorn.x && Neighbours[i].SEcorn.y <= Neighbours[j].NWcorn.y)
						{
							smaller_corner = Neighbours[j].NWcorn.x;
						}
					}
					else
					{
						if (Neighbours[j].NWcorn.x < smaller_corner && Neighbours[j].SEcorn.x >= Neighbours[i].NWcorn.x && Neighbours[i].SEcorn.y <= Neighbours[j].NWcorn.y)
						{
							smaller_corner = Neighbours[j].NWcorn.x;
						}
					}


					if (bigger_corner == -1)
					{
						if (Neighbours[i].NWcorn.x <= Neighbours[j].SEcorn.x && Neighbours[i].SEcorn.x >= Neighbours[j].NWcorn.x && Neighbours[i].SEcorn.y <= Neighbours[j].NWcorn.y)
						{
							bigger_corner = Neighbours[j].SEcorn.x;
						}
					}
					else
					{
						if (Neighbours[j].SEcorn.x > bigger_corner && Neighbours[j].NWcorn.x < Neighbours[i].SEcorn.x && Neighbours[i].SEcorn.y <= Neighbours[j].NWcorn.y)
						{
							bigger_corner = Neighbours[j].SEcorn.x;
						}
					}
					if (smaller_corner <= nw.x && bigger_corner >= se.x)
					{
						is_empty_space_between_neighbours = false;
						break;
					}
				}
				if (is_empty_space_between_neighbours && (nw.y - Neighbours[i].SEcorn.y) % distance)
					return false;
			}
		}
		else if (Neighbours[i].NWcorn.y < se.y && nw.y < Neighbours[i].SEcorn.y)
		{
			int distance = 2 * building_size.width + size_of_small_road;
			if (Neighbours[i].NWcorn.x > nw.x)
			{
				is_empty_space_between_neighbours = true;
				float smaller_corner = -1;
				float bigger_corner = -1;
				for (int j = 0; j < n_Neighbours; j++)
				{
					if (i == j)
						continue;
					if (smaller_corner == -1)
					{
						if (Neighbours[i].NWcorn.y <= Neighbours[j].SEcorn.y && Neighbours[i].SEcorn.y >= Neighbours[j].NWcorn.y && Neighbours[i].NWcorn.x >= Neighbours[j].SEcorn.x)
						{
							smaller_corner = Neighbours[j].NWcorn.y;
						}
					}
					else
					{
						if (Neighbours[j].NWcorn.y < smaller_corner && Neighbours[j].SEcorn.y >= Neighbours[i].NWcorn.y && Neighbours[i].NWcorn.x >= Neighbours[j].SEcorn.x)
						{
							smaller_corner = Neighbours[j].NWcorn.y;
						}
					}


					if (bigger_corner == -1)
					{
						if (Neighbours[i].NWcorn.y <= Neighbours[j].SEcorn.y && Neighbours[i].SEcorn.y >= Neighbours[j].NWcorn.y && Neighbours[i].NWcorn.x >= Neighbours[j].SEcorn.x)
						{
							bigger_corner = Neighbours[j].SEcorn.y;
						}
					}
					else
					{
						if (Neighbours[j].SEcorn.y > bigger_corner && Neighbours[j].NWcorn.y <= Neighbours[i].SEcorn.y && Neighbours[i].NWcorn.x >= Neighbours[j].SEcorn.x)
						{
							bigger_corner = Neighbours[j].SEcorn.y;
						}
					}
					if (smaller_corner <= nw.y && bigger_corner >= se.y)
					{
						is_empty_space_between_neighbours = false;
						break;
					}
				}
				if (is_empty_space_between_neighbours && (Neighbours[i].NWcorn.x - se.x) % distance)
					return false;
			}
			else
			{
				is_empty_space_between_neighbours = true;
				float smaller_corner = -1;
				float bigger_corner = -1;
				for (int j = 0; j < n_Neighbours; j++)
				{
					if (i == j)
						continue;
					if (smaller_corner == -1)
					{
						if (Neighbours[i].NWcorn.y <= Neighbours[j].SEcorn.y && Neighbours[i].SEcorn.y >= Neighbours[j].NWcorn.y && Neighbours[i].SEcorn.x <= Neighbours[j].NWcorn.x)
						{
							smaller_corner = Neighbours[j].NWcorn.y;
						}
					}
					else
					{
						if (Neighbours[j].NWcorn.y < smaller_corner && Neighbours[j].SEcorn.y >= Neighbours[i].NWcorn.y && Neighbours[i].SEcorn.x <= Neighbours[j].NWcorn.x)
						{
							smaller_corner = Neighbours[j].NWcorn.y;
						}
					}


					if (bigger_corner == -1)
					{
						if (Neighbours[i].NWcorn.y <= Neighbours[j].SEcorn.y && Neighbours[i].SEcorn.y >= Neighbours[j].NWcorn.y && Neighbours[i].SEcorn.x <= Neighbours[j].NWcorn.x)
						{
							bigger_corner = Neighbours[j].SEcorn.y;
						}
					}
					else
					{
						if (Neighbours[j].SEcorn.y > bigger_corner && Neighbours[j].NWcorn.y < Neighbours[i].SEcorn.y && Neighbours[i].SEcorn.x <= Neighbours[j].NWcorn.x)
						{
							bigger_corner = Neighbours[j].SEcorn.y;
						}
					}
					if (smaller_corner <= nw.y && bigger_corner >= se.y)
					{
						is_empty_space_between_neighbours = false;
						break;
					}
				}
				if (is_empty_space_between_neighbours && (nw.x - Neighbours[i].SEcorn.x) % distance)
					return false;
			}
		}
	}
	return true;
}

void Cityclass::AddFactory(FactoryClass * input, D2D1_POINT_2L index)
{
	int chosen;
	int posibilites = 0;
	D2D1_POINT_2L chosen_index;
	int range = 1;

	do
	{
		for (int i = 0; i < range; i++)
		{
			if (i + index.x == 0 || range - i + index.y == 0 || i - range + index.y == 0 || -i + index.x == 0)
				continue;

			if (!LookForByIndex(Point2L(i + index.x, range - i + index.y)))
				posibilites++;
			if (!LookForByIndex(Point2L(i + index.x, i - range + index.y)))
				posibilites++;
			if (!LookForByIndex(Point2L(-i + index.x, range - i + index.y)))
				posibilites++;
			if (!LookForByIndex(Point2L(-i + index.x, i - range + index.y)))
				posibilites++;
		}
		if (posibilites == 0)
		{
			range++;
		}
		else
		{
			break;
		}
	} while (1);
	chosen = rand() % posibilites;
	chosen++;

	posibilites = 0;
	do
	{
		for (int i = 0; i < range; i++)
		{
			if (i + index.x == 0 || range - i + index.y == 0 || i - range + index.y == 0 || -i + index.x == 0)
				continue;

			if (!LookForByIndex(Point2L(i + index.x, range - i + index.y)))
				posibilites++;
			if (posibilites == chosen)
			{
				chosen_index = Point2L(i + index.x, range - i + index.y);
				break;
			}

			if (!LookForByIndex(Point2L(i + index.x, i - range + index.y)))
				posibilites++;
			if (posibilites == chosen)
			{
				chosen_index = Point2L(i + index.x, i - range + index.y);
				break;
			}

			if (!LookForByIndex(Point2L(-i + index.x, range - i + index.y)))
				posibilites++;
			if (posibilites == chosen)
			{
				chosen_index = Point2L(-i + index.x, range - i + index.y);
				break;
			}

			if (!LookForByIndex(Point2L(-i + index.x, i - range + index.y)))
				posibilites++;
			if (posibilites == chosen)
			{
				chosen_index = Point2L(-i + index.x, i - range + index.y);
				break;
			}

		}
	} while (posibilites != chosen);

	D2D1_POINT_2L Building_pos;
	Building_pos.x = Center.x;
	Building_pos.y = Center.y;

	CalculatePosition(Building_pos, input->GetSize(), chosen_index);

	bool inNeighbour = false;
	for (int i = 0; i < n_Neighbours; i++)
	{
		if (Neighbours[i].NWcorn.x < Building_pos.x && Neighbours[i].SEcorn.x > Building_pos.x)
		{
			if (Neighbours[i].NWcorn.y < Building_pos.y && Neighbours[i].SEcorn.y > Building_pos.y)
			{
				inNeighbour = true;
			}
		}
	}

	if (inNeighbour == false)
	{
		D2D1_POINT_2L nwcorner, secorner;
		bool isNeighbourCorrect;
		Street S1, S2, S3, S4;

		if (chosen_index.x > 0)
		{
			if (chosen_index.y > 0)
			{
				do
				{
					D2D1_POINT_2L existing_corner;
					existing_corner.x = Building_pos.x - 30 - (input->GetSize().width / 2);
					existing_corner.y = Building_pos.y + 30 + (input->GetSize().height / 2);
					bool vertical_azimuth_of_neighbour = rand() % 2; // 0 - horizontal, 1 - vertical
					int n = 1;
					while (rand() % 101 > 30 && n < 4)
					{
						n++;
					}
					nwcorner.x = existing_corner.x;
					secorner.y = existing_corner.y;
					if (vertical_azimuth_of_neighbour)
					{
						nwcorner.y = existing_corner.y - 60 - (n * 2 * input->GetSize().height);
						secorner.x = existing_corner.x + 60 + (2 * input->GetSize().width);
					}
					else
					{
						nwcorner.y = existing_corner.y - 60 - (2 * input->GetSize().height);
						secorner.x = existing_corner.x + 60 + (n * 2 * input->GetSize().width);
					}

					S1.firstcor = nwcorner;
					S1.secondcor = Point2L(secorner.x, nwcorner.y);
					S2.firstcor = nwcorner;
					S2.secondcor = Point2L(nwcorner.x, secorner.y);
					S3.firstcor = Point2L(nwcorner.x, secorner.y);
					S3.secondcor = secorner;
					S4.firstcor = Point2L(secorner.x, nwcorner.y);
					S4.secondcor = secorner;
					isNeighbourCorrect = Checkisneighbourcorrect(nwcorner, secorner, input->GetSize());
				}
				while (IsCrossing(S1) || IsCrossing(S2) || IsCrossing(S3) || IsCrossing(S4) || !isNeighbourCorrect);

				AddStreet(nwcorner, Point2L(secorner.x, nwcorner.y));
				AddStreet(Point2L(secorner.x, nwcorner.y), secorner);
			}
			else
			{
				do
				{
					D2D1_POINT_2L existing_corner;
					existing_corner.x = Building_pos.x - 30 - (input->GetSize().height / 2);
					existing_corner.y = Building_pos.y - 30 - (input->GetSize().height / 2);
					bool vertical_azimuth_of_neighbour = rand() % 2; // 0 - horizontal, 1 - vertical
					int n = 1;
					while (rand() % 101 > 30 && n < 4)
					{
						n++;
					}
					nwcorner = existing_corner;
					if (vertical_azimuth_of_neighbour)
					{
						secorner.y = existing_corner.y + 60 + (n * 2 * input->GetSize().height);
						secorner.x = existing_corner.x + 60 + (2 * input->GetSize().width);
					}
					else
					{
						secorner.y = existing_corner.y + 60 + (2 * input->GetSize().height);
						secorner.x = existing_corner.x + 60 + (n * 2 * input->GetSize().width);
					}

					S1.firstcor = nwcorner;
					S1.secondcor = Point2L(secorner.x, nwcorner.y);
					S2.firstcor = nwcorner;
					S2.secondcor = Point2L(nwcorner.x, secorner.y);
					S3.firstcor = Point2L(nwcorner.x, secorner.y);
					S3.secondcor = secorner;
					S4.firstcor = Point2L(secorner.x, nwcorner.y);
					S4.secondcor = secorner;
					isNeighbourCorrect = Checkisneighbourcorrect(nwcorner, secorner, input->GetSize());
				} while (IsCrossing(S1) || IsCrossing(S2) || IsCrossing(S3) || IsCrossing(S4) || !isNeighbourCorrect);

				AddStreet(Point2L(secorner.x, nwcorner.y), secorner);
				AddStreet(Point2L(nwcorner.x, secorner.y), secorner);
			}
		}
		else
		{
			if (chosen_index.y > 0)
			{
				do
				{
					D2D1_POINT_2L existing_corner;
					existing_corner.x = Building_pos.x + 30 + (input->GetSize().height / 2);
					existing_corner.y = Building_pos.y + 30 + (input->GetSize().height / 2);
					bool vertical_azimuth_of_neighbour = (bool)(rand() % 2); // 0 - horizontal, 1 - vertical
					int n = 1;
					while (rand() % 101 > 30 && n < 4)
					{
						n++;
					}
					secorner = existing_corner;
					if (vertical_azimuth_of_neighbour)
					{
						nwcorner.y = (LONG)(existing_corner.y - 60 - (n * 2 * input->GetSize().height));
						nwcorner.x = (LONG)(existing_corner.x - 60 - (2 * input->GetSize().width));
					}
					else
					{
						nwcorner.y = (LONG)(existing_corner.y - 60 - (2 * input->GetSize().height));
						nwcorner.x = (LONG)(existing_corner.x - 60 - (n * 2 * input->GetSize().width));
					}

					S1.firstcor = nwcorner;
					S1.secondcor = Point2L(secorner.x, nwcorner.y);
					S2.firstcor = nwcorner;
					S2.secondcor = Point2L(nwcorner.x, secorner.y);
					S3.firstcor = Point2L(nwcorner.x, secorner.y);
					S3.secondcor = secorner;
					S4.firstcor = Point2L(secorner.x, nwcorner.y);
					S4.secondcor = secorner;
					isNeighbourCorrect = Checkisneighbourcorrect(nwcorner, secorner, input->GetSize());
				} while (IsCrossing(S1) || IsCrossing(S2) || IsCrossing(S3) || IsCrossing(S4) || !isNeighbourCorrect);

				AddStreet(nwcorner, Point2L(secorner.x, nwcorner.y));
				AddStreet(nwcorner, Point2L(nwcorner.x, secorner.y));
			}
			else
			{
				do
				{
					D2D1_POINT_2L existing_corner;
					existing_corner.x = (LONG)(Building_pos.x + 30 + (input->GetSize().height / 2));
					existing_corner.y = (LONG)(Building_pos.y - 30 - (input->GetSize().height / 2));
					bool vertical_azimuth_of_neighbour = (bool)(rand() % 2); // 0 - horizontal, 1 - vertical
					int n = 1;
					while (rand() % 101 > 30 && n < 4)
					{
						n++;
					}
					nwcorner.y = existing_corner.y;
					secorner.x = existing_corner.x;
					if (vertical_azimuth_of_neighbour)
					{
						secorner.y = (LONG)(existing_corner.y + 60 + (n * 2 * input->GetSize().height));
						nwcorner.x = (LONG)(existing_corner.x - 60 - (2 * input->GetSize().width));
					}
					else
					{
						secorner.y = (LONG)(existing_corner.y + 60 + (2 * input->GetSize().height));
						nwcorner.x = (LONG)(existing_corner.x - 60 - (n * 2 * input->GetSize().width));
					}

					S1.firstcor = nwcorner;
					S1.secondcor = Point2L(secorner.x, nwcorner.y);
					S2.firstcor = nwcorner;
					S2.secondcor = Point2L(nwcorner.x, secorner.y);
					S3.firstcor = Point2L(nwcorner.x, secorner.y);
					S3.secondcor = secorner;
					S4.firstcor = Point2L(secorner.x, nwcorner.y);
					S4.secondcor = secorner;
					isNeighbourCorrect = Checkisneighbourcorrect(nwcorner, secorner, input->GetSize());
				} while (IsCrossing(S1) || IsCrossing(S2) || IsCrossing(S3) || IsCrossing(S4) || !isNeighbourCorrect);

				AddStreet(Point2L(nwcorner.x, secorner.y), secorner);
				AddStreet(nwcorner, Point2L(nwcorner.x, secorner.y));
			}
		}
		AddNeighbour(nwcorner, secorner);
		CompliteRoads();
		RemoveDoubles();
	}

	input->SetPosition(Point2F((float)Building_pos.x, (float)Building_pos.y));
	input->SetIndex(chosen_index);
	FactoryClass** buffor;
	buffor = new FactoryClass*[n_Factory + 1];
	for (int i = 0; i < n_Factory; i++)
	{
		buffor[i] = Factory[i];
	}
	buffor[n_Factory] = input;
	if (n_Factory)
		delete[] Factory;
	n_Factory++;
	Factory = buffor;
}

void Cityclass::RemoveFactory(int index)
{
	FactoryClass** buffor = 0;
	int j = 0;
	if (n_Factory > 1)
	{
		buffor = new FactoryClass*[n_Factory - 1];
	}

	for (int k = 0; k < n_Factory; k++)
	{
		if (k == index)
		{
			delete (Factory[k]);
			continue;
		}
		buffor[j] = Factory[k];
		j++;
	}

	delete[] Factory;
	Factory = buffor;
	n_Factory--;
}

void Cityclass::Draw_streets(ID2D1RenderTarget * target, ID2D1Brush* brush)
{
	for (int i = 0; i < n_Streets; i++)
	{
		target->DrawLine(Point2F(Streets[i].firstcor.x, Streets[i].firstcor.y), Point2F(Streets[i].secondcor.x, Streets[i].secondcor.y), brush, size_of_small_road);
		target->DrawEllipse(Ellipse(Point2F(Streets[i].firstcor.x, Streets[i].firstcor.y), 10, 10), brush);
		target->DrawEllipse(Ellipse(Point2F(Streets[i].secondcor.x, Streets[i].secondcor.y), 20, 20), brush);
	}
	target->DrawLine(Point2F(Center.x - 5000, Center.y), Point2F(Center.x + 5000, Center.y), brush, size_of_big_road);
	target->DrawLine(Point2F(Center.x, Center.y - 5000), Point2F(Center.x, Center.y + 5000), brush, size_of_big_road);
}

FactoryClass * Cityclass::GetFactory(int index)
{
	return Factory[index];
}

int Cityclass::get_n_of_factores()
{
	return n_Factory;
}

/*void Cityclass::AddEmployee(EmployeeClass * input)
{
	EmployeeClass** buffor;
	buffor = new EmployeeClass*[n_Employees + 1];
	for (int i = 0; i < n_Employees; i++)
	{
		buffor[i] = Employees[i];
	}
	buffor[n_Employees] = input;
	if (n_Employees)
		delete[] Employees;
	n_Employees++;
	Employees = buffor;
}
*/

void Cityclass::init(D2D1_POINT_2L center)
{
	Streets = 0;
	n_Streets = 0;
	size_of_big_road = 100;
	size_of_small_road = 60;
	Factory = 0;
	n_Factory = 0;
	//Employees = 0;
	//n_Employees = 0; 
	Center = center;
	Neighbours = 0;
	n_Neighbours = 0;
}
