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

struct pathstruct
{
public:
	friend class PFindingclass;
	D2D1_POINT_2F GetPoint();
	bool isempty();
	void Draw_streets(ID2D1RenderTarget* target, ID2D1Brush* brush);
	pathstruct operator= (pathstruct input);
	pathstruct();
	double GetRange();
private:

	struct stack
	{
		D2D1_POINT_2F point;
		stack* older;
		stack();
	};

	void delete_all();
	void Put_new_point(D2D1_POINT_2F input);
	bool is_in_stack(D2D1_POINT_2F input);
	stack* last_path;
	double range;
	D2D1_POINT_2F Newestpoint();
};

class PFindingclass
{
	struct geometries
	{
		D2D1_POINT_2F* mesh;
		D2D1_POINT_2F* vex;
		int n_vex;
		bool changed;
		geometries() : mesh(0) {}
	};
	geometries* geom;
	int n_geom;
	D2D1_POINT_2F* mesh;
	int n_mesh;
	void AddGeom(geometries input);
	void DeleteGeom(int index);
	bool chceckcollision(D2D1_POINT_2F first, D2D1_POINT_2F second);
	void deletevertex(int geomindex, int vexindex);
	void addvertex(int geomindex, D2D1_POINT_2F input);
	void connectgeom();
	void correctionofvertexingeom();
	void checkvertexesingeom(); //delete doubles
	void createmesh();
	void chceckgeom();
	void collect_mesh();
	
public:
	PFindingclass();
	void AddBuilding(Building* input);
	void DrawGeom(ID2D1RenderTarget* target, ID2D1Brush* brush);
	pathstruct* RequestPath(D2D1_POINT_2F A, D2D1_POINT_2F B);
};

#endif