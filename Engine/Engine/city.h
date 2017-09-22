#ifndef _CITYCLASS_H_
#define _CITYCLASS_H_
#include "Employee.h"
#include "Factory.h"

class Cityclass
{
public:
	struct Street
	{
	public:
		D2D1_POINT_2L firstcor;
		D2D1_POINT_2L secondcor;
	};
private:
	Street* Streets;
	int n_Streets;

	struct Neighbour
	{
		D2D1_POINT_2L NWcorn;
		D2D1_POINT_2L SEcorn;
	}*Neighbours;
	int n_Neighbours;

	D2D1_POINT_2L Center;
	int size_of_big_road, size_of_small_road;
	FactoryClass** Factory;
	int n_Factory;
	EmployeeClass** Employees;
	int n_Employees;
	void AddStreet(D2D1_POINT_2L firstcoord, D2D1_POINT_2L secondcoord);
	FactoryClass* LookForByIndex(D2D1_POINT_2L input);
	void AddNeighbour(D2D1_POINT_2L nwcor, D2D1_POINT_2L secor);
	bool IsCrossing(Street input);
	void CompliteRoads(int index = NULL);
	void RemoveDoubles();
	void CalculatePosition(D2D1_POINT_2L& input, D2D1_SIZE_F building_size, D2D1_POINT_2L input_index);
	bool CheckSmallestNeighbour(D2D1_POINT_2L input_index, D2D1_SIZE_F building_size);
	bool Checkisneighbourcorrect(D2D1_POINT_2L nw, D2D1_POINT_2L se, D2D1_SIZE_F building_size);
public:
	FactoryClass* GetFactory(int index);
	int get_n_of_factores();
	//void AddEmployee(EmployeeClass* input);
	void init(D2D1_POINT_2L center);
	void AddFactory(FactoryClass* input, D2D1_POINT_2L index);
	void RemoveFactory(int index);
	void Draw_streets(ID2D1RenderTarget* target, ID2D1Brush* brush);
	int get_n_streets();
	Street* get_streets();
	D2D1_POINT_2L GetCenter();
};

#endif // !_CITYCLASS_H_