////////////////////////////////////////////////////////////////////////////////
// Filename: Pathfinding.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PFINDINGCLASS_H_
#define _PFINDINGCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////

#include "building.h"
#include "unit.h"

/////////////
// GLOBALS //
/////////////

class PFindingclass
{
	struct geometries
	{
		D2D1_POINT_2F* vex;
		int n_vex;
	};
	geometries* geom;
	int n_geom;
	D2D1_POINT_2F* vertexes;
	int n_vertexes;
	int multiplies_n_vex; // x256 razy.
	void AddVertex(D2D1_POINT_2F input);
	void DeleteVertex(int index);
	void checkvertexes();
	bool chceckcollision(D2D1_POINT_2F first, D2D1_POINT_2F second);
	void chceckgeom();
	void addgeom(D2D1_POINT_2F* vex);
	
public:
	PFindingclass();
	void AddBuilding(Building* input);
};

#endif