#include "Pathfinding.h"

void PFindingclass::AddVertex(D2D1_POINT_2F input)
{
	if (n_vertexes + 1 > multiplies_n_vex * 256)
	{
		D2D1_POINT_2F* subarray = new D2D1_POINT_2F[(multiplies_n_vex + 1) * 256];
		for (int i = 0; i < n_vertexes; i++)
		{
			subarray[i] = vertexes[i];
		}
		multiplies_n_vex++;
		delete[] vertexes;
		vertexes = subarray;
	}
	vertexes[n_vertexes] = input;
	n_vertexes++;
}

void PFindingclass::DeleteVertex(int index)
{
	D2D1_POINT_2F* subarray = new D2D1_POINT_2F[multiplies_n_vex * 256];
	int j = 0;
	for (int i = 0; i < n_vertexes; i++)
	{
		if (i != index)
		{
			subarray[j] = vertexes[i];
			j++;
		}
	}
	delete[] vertexes;
	n_vertexes--;
	vertexes = subarray;
}

void PFindingclass::checkvertexes()
{
	bool recheck = false;
	do
	{
		for (int i = 0; i < n_vertexes; i++)
		{
			for (int j = 0; j < n_vertexes; j++)
			{
				if (i >= j)
					continue;
				if (vertexes[i].x == vertexes[j].x && vertexes[i].y == vertexes[j].y)
				{
					recheck = true;
					DeleteVertex(j);
					DeleteVertex(i);
					goto reneval;
				}
			}
		}
	reneval:
	} while (recheck);
}

bool PFindingclass::chceckcollision(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	D2D1_POINT_2F vec;
	vec.x = second.x - first.x;
	vec.y = second.y - first.y;

	return false;
}

void PFindingclass::chceckgeom()
{
	for (int i = 0; i < n_geom; i++)
	{
		for (int j = 0; j < n_geom; j++)
		{

		}
	}
}

void PFindingclass::addgeom(D2D1_POINT_2F * vex)
{
	geometries* subarray = new geometries[n_geom + 1];
	for (int i = 0; i < n_geom; i++)
	{
		subarray[i] = geom[i];
	}

	subarray[n_geom].n_vex = 4;
	subarray[n_geom].vex = vex;

	delete[] geom;
	n_geom++;
	geom = subarray;
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
	array[0] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y);
	array[1] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y);
	array[2] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y + input->GetSize().height);
	array[3] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y + input->GetSize().height);
	addgeom(array);
	checkvertexes();
}
