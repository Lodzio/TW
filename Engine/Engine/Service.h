#ifndef _SERVICECLASS_H_
#define _SERVICECLASS_H_
#include "Factory.h"

class ServiceClass : public FactoryClass
{
private:
	void** orders;
	int n_orders;
	void Update(int input, PFindingclass* pathmaker);
	void EndOfMonth(float smallest_sallary);
public:
	ServiceClass();
	double prize_order(PFindingclass* pathmaker, Building* order);
};

#endif