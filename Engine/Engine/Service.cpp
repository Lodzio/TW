#include "Service.h"
#include "Employee.h"

void ServiceClass::Update(int input)
{
	switch (FactoryType)
	{
	case BUILDERS:
	{
		for (int i = 0; i < n_workers; i++)
		{
			if (Workers[i]->isinfactory)
			{
				for (int j = 0; j < n_orders; j++)
				{
					if (((Building*)orders[j])->How_many_supplies_needed)
					{

					}
				}
			}
		}
		break;
	}
	}
}

ServiceClass::ServiceClass()
{
	orders = 0;
	n_orders;
}
