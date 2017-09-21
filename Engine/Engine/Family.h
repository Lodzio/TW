#ifndef  _FAMILYCLASS_H_
#define _FAMILYCLASS_H_
#include "Employee.h"

class FamilyClass
{
	Wallet wallet;
public:
	FamilyClass();
	void AddMember(EmployeeClass* input);
	void RemoveMember(EmployeeClass* input);
	EmployeeClass* GetEmployeebyIndex(int ind);
	int Get_n_adults_members();
	int Get_n_members();
	void End_of_month();
	Wallet* GetWallet();
	int income;
	float life_status;
private:
	EmployeeClass** Members;
	int n_members;
};
#endif // ! _FAMILYCLASS_H_