#include "Service.h"
#include "Employee.h"

void ServiceClass::Update(int input)
{
	switch (FactoryType)
	{
	case BUILDERS:
	{
		if (Input_object->ammount() < 100)
		{
			double minprice = -1;
			int index;
			for (int i = 0; i < *amm_Suppliers; i++)
			{
				if (minprice == -1 || minprice > (*Suppliers)[i]->GetPriceOfProducts())
				{
					minprice = (*Suppliers)[i]->GetPriceOfProducts();
					index = i;
				}
			}

			if (minprice != -1)
			{
				buy_products((*Suppliers)[index], 5);
			}
		}
		for (int j = 0; j < n_orders; j++)
		{
			if (((Building*)orders[j])->How_many_supplies_needed())
			{
				for (int i = 0; i < n_workers; i++)
				{
					if (Workers[i]->isinfactory())
					{
						Workers[i]->GoToFactory((FactoryClass*)orders[j]);
						break;
					}
				}
			}
		}
		break;
	}
	}
}

void ServiceClass::EndOfMonth(float smallest_sallary)
{
	switch (FactoryType)
	{
	case BUILDERS:
	{

		break;
	}
	}

}

ServiceClass::ServiceClass()
{
	orders = 0;
	n_orders;
}
