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
	geometries* subarray;
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

	int j = 0;
	for (int i = 0; i < geom[geomindex].n_vex; i++)
	{
		if (vexindex == i)
			continue;
		vertexes[j] = geom[geomindex].vex[i];
		j++;
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

bool NEXTVERTEX(D2D1_POINT_2F* arr, int n, int x1, int &x2)
{
	int subrange = 10000;
	if (x1 == 0)
	{
		for (int i = 1; i < n; i++)
		{
			if (arr[i].y < arr[x1].y)
			{
				x2 = -1;
				return false;
			}
			else if (arr[i].y == arr[x1].y)
			{
				if (arr[i].x < arr[x1].x)
				{
					x2 = -1;
					return false;
				}
				if (arr[i].x - arr[x1].x < subrange)
				{
					subrange = arr[i].x - arr[x1].x;
					x2 = i;
				}
			}
		}
	}
	else
	{
		D2D1_POINT_2F* copy = new D2D1_POINT_2F[n];
		D2D1_POINT_2F memory;
		int firstvex = -1;
		int current_index;
		bool* unchceck = new bool[n];
		for (int i = 0; i < n; i++)
			unchceck[i] = true;
		for (int i = 0; i < n; i++)
			copy[i] = arr[i];

		for (int i = 1; i < n; i++)
		{
			if (NEXTVERTEX(copy, n, 0, x2))
				if (x2 != -1)
				{
					firstvex = i - 1;
					break;
				}

			memory = copy[i];
			copy[i] = copy[0];
			copy[0] = memory;
		}
		delete[] copy;

		if (firstvex == -1)
			return false;
		current_index = firstvex;
		unchceck[current_index] = false;
		for (int i = 0; i < n; i++)
		{
			if (i != current_index && arr[i].y == arr[current_index].y && arr[i].x - arr[current_index].x < subrange)
			{
				subrange = arr[i].x - arr[current_index].x;
				x2 = i;
			}
		}


		do
		{
			subrange = 10000;
			current_index = x2;
			unchceck[current_index] = false;
			for (int i = 0; i < n; i++)
			{
				if (!unchceck[i])
					continue;

				if (arr[i].y == arr[current_index].y)
				{
					int range = arr[i].x - arr[current_index].x;
					range = abs(range);
					if (range < subrange)
					{
						subrange = range;
						x2 = i;
					}
				}

				if (arr[i].x == arr[current_index].x)
				{
					int range = arr[i].y - arr[current_index].y;
					range = abs(range);
					if (range < subrange)
					{
						subrange = range;
						x2 = i;
					}
				}
			}
		} while (x2 != firstvex && current_index != x1);
	}
	return true;
}

void PFindingclass::correctionofvertexingeom()
{
	for (int i = 0; i < n_geom; i++)
	{
		bool to_correct = false;
		D2D1_POINT_2F* ArrayToChceck = geom[i].vex;
		const int n_Array = geom[i].n_vex;
		int next;

		for (int j = 0; j < n_Array; j++)
		{
			if (!NEXTVERTEX(ArrayToChceck, n_Array, 0, next))
			{
				to_correct = true;
				break;
			}

			if (next != j + 1 && next != 0)
			{
				to_correct = true;
				break;
			}
		}

		if (to_correct)
		{
			geom[i].changed = true;
			D2D1_POINT_2F* copy = new D2D1_POINT_2F[n_Array];
			for (int j = 0; j < n_Array; j++)
				copy[j] = ArrayToChceck[j];

			for (int j = 0; j < n_Array; j++)
			{
				D2D1_POINT_2F memory;
				memory = copy[j];
				copy[j] = copy[0];
				copy[0] = memory;
				if (!NEXTVERTEX(copy, n_Array, 0, next))
					continue;
				if (next != -1)
				{
					break;
				}
			}

			for (int j = 0; j < n_Array - 1; j++)
			{
				if (!NEXTVERTEX(copy, n_Array, j, next))
					break;
				if (j + 1 == next)
					continue;
				D2D1_POINT_2F memory;
				memory = copy[j + 1];
				copy[j + 1] = copy[next];
				copy[next] = memory;
			}
			delete[] ArrayToChceck;
			geom[i].vex = copy;
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
						deletevertex(i, j);
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

void PFindingclass::chceckgeom()
{
	connectgeom();
	checkvertexesingeom();
	correctionofvertexingeom();
	createmesh();
	collect_mesh();
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

pathstruct pathstruct::operator=(pathstruct input)
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
