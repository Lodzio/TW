#include "Pathfinding.h"
#define MESHDEPTH 20

void PFindingclass::AddGeom(geometries input)
{
	geometries* subarray = new geometries[n_geom + 1];
	for (int i = 0; i < n_geom; i++)
	{
		subarray[i] = geom[i];
	}
	if (n_geom)
		delete[] geom;
	geom = subarray;
	geom[n_geom].vex = input.vex;
	geom[n_geom].n_vex = input.n_vex;
	n_geom++;
}

void PFindingclass::DeleteGeom(int index)
{
	geometries* subarray = 0;
	if (n_geom - 1)
		subarray = new geometries[n_geom - 1];
	int j = 0;
	for (int i = 0; i < n_geom; i++)
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
	n_geom--;
	geom = subarray;
}

bool PFindingclass::chceckcollision(D2D1_POINT_2F first, D2D1_POINT_2F second)
{
	D2D1_POINT_2F vec;
	vec.x = second.x - first.x;
	vec.y = second.y - first.y;

	double tg_vec;
	if (vec.x != 0)
		tg_vec = vec.y / vec.x;

	double ctg_vec;
	if (vec.y != 0)
		ctg_vec = vec.x / vec.y;

	for (int i = 0; i < n_geom; i++)
	{
		D2D1_POINT_2F* geocheck = geom[i].vex;
		for (int j = 0; j < geom[i].n_vex; j++)
		{
			D2D1_POINT_2F next;
			if (j + 1 < geom[i].n_vex)
				next = geocheck[j + 1];
			else
				next = geocheck[0];

			if (geocheck[j].x == next.x)
			{
				double k = (geocheck[j].x - first.x) / vec.x;
				if (k <= 0 || k >= 1)
					continue;

				double y;
				if (vec.x != 0)
					y = (geocheck[j].x * tg_vec) - (first.x * tg_vec) + first.y;
				else
					continue;

				if (geocheck[j].y > y && next.y < y)
					return true;
				if (geocheck[j].y < y && next.y > y)
					return true;
			}
			else if (geocheck[j].y == next.y)
			{
				double k = (geocheck[j].y - first.y) / vec.y;
				if (k <= 0 || k >= 1)
					continue;

				double x;
				if (vec.y != 0)
					x = (geocheck[j].y * ctg_vec) - (first.y * ctg_vec) + first.x;
				else
					continue;

				if (geocheck[j].x > x && next.x < x)
					return true;
				if (geocheck[j].x < x && next.x > x)
					return true;
			}
		}
	}

	return false;
}

void PFindingclass::deletevertex(int geomindex, int vexindex)
{
	D2D1_POINT_2F* vertexes;            
	if (geom[geomindex].n_vex - 1)
		vertexes = new D2D1_POINT_2F[geom[geomindex].n_vex - 1];
	else
		DeleteGeom(geomindex);

	int j = 0;
	for (int i = 0; i < geom[geomindex].n_vex; i++)
	{
		if (vexindex == i)
			continue;
		vertexes[j] = geom[geomindex].vex[i];
		j++;
	}
	delete[] geom[geomindex].vex;
	if (geom[geomindex].n_vex - 1)
		geom[geomindex].vex = vertexes;
	else
		geom[geomindex].vex = NULL;
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
				geometries First_to_connect = geom[i];
				geometries Second_to_connect = geom[j];
				for (int k = 0; k < First_to_connect.n_vex; k++)
				{
					for (int l = 0; l < Second_to_connect.n_vex; l++)
					{
						if (First_to_connect.vex[k].x == Second_to_connect.vex[l].x && First_to_connect.vex[k].y == Second_to_connect.vex[l].y)
						{
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
								New_geometries.vex[m] = Second_to_connect.vex[n];
								m++;
							}
							New_geometries.changed = true;
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
		continue;
	} while (Recheck);
}

int NEXTVERTEX(D2D1_POINT_2F* arr, int length, int x)
{
	float closest_range = -1;
	int closest_index = -1;
	if (x + 1 == length)
		return 0;
	for (int i = x + 1; i < length; i++) {
		if (arr[i].x == arr[x].x && arr[i].y < arr[x].y) {
			if (closest_range == -1) {
				closest_range = arr[x].y - arr[i].y;
				closest_index = i;
			}
			else if (closest_range > arr[x].y - arr[i].y) {
				closest_range = arr[x].y - arr[i].y;
				closest_index = i;
			}
		}
	}
	if (closest_range != -1)
		return closest_index;

	for (int i = x + 1; i < length; i++) {
		if (arr[i].y == arr[x].y && arr[i].x > arr[x].x) {
			if (closest_range == -1) {
				closest_range = arr[i].x - arr[x].x;
				closest_index = i;
			}
			else if (closest_range > arr[i].x - arr[x].x) {
				closest_range = arr[i].x - arr[x].x;
				closest_index = i;
			}
		}
	}

	for (int i = x + 1; i < length; i++) {
		if (arr[i].x == arr[x].x && arr[i].y > arr[x].y) {
			if (closest_range == -1) {
				closest_range = arr[i].y - arr[x].y;
				closest_index = i;
			}
			else if (closest_range > arr[i].y - arr[x].y) {
				closest_range = arr[i].y - arr[x].y;
				closest_index = i;
			}
		}
	}
	if (closest_range != -1)
		return closest_index;

	for (int i = x + 1; i < length; i++) {
		if (arr[i].y == arr[x].y && arr[i].x < arr[x].x) {
			if (closest_range == -1) {
				closest_range = arr[x].x - arr[i].x;
				closest_index = i;
			}
			else if (closest_range > arr[x].x - arr[i].x) {
				closest_range = arr[x].x - arr[i].x;
				closest_index = i;
			}
		}
	}

	return closest_index;
}

int FINDFIRSTVERTEX(D2D1_POINT_2F* arr, int n) {
	D2D1_POINT_2F FIRST = arr[0];
	int index = 0;
	for (int i = 0; i < n; i++) {
		if (arr[i].x < FIRST.x) {
			FIRST = arr[i];
			index = i;
		}
		else if (arr[i].x == FIRST.x) {
			if (arr[i].y < FIRST.y) {
				FIRST = arr[i];
				index = i;
			}
		}
	}
	return index;
}

void PFindingclass::correctionofvertexingeom()
{
	for (int i = 0; i < n_geom; i++)
	{
		bool to_correct = false;
		D2D1_POINT_2F* ArrayToChceck = geom[i].vex;
		D2D1_POINT_2F buffor;
		const int n_Array = geom[i].n_vex;
		int first = FINDFIRSTVERTEX(geom[i].vex, geom[i].n_vex);
		
		buffor = geom[i].vex[0];
		geom[i].vex[0] = geom[i].vex[first];
		geom[i].vex[first] = buffor;

		for (int j = 0; j < geom[i].n_vex - 1; j++) {
			int next = NEXTVERTEX(geom[i].vex, geom[i].n_vex, j);
			if (next == -1)
				continue;
			buffor = geom[i].vex[j + 1];
			geom[i].vex[j + 1] = geom[i].vex[next];
			geom[i].vex[next] = buffor;
		}
	}
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
						deletevertex(i, k);
						geom[i].changed = true;
						recheck = true;
						goto reneval;
					}
				}
			}
		reneval:
			continue;
		} while (recheck);
	}
}

void PFindingclass::createmesh()
{
	for (int i = 0; i < n_geom; i++)
	{
		if (!geom[i].changed)
			continue;
		if (geom[i].mesh)
			delete[] geom[i].mesh;
		geom[i].mesh = new D2D1_POINT_2F[geom[i].n_vex];
		D2D1_POINT_2F* mesh = geom[i].mesh;
		D2D1_POINT_2F* vex = geom[i].vex;

		for (int j = 0; j < geom[i].n_vex; j++)
		{
			D2D1_POINT_2F delta;
			float LenghtVec;
			int next, last;

			if (j == 0)
				last = geom[i].n_vex - 1;
			else
				last = j - 1;

			if (j == geom[i].n_vex - 1)
				next = 0;
			else
				next = j + 1;

			LenghtVec = sqrt(pow(vex[next].x - vex[last].x, 2) + pow(vex[next].y - vex[last].y, 2));
			delta.y = -(((vex[next].x - vex[last].x) / LenghtVec) * MESHDEPTH); // y = -x
			delta.x = (((vex[next].y - vex[last].y) / LenghtVec) * MESHDEPTH); //x = y

			mesh[j].x = vex[j].x + delta.x;
			mesh[j].y = vex[j].y + delta.y;
		}
	}
}

void PFindingclass::expandGeom(int geomIndex, D2D1_POINT_2F *newBuilding, int lenght) {

	geometries Geometry = geom[geomIndex];

	int TouchedGeometryVertex[2];
	int TouchedBuildingVertex[2];
	int numOfTouchedVertexes = 0;
	int in_case_0index_in_center_building = 0;
	int in_case_0index_in_center_geometry = 0;
	for (int i = 0; i < Geometry.n_vex; i++) {
		for (int j = 0; j < lenght; j++) {
			if (Geometry.vex[i].x == newBuilding[j].x && Geometry.vex[i].y == newBuilding[j].y) {
				if (numOfTouchedVertexes > 0) {
					if (i == 1) {
						in_case_0index_in_center_building = j;
						in_case_0index_in_center_geometry = i;
					}
					else if (i == Geometry.n_vex - 1 && in_case_0index_in_center_building != 0) {
						TouchedGeometryVertex[0] = in_case_0index_in_center_geometry;
						TouchedBuildingVertex[0] = in_case_0index_in_center_building;
					}

					TouchedGeometryVertex[1] = i;
					TouchedBuildingVertex[1] = j;
				}
				else {
					TouchedGeometryVertex[0] = i;
					TouchedBuildingVertex[0] = j;
				}		
				numOfTouchedVertexes++;
			}
		}
	}

	if (TouchedGeometryVertex[1] == Geometry.n_vex - 1 && TouchedGeometryVertex[0] == 0) {
		TouchedGeometryVertex[0] = TouchedGeometryVertex[1];
		TouchedGeometryVertex[1] = 0;

		int buffor = TouchedBuildingVertex[0];
		TouchedBuildingVertex[0] = TouchedBuildingVertex[1];
		TouchedBuildingVertex[1] = buffor;
	}


	geometries New_geometries;
	int fixedSecondVertex = TouchedBuildingVertex[1];
	if (fixedSecondVertex < TouchedBuildingVertex[0])
		fixedSecondVertex += 4;
	New_geometries.n_vex = Geometry.n_vex + lenght - (2 + ((numOfTouchedVertexes - 2) * 2));// (TouchedGeometryVertex[1] - TouchedGeometryVertex[0]) + (fixedSecondVertex - TouchedBuildingVertex[0]);

	New_geometries.vex = new D2D1_POINT_2F[New_geometries.n_vex];
	int index = 0;
	for (int i = 0; i < TouchedGeometryVertex[0]; i++) {
		New_geometries.vex[index] = Geometry.vex[i];
		index++;
	}

	for (int i = TouchedBuildingVertex[0]; i != TouchedBuildingVertex[1]; i++) {
		if (i == 4)
 			i -= 4;
		if (i == TouchedBuildingVertex[1])
			break;
		New_geometries.vex[index] = newBuilding[i];
		index++;
	}
	if (index > New_geometries.n_vex)
		Sleep(1);

	for (int i = TouchedGeometryVertex[1]; i < Geometry.n_vex; i++) {
		if (TouchedGeometryVertex[1] == 0)
			break;
		New_geometries.vex[index] = Geometry.vex[i];
		index++;
	}

	if (index > New_geometries.n_vex)
		Sleep(1);
	DeleteGeom(geomIndex);
	AddGeom(New_geometries);
}

void PFindingclass::reduceGeom(int geomIndex, D2D1_POINT_2F *newBuilding, int lenght) {
	geometries Geometry = geom[geomIndex];

	int TouchedGeometryVertex[2];
	int TouchedBuildingVertex[2];
	int numOfTouchedVertexes = 0;
	if (Geometry.n_vex == 4)
		DeleteGeom(geomIndex);

	for (int i = 0; i < Geometry.n_vex; i++) {
		for (int j = 0; j < lenght; j++) {
			if (Geometry.vex[i].x == newBuilding[j].x && Geometry.vex[i].y == newBuilding[j].y) {
				if (numOfTouchedVertexes > 0) {
					TouchedGeometryVertex[1] = i;
					TouchedBuildingVertex[1] = j;
				}
				else {
					TouchedGeometryVertex[0] = i;
					TouchedBuildingVertex[0] = j;
				}
				numOfTouchedVertexes++;
			}
		}
	}

	geometries New_geometries;
	int fixedSecondVertex = TouchedBuildingVertex[1];
	if (fixedSecondVertex < TouchedBuildingVertex[0])
		fixedSecondVertex += 4;
	New_geometries.n_vex = Geometry.n_vex - ((numOfTouchedVertexes - 3) * 2);// (TouchedGeometryVertex[1] - TouchedGeometryVertex[0]) + (fixedSecondVertex - TouchedBuildingVertex[0]);

	New_geometries.vex = new D2D1_POINT_2F[New_geometries.n_vex];
	int index = 0;
	for (int i = 0; i <= TouchedGeometryVertex[0]; i++) {
		New_geometries.vex[index] = Geometry.vex[i];
		index++;
	}

	if (index > New_geometries.n_vex)
		Sleep(1);

	for (int i = TouchedGeometryVertex[1]; i < Geometry.n_vex; i++) {
		if (TouchedGeometryVertex[1] == 0)
			break;
		New_geometries.vex[index] = Geometry.vex[i];
		index++;
	}

	if (index > New_geometries.n_vex)
		Sleep(1);
	DeleteGeom(geomIndex);
	AddGeom(New_geometries);
}

int PFindingclass::getGeomIndex(D2D1_POINT_2F *newBuilding, int lenght) {
	for (int i = 0; i < n_geom; i++) {
		D2D1_POINT_2F* vertexes = geom[i].vex;
		for (int j = 0; j < lenght; j++) {
			for (int k = 0; k < geom[i].n_vex; k++) {
				if (newBuilding[j].x == vertexes[k].x && newBuilding[j].y == vertexes[k].y)
					return i;
			}
		}
	}
	return -1;
}

void PFindingclass::chceckgeom()
{
	//connectgeom();
	//checkvertexesingeom();
	//clearVertexes();
	//correctionofvertexingeom();
	/*createmesh();
	collect_mesh();*/
}

void PFindingclass::collect_mesh()
{
	if (n_mesh)
		delete[] mesh;
	n_mesh = 0;
	for (int i = 0; i < n_geom; i++)
	{
		n_mesh += geom[i].n_vex;
	}

	mesh = new D2D1_POINT_2F[n_mesh];

	int k = 0;
	for (int i = 0; i < n_geom; i++)
		for (int j = 0; j < geom[i].n_vex; j++)
		{
			mesh[k] = geom[i].mesh[j];
			k++;
		}
}

PFindingclass::PFindingclass()
{
	n_mesh = 0;
	n_geom = 0;
}

void PFindingclass::AddBuilding(Building * input)
{
	D2D1_POINT_2F* array = new D2D1_POINT_2F[4];
	array[0] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y);
	array[1] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y);
	array[3] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y + input->GetSize().height);
	array[2] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y + input->GetSize().height);

	int geometryIndex = getGeomIndex(array, 4);
	if (geometryIndex == -1) {
		geometries new_geom;
		new_geom.vex = array;
		new_geom.n_vex = 4;
		AddGeom(new_geom);
		chceckgeom();
	}
	else {
		expandGeom(geometryIndex, array, 4);
	}
}

void PFindingclass::RemoveBuilding(Building * input) {
	D2D1_POINT_2F* array = new D2D1_POINT_2F[4];
	array[0] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y);
	array[1] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y);
	array[3] = Point2F(input->GetStartCorner().x, input->GetStartCorner().y + input->GetSize().height);
	array[2] = Point2F(input->GetStartCorner().x + input->GetSize().width, input->GetStartCorner().y + input->GetSize().height);

	int geometryIndex = getGeomIndex(array, 4);
	if (geometryIndex == -1)
		Sleep(1);
	else {
		reduceGeom(geometryIndex, array, 4);
	}
}

void PFindingclass::DrawGeom(ID2D1RenderTarget * target, ID2D1Brush * brush)
{
	for (int i = 0; i < n_geom; i++)
	{
		for (int j = 0; j < geom[i].n_vex - 1; j++)
		{
			target->DrawLine(Point2F(geom[i].vex[j].x, geom[i].vex[j].y), Point2F(geom[i].vex[j + 1].x, geom[i].vex[j + 1].y), brush, 1);
			target->DrawEllipse(Ellipse(Point2F(geom[i].vex[j].x, geom[i].vex[j].y), 10, 10), brush);
			target->DrawEllipse(Ellipse(Point2F(geom[i].vex[j + 1].x, geom[i].vex[j + 1].y), 20, 20), brush);
		}
	}
}

pathstruct* PFindingclass::RequestPath(D2D1_POINT_2F A, D2D1_POINT_2F B)
{
	pathstruct* output = new pathstruct;
	pathstruct* possiblepaths = new pathstruct[256];
	int n_ppaths = 0;
	int* closedpathrange = new int[n_mesh];
	for (int i = 0; i < n_mesh; i++)
		closedpathrange[i] = 0;
	int loops_after_finded_ways;
	double smallestrange = -1;
	
	D2D1_POINT_2F actualposition;

	if (chceckcollision(A, B))
	{
		for (int i = 0; i < n_mesh; i++)
		{
			if (!chceckcollision(A, mesh[i]))
			{
				double range = sqrt(pow(mesh[i].x - A.x, 2) + pow(mesh[i].y - A.y, 2));
				if (closedpathrange[i] == 0)
				{
					possiblepaths[n_ppaths].Put_new_point(mesh[i]);
					possiblepaths[n_ppaths].range += range;
					closedpathrange[i] = possiblepaths[n_ppaths].range;
					n_ppaths++;
				}
			}
		}
		loops_after_finded_ways = 0;
		do
		{
			//cleaning paths
			int needed = n_ppaths * 0.5;
			needed = (((needed / 256) + 1) * 256);
			pathstruct* newarray = new pathstruct[needed];
			int index = 0;
			for (int i = 0; i < n_ppaths; i++)
			{
				if (possiblepaths[i].isempty())
					continue;
				newarray[index] = possiblepaths[i];
				index++;
			}
			n_ppaths = index;
			delete []possiblepaths;
			possiblepaths = newarray;
			int actual_n_paths = n_ppaths;
			bool possibilites;

			for (int i = 0; i < actual_n_paths; i++)
			{
				possibilites = false;
				if (possiblepaths[i].isempty())
					continue;
				actualposition = possiblepaths[i].Newestpoint();
				if (actualposition.x == B.x && actualposition.y == B.y)
				{
					smallestrange = possiblepaths[i].range;
					loops_after_finded_ways++;
					continue;
				}
				if (!chceckcollision(actualposition, B))
				{
					possiblepaths[i].Put_new_point(B);
					double range = sqrt(pow(B.x - actualposition.x, 2) + pow(B.y - actualposition.y, 2));
					possiblepaths[i].range += range;
					continue;
				}

				for (int j = 0; j < n_mesh; j++)
				{
					if (chceckcollision(actualposition, mesh[j]))
					{
						continue;
					}

					if (possiblepaths[i].is_in_stack(mesh[j]))
					{
						continue;
					}

					double range = sqrt(pow(mesh[j].x - actualposition.x, 2) + pow(mesh[j].y - actualposition.y, 2));
					if (closedpathrange[j] <= range + possiblepaths[i].range && closedpathrange[j] != 0)
					{
						continue;
					}
					
					possiblepaths[n_ppaths] = possiblepaths[i];
					possiblepaths[n_ppaths].Put_new_point(mesh[j]);
					possiblepaths[n_ppaths].range += range;
					closedpathrange[j] = possiblepaths[n_ppaths].range;
					n_ppaths++;
					possibilites = true;

					if (closedpathrange[j] != 0)
					for (int k = 0; k < actual_n_paths; k++)
					{
						if (i == k)
							continue;
						if (possiblepaths[k].is_in_stack(mesh[j]))
						{
							possiblepaths[k].delete_all();
						}
					}
				}
				if (!possibilites)
					possiblepaths[i].delete_all();
			}
			
			if (smallestrange != -1)
				for (int i = 0; i < n_ppaths; i++)
				{
					if (possiblepaths[i].isempty())
						continue;
					actualposition = possiblepaths[i].Newestpoint();
					if (actualposition.x == B.x && actualposition.y == B.y)
					{
						if (possiblepaths[i].range <= smallestrange)
						{
							smallestrange = possiblepaths[i].range;
							*output = possiblepaths[i];
						}
					}
				}
			if (loops_after_finded_ways)
				loops_after_finded_ways++;
		} while (loops_after_finded_ways < 3);
		for (int i = 0; i < n_ppaths; i++)
			possiblepaths[i].delete_all();
	}
	else
	{
		possiblepaths[0].Put_new_point(B);
		*output = possiblepaths[0];
		possiblepaths[0].delete_all();
	}
	delete[] closedpathrange;
	delete[] possiblepaths;
	return output;
}

D2D1_POINT_2F pathstruct::GetPoint()
{
	if (last_path == 0)
		return Point2F(-1, -1);
	D2D1_POINT_2F output = last_path->point;
	stack* old = last_path->older;
	delete last_path;
	last_path = old;
	return output;
}

pathstruct::pathstruct()
{
	last_path = 0;
	range = 0;
}

pathstruct::~pathstruct()
{
	while (last_path)
		GetPoint();
	range = 0;
}

double pathstruct::GetRange()
{
	return range;
}

void pathstruct::delete_all()
{
	while (last_path)
		GetPoint();
	range = 0;
}

void pathstruct::Put_new_point(D2D1_POINT_2F input)
{
	stack* actual;
	if (last_path)
	{
		actual = last_path;

		while (actual->older)
			actual = actual->older;

		stack* new_path = new stack;
		new_path->point = input;
		actual->older = new_path;
	}
	else
	{
		stack* new_path = new stack;
		new_path->point = input;
		last_path = new_path;
	}
}

bool pathstruct::isempty()
{
	if (last_path)
		return false;
	return true;
}

bool pathstruct::is_in_stack(D2D1_POINT_2F input)
{
	stack* actual = last_path;
	
	while (actual)
	{
		if (input.x == actual->point.x && input.y == actual->point.y)
			return true;
		actual = actual->older;
	}
	return false;
}

pathstruct & pathstruct::operator=(pathstruct & input)
{
	stack* actual;
	stack* actualcopied;
	range = input.range;

	if (input.last_path)
	{
		last_path = new stack;
		actual = last_path;
		actualcopied = input.last_path;
	}
	else
		return *this;

	while (actualcopied)
	{
		if (actualcopied->older)
			actual->older = new stack;
		actual->point = actualcopied->point;
		actual = actual->older;
		actualcopied = actualcopied->older;
	} 
	return *this;
}

D2D1_POINT_2F pathstruct::Newestpoint()
{
	stack* actual = last_path;
	while (actual->older)
	{
		actual = actual->older;
	}
	return actual->point;
}

void pathstruct::Draw_streets(ID2D1RenderTarget * target, ID2D1Brush * brush)
{
	stack* actual = last_path;
	stack* next;
	while (actual)
	{
		next = actual->older;
		if (next)
		{
			target->DrawLine(actual->point, next->point, brush, 1);
			target->DrawEllipse(Ellipse(next->point, 7, 7), brush);
		}
		target->DrawEllipse(Ellipse(actual->point, 5, 5), brush);
		actual = next;
	}
}

pathstruct::stack::stack()
{
	older = 0;
}
