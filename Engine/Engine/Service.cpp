#include "Service.h"
#include "Employee.h"
#include "Pathfinding.h"

void ServiceClass::Update(int input)
{
	switch (FactoryType)
	{
	case BUILDERS:
	{
		int acctual_amm_of_objects = Input_object->ammount();
		if (acctual_amm_of_objects < 100)
		{
			while (acctual_amm_of_objects < 100)
			{
				double minprice = -1;
				int index;
				for (int i = 0; i < *amm_Suppliers; i++)
				{
					if ((*Suppliers)[i]->GetAmmountOfOutputProducts())
						if (minprice == -1 || minprice > (*Suppliers)[i]->GetPriceOfProducts())
						{
							minprice = (*Suppliers)[i]->GetPriceOfProducts();
							index = i;
						}
				}

				if (minprice != -1)
				{
					acctual_amm_of_objects += buy_products((*Suppliers)[index], 100);
				}
				else
				{
					if (acctual_amm_of_objects != 0)
						goto send_supplies;
				}
			}
		}
		else
		{
			send_supplies:
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
		}
		break;
	}
	}
}

void ServiceClass::EndOfMonth(float smallest_sallary)
{
	OldSoldProd = SoldProd;
	SoldProd = 0;
	switch (FactoryType)
	{
	case BUILDERS:
	{
		wanted_workers = n_orders;
		if (!Input_object->ammount())
			wanted_workers = 0;

		if (wanted_workers > n_workers)
		{
			if (Salary < smallest_sallary)
				Salary = smallest_sallary;
			else
				Salary *= 1.1;
		}
		else if (wanted_workers == n_workers)
		{
			Salary = smallest_sallary;
		}
		while (wanted_workers < n_workers)
		{
			FireEmployee();

			if (OldSoldProd == 0)
			{
				if (Margin - 0.01 >= 1.1)
				{
					Margin -= 0.01;
				}
			}
			else
			{
				Margin += 0.01;
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

double ServiceClass::prize_order(PFindingclass* pathmaker, Building* order)
{
	double prize;
	switch (FactoryType)
	{

	case BUILDERS:
	{
		pathstruct* path = pathmaker->RequestPath(Enter, ((FactoryClass*)order)->GetEnter());
		double path_lenght = path->GetRange();
		delete path;
		prize = 2 * (path_lenght / 200) * Salary;
		break;
	}

	default:
		prize = -1;
		break;

	}
	return prize;
}
