#ifndef _SERVICECLASS_H_
#define _SERVICECLASS_H_
#include "Factory.h"

class ServiceClass : public FactoryClass
{
private:
	void** orders;
	int n_orders;
	void add_order(void* ord);
	void delete_order(void* ord);
public:
	ServiceClass();
	double order(PFindingclass* pathmaker, Building* order);
	double prize_order(PFindingclass* pathmaker, Building* order);
	void Update(int input, PFindingclass* pathmaker);
	void EndOfMonth(float smallest_sallary, PFindingclass* pathmaker);
};

#endif