#ifndef _EMPLOYEECLASS_H_
#define _EMPLOYEECLASS_H_
#include "unit.h"
#include "Wallet.h"

class FactoryClass;
class FamilyClass;
class Cityclass;

class EmployeeClass : public UnitClass
{
private:

	enum
	{
		IDLE = 0,
		GO_FOR_PORODUCTS = 1
	};

	UINT employee_mode;
	bool is_in_factory;
	Wallet wallet;
	FactoryClass* Employed;
	FactoryClass*** List_of_shops;
	int* amm_of_shops;
	bool Working;
	unsigned int hours_worked;
	void Buy();
	float Hungry;
	float Cutlerys;
	FamilyClass* family;
	bool isinfamily;
	int Workless_month;
	bool adult;
	float growing;
	struct struct_borrower
	{
		EmployeeClass* owner;
		float ammount;
	};
	struct_borrower* borrower;
	int n_borrower;
	void add_borrower(EmployeeClass* input);
	void Update_borrowers();
public:
	void CalcPosition(int timeDiff);
	bool IsInDebt();
	~EmployeeClass();
	void remove_borrower(EmployeeClass* input);
	void add_debt(EmployeeClass* emp, float to_borrow);
	FamilyClass* GetFamily();
	bool IsAdult();
	void GiveAdult();
	void FindFamily(FamilyClass* input);
	int GetHungry();
	Wallet* GetWallet();
	void Init_employee();
	void SetShops(FactoryClass*** Shops, int* ammofshops);
	void Update(int input);
	void EndOfMonth();
	void Payment();
	void LookForJob(FactoryClass** list, int ammount, float cost_of_life);
	void Fire();
	bool IsInFamily();
	void Dead();
	void RemoveFromFamily();
	int MonthsWithoutJob();
	void GoToFactory(FactoryClass* Fac);
	bool isinfactory();
	void ComeBackToFactory();
};

#endif