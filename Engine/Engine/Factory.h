#ifndef _FACTORYCLASS_H_
#define _FACTORYCLASS_H_
#include "building.h"
#include "InventoryObjects.h"

class EmployeeClass;
class Cityclass;
class PFindingclass;
class ServiceClass;

class FactoryClass : public Building
{
protected:
	bool upgrading;
	int level;
	int max_workers_per_level;
	void increase_level();
	InvObject* Output_object;
	InvObject* Input_object;
	UINT FactoryType;
	int Prod_delay; //in ms.
	int Prod_process; //in ms.
	float Salary;
	float Price;
	FactoryClass*** Suppliers;
	int* amm_Suppliers;
	ServiceClass*** Builders;
	int* amm_Builders;

	EmployeeClass* Owner;
	int SoldProd, ProducedProd;
	int OldSoldProd, OldProducedProd;
	bool factoring;
	float Margin;
	int n_workers;
	int wanted_workers;
	int max_workers;
	EmployeeClass** Workers;
	int Prod_in_progress;
	bool not_enough_resources;
	int Max_products;
	D2D1_POINT_2F Enter;
	int buy_products(FactoryClass* fac, int wanted);
	double cost_of_single_raw_material;
public:
	~FactoryClass();
	float GetSalary();
	FactoryClass();
	bool IsLookingForEmployee();
	D2D1_POINT_2L GetIndex();
	void SetIndex(D2D1_POINT_2L input);
	double SellProducts(int ammount, InvObject* stack);
	void Init_factory(UINT outObj, UINT inObj, int proddelay, EmployeeClass* own, UINT typ_fact, float salary, Cityclass* city, D2D1_POINT_2L index = Point2L());
	virtual void Update(int input, PFindingclass* pathmaker);
	virtual void EndOfMonth(float smallest_sallary, PFindingclass* pathmaker);
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
	void SetSuppler(int* num_of_supplayers, FactoryClass*** suppliers, int* num_of_builders, ServiceClass*** builders);
	void SetParameters(float margin, float salary);
	void SetEnter(Cityclass* city);
	void SetAmmountOfInputProd(int x);
	D2D1_POINT_2F GetEnter();
	int build(InvObject* supplies);
	void ServeEmployee(EmployeeClass* emp);

	enum factory_type
	{
		BAKERY = 0,
		MILL = 1,
		FARM = 2,
		WORKSHOP = 3,
		MINE = 4,
		FORGER = 5,
		BUILDERS = 6,
		BRICKERY = 7
	};
};


#endif // !