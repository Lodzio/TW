#ifndef _SERVICECLASS_H_
#define _SERVICECLASS_H_
#include "Factory.h"

class ServiceClass : public FactoryClass
{
private:
	void** orders;
	int n_orders;
	void Update(int input);
	void EndOfMonth(float smallest_sallary);
public:
	ServiceClass();
};

#endif