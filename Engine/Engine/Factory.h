#ifndef _FACTORYCLASS_H_
#define _FACTORYCLASS_H_
#include "building.h"

class EmployeeClass;
class Cityclass;
class PFindingclass;

class FactoryClass : public Building
{
private:
	UINT Output_object_type;
	UINT Input_object_type;
	UINT FactoryType;
	int Prod_delay; //in ms.
	int Prod_process; //in ms.
	int amm_of_in_obj;
	int amm_of_out_obj;
	float Salary;
	float Price;
	FactoryClass*** Suppliers;
	int* amm_Suppliers;
	EmployeeClass* Owner;
	int SoldProd, ProducedProd;
	int OldSoldProd, OldProducedProd;
	bool factoring;
	float Margin;
	int n_workers;
	int wanted_workers;
	EmployeeClass** Workers;
	int Prod_in_progress;
	bool not_enough_resources;
	int Max_products;
	D2D1_POINT_2F Enter;
	int buy_products(FactoryClass* fac, int wanted);
public:
	~FactoryClass();
	float GetSalary();
	FactoryClass();
	bool IsLookingForEmployee();
	D2D1_POINT_2L GetIndex();
	void SetIndex(D2D1_POINT_2L input);
	int SellProducts(int ammount);
	void Init_factory(UINT outObj, UINT inObj, int proddelay, EmployeeClass* own, UINT typ_fact, float salary, Cityclass* city, D2D1_POINT_2L index = Point2L());
	void Update(int input);
	void EndOfMonth(float smallest_sallary);
	UINT GetOutputProductsinfo();
	UINT GetInputProductsinfo();
	UINT GetFactoryType();
	int GetAmmountOfInputProducts();
	int GetAmmountOfOutputProducts();
	int GetAmmountOfMoney();
	float GetPriceOfProducts();
	float GetMargin();
	int GetAmmountOfWorkers();
	int GetAmmountOfWantedWorkers();
	EmployeeClass* GetOwner();
	void Payment(EmployeeClass* emp, int hours);
	D2D1_POINT_2L Index;
	void AddEmployee(EmployeeClass* input);
	void FireEmployee(EmployeeClass* input = NULL);
	void SetSuppler(int* num_of_supplayers, FactoryClass*** suppliers);
	void SetParameters(float margin, float salary);
	void SetEnter(Cityclass* city);
	D2D1_POINT_2F GetEnter();
	void GiveProducts(int i);
	void ServeEmployee(EmployeeClass* emp);

	enum
	{
		Null = 0,
		Wheat = 1,
		Flour = 2,
		Bread = 3,
		Iron_ore = 4,
		Steel = 5,
		cutlery = 6
	};

	enum factory_type
	{
		BAKERY = 0,
		MILL = 1,
		FARM = 2,
		WORKSHOP = 3,
		MINE = 4,
		FORGER = 5,
		BUILDERS = 6
	};
};


#endif // !