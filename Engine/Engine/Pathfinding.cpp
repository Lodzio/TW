#include "Pathfinding.h"

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
	double tg_vec = vec.y / vec.x;
	double ctg_vec = vec.x / vec.y;

	for (int i = 0; i < n_geom; i++)
	{
		D2D1_POINT_2F* geocheck = geom[i].vex;
		for (int j = 0; j < geom[i].n_vex - 1; j++)
		{
			if (geocheck[j].x == geocheck[j + 1].x)
			{
				double y = (geocheck[j].x * tg_vec) - (first.x * tg_vec) + first.y;
				if (geocheck[j].y > y && geocheck[j + 1].y < y)
					return true;
				if (geocheck[j].y < y && geocheck[j + 1].y > y)
					return true;
			}
			else if (geocheck[j].y == geocheck[j + 1].y)
			{
				double x = (geocheck[j].y * ctg_vec) - (first.y * ctg_vec) + first.x;
				if (geocheck[j].x > x && geocheck[j + 1].x < x)
					return true;
				if (geocheck[j].x < x && geocheck[j + 1].x > x)
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

void PFindingclass::chceckgeom()
{
	connectgeom();
	checkvertexesingeom();
	correctionofvertexingeom();
	collect_vertices();
}

void PFindingclass::collect_vertices()
{
	if (n_vertexes)
		delete[] vertexes;
	n_vertexes = 0;
	for (int i = 0; i < n_geom; i++)
	{
		n_vertexes += geom[i].n_vex;
	}

	vertexes = new D2D1_POINT_2F[n_vertexes];

	int k = 0;
	for (int i = 0; i < n_geom; i++)
		for (int j = 0; j < geom[i].n_vex; j++)
		{
			vertexes[k] = geom[i].vex[j];
			k++;
		}
}

PFindingclass::PFindingclass()
{
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
	int* closedpathrange = new int[n_vertexes];
	for (int i = 0; i < n_vertexes; i++)
		closedpathrange[i] = 0;
	int finded_ways;
	double smallestrange = -1;
	
	D2D1_POINT_2F actualposition;

	if (chceckcollision(A, B))
	{
		for (int i = 0; i < n_vertexes; i++)
		{
			if (!chceckcollision(A, vertexes[i]))
			{
				double range = sqrt(pow(vertexes[i].x - A.x, 2) + pow(vertexes[i].y - A.y, 2));
				if (closedpathrange[i] == 0)
				{
					possiblepaths[n_ppaths].Put_new_point(vertexes[i]);
					possiblepaths[n_ppaths].range += range;
					n_ppaths++;
					closedpathrange[i] = possiblepaths[n_ppaths].range;
				}
			}
		}
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

			finded_ways = 0;
			for (int i = 0; i < n_ppaths; i++)
			{
				if (possiblepaths[i].isempty())
					continue;
				actualposition = possiblepaths[i].Newestpoint();
				if (actualposition.x == B.x && actualposition.y == B.y)
				{
					smallestrange = possiblepaths[i].range;
					continue;
				}
				for (int j = 0; j < n_vertexes; j++)
				{
					if (!possiblepaths[i].is_in_stack(vertexes[j]))
					{
						double range = sqrt(pow(vertexes[j].x - actualposition.x, 2) + pow(vertexes[j].y - actualposition.y, 2));
						if (closedpathrange[j] <= range + possiblepaths[i].range)
						{
							possiblepaths[i].delete_all();
						}
						else
						{
							possiblepaths[n_ppaths] = possiblepaths[i];
							possiblepaths[n_ppaths].Put_new_point(vertexes[j]);
							possiblepaths[n_ppaths].range += range;

							if (closedpathrange[j] != 0)
							for (int k = 0; k < n_ppaths; k++)
							{
								if (possiblepaths[k].is_in_stack(vertexes[j]))
								{
									possiblepaths[k].delete_all();
								}
							}

							closedpathrange[j] = possiblepaths[n_ppaths].range;
							possiblepaths[i].delete_all();
							n_ppaths++;
						}
					}
				}
			}
			
			if (smallestrange != -1)
				for (int i = 0; i < n_ppaths; i++)
				{
					actualposition = possiblepaths[i].Newestpoint();
					if (actualposition.x == B.x && actualposition.y == B.y)
					{
						if (possiblepaths[i].range < smallestrange)
						{
							smallestrange = possiblepaths[i].range;
							*output = possiblepaths[i];
						}
						finded_ways++;
					}
				}
		} while (finded_ways < 3);
		for (int i = 0; i < n_ppaths; i++)
			possiblepaths[i].delete_all();
	}
	else
	{
		possiblepaths[0].Put_new_point(B);
		*output = possiblepaths[0];
		possiblepaths[0].delete_all();
		delete closedpathrange;
	}
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

void pathstruct::delete_all()
{
	while (last_path)
		GetPoint();
}

void pathstruct::Put_new_point(D2D1_POINT_2F input)
{
	stack* new_path = new stack;
	new_path->point = input;
	new_path->older = last_path;
	last_path = new_path;
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
	
	do
	{
		if (input.x == actual->point.x && input.y == actual->point.y)
			return true;
	} while (actual->older != 0);
	return false;
}

pathstruct pathstruct::operator=(pathstruct input)
{
	pathstruct output;
	stack* actual = 0;
	stack* actualcopied;
	output.range = input.range;

	if (input.last_path)
	{
		output.last_path = new stack;
		output.last_path->point = input.last_path->point;
		actual = output.last_path;
		actualcopied = input.last_path;
	}
	else
		return output;

	while (actualcopied)
	{
		actual->older = new stack;
		actual->point = actualcopied->point;
		actual = actual->older;
		actualcopied = actualcopied->older;
	} 
	return output;
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
		target->DrawLine(actual->point, next->point, brush, 1);
		target->DrawEllipse(Ellipse(actual->point, 5, 5), brush);
		target->DrawEllipse(Ellipse(next->point, 7, 7), brush);
	}
}
