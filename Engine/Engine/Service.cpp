#include "Service.h"
#include "Employee.h"
#include "Pathfinding.h"

void ServiceClass::Update(int input, PFindingclass* pathmaker)
{
	switch (FactoryType)
	{
	case BUILDERS:
	{


		float lowest_price = -1;
		int index = -1;
		for (int i = 0; i < *amm_Suppliers; i++)
		{
			int ammount = (*Suppliers)[i]->GetAmmountOfOutputProducts();
			if (!ammount)
				continue;
			float supp_price = (*Suppliers)[i]->GetPriceOfProducts();
			pathstruct* path = pathmaker->RequestPath(Enter, (*Suppliers)[i]->GetEnter());
			double path_lenght = path->GetRange();
			delete path;
			supp_price += (2 * (path_lenght / 200) * Salary) / ammount;
			cost_of_single_raw_material = supp_price;

			if (lowest_price == -1 || lowest_price > supp_price)
			{
				Price = supp_price + (Salary * Prod_delay / 1000.0);
				Price *= Margin;
				lowest_price = supp_price;
				index = i;
			}
		}

		int acctual_amm_of_objects = Input_object->ammount();
		if (acctual_amm_of_objects < 100)
		{
			while (acctual_amm_of_objects < 100)
			{
				double minprice = -1;
				int index;
				for (int i = 0; i < *amm_Suppliers; i++)
				{
					int ammount = (*Suppliers)[i]->GetAmmountOfOutputProducts();
					if (!ammount)
						continue;
					float supp_price = (*Suppliers)[i]->GetPriceOfProducts();
					pathstruct* path = pathmaker->RequestPath(Enter, (*Suppliers)[i]->GetEnter());
					double path_lenght = path->GetRange();
					delete path;
					supp_price += (2 * (path_lenght / 200) * Salary) / ammount;
					cost_of_single_raw_material = supp_price;

					if (minprice == -1 || minprice > supp_price)
					{
						minprice = supp_price;
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
		prize += cost_of_single_raw_material * order->How_many_supplies_needed();
		break;
	}

	default:
		prize = -1;
		break;

	}
	return prize;
}
