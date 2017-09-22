#include "Pathfinding.h"

void PFindingclass::AddGeom(geometries input)
{
	geometries* subarray = new geometries[n_geom + 1];
	for (int i = 0; i < n_vertexes; i++)
	{
		subarray[i] = geom[i];
	}
	n_geom++;
	delete[] geom;
	geom = subarray;
	geom[n_geom] = input;
	n_geom++;
}

void PFindingclass::DeleteGeom(int index)
{
	geometries* subarray;
	if (n_geom - 1)
		subarray = new geometries[n_geom - 1];
	int j = 0;
	for (int i = 0; i < n_vertexes; i++)
	{
		if (i != index)
		{
			subarray[j] = geom[i];
			j++;
		}
		else
		{
			delete[] geom[i].vex;
		}
	}
	delete[] geom;
	n_vertexes--;
	geom = subarray;
}

bool PFindingclass::chceckcollision(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	D2D1_POINT_2F vec;
	vec.x = second.x - first.x;
	vec.y = second.y - first.y;

	return false;
}

void PFindingclass::deletevertex(int geomindex, int vexindex)
{
	D2D1_POINT_2F* vertexes;
	if (geom[geomindex].n_vex - 1)
		vertexes = new D2D1_POINT_2F[geom[geomindex].n_vex - 1];

	int j = 0;
	for (int i = 0; i < geom[geomindex].n_vex; i++)
	{
		if (vexindex == i)
			continue;
		vertexes[j] = geom[geomindex].vex[i];
	}
	delete[] geom[geomindex].vex;
	geom[geomindex].vex = vertexes;
	geom[geomindex].n_vex--;
}

void PFindingclass::addvertex(int geomindex, D2D1_POINT_2F input)
{
	D2D1_POINT_2F* vertexes = new D2D1_POINT_2F[geom[geomindex].n_vex + 1];
	for (int i = 0; i < geom[geomindex].n_vex; i++)
	{
		vertexes[i] = geom[geomindex].vex[i];
	}
	vertexes[geom[geomindex].n_vex] = input;
	delete[] geom[geomindex].vex;
	geom[geomindex].vex = vertexes;
	geom[geomindex].n_vex++;
}

void PFindingclass::connectgeom()
{
	geometries New_geometries;
	bool Recheck;
	do
	{
		Recheck = false;
		for (int i = 0; i < n_geom; i++)
		{
			for (int j = 0; j < n_geom; j++)
			{
				if (i >= j)
					continue;
				for (int k = 0; k < geom[i].n_vex; k++)
				{
					for (int l = 0; l < geom[j].n_vex; l++)
					{
						if (geom[i].vex[k].x == geom[j].vex[l].x && geom[i].vex[k].y == geom[j].vex[l].y)
						{
							geometries First_to_connect = geom[i];
							geometries Second_to_connect = geom[j];
							New_geometries.n_vex = First_to_connect.n_vex + Second_to_connect.n_vex;
							New_geometries.vex = new D2D1_POINT_2F[New_geometries.n_vex];
							int m = 0;
							for (int n = 0; n < First_to_connect.n_vex; n++)
							{
								New_geometries.vex[m] = First_to_connect.vex[n];
								m++;
							}

							for (int n = 0; n < Second_to_connect.n_vex; n++)
							{
								New_geometries.vex[m] = First_to_connect.vex[n];
								m++;
							}
							DeleteGeom(j);
							DeleteGeom(i);
							AddGeom(New_geometries);
							Recheck = true;
							goto recheck;
						}
					}
				}
			}
		}
	recheck:
	} while (Recheck);
}

void PFindingclass::checkvertexesingeom()
{
	for (int i = 0; i < n_geom; i++)
	{
		bool recheck;
		do
		{
			recheck = false;
			D2D1_POINT_2F* chcecking = geom[i].vex;
			for (int j = 0; j < geom[i].n_vex; j++)
			{
				for (int k = 0; k < geom[i].n_vex; k++)
				{
					if (j >= k)
						continue;
					if (chcecking[j].x == chcecking[k].x && chcecking[j].y == chcecking[k].y)
					{
						deletevertex(i, j);
						recheck = true;
						goto reneval;
					}
				}
			}
			reneval:
		} while (recheck);
	}
}

void PFindingclass::chceckgeom()
{
	connectgeom();
	checkvertexesingeom();
}

PFindingclass::PFindingclass()
{
	multiplies_n_vex = 1;
	vertexes = new D2D1_POINT_2F[256 * multiplies_n_vex];
	n_vertexes = 0;
	n_geom = 0;
}

void PFindingclass::AddBuilding(Building * input)
{
	D2D1_POINT_2F* array = new D2D1_POINT_2F[4];
	geometries new_geom;
	array[0] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y);
	array[1] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y);
	array[2] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y + input->GetSize().height);
	array[3] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y + input->GetSize().height);
	new_geom.vex = array;
	new_geom.n_vex = 4;
	AddGeom(new_geom);
	chceckgeom();
}
