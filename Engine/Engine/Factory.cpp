#include "Factory.h"
#include "Employee.h"
#include "city.h"
#include "Pathfinding.h"

void FactoryClass::Init_factory(UINT outObj, UINT inObj, int proddelay, EmployeeClass* own, UINT typ_fact, float salary, Cityclass* city, D2D1_POINT_2L index)
{
	Input_object_type = inObj;
	Output_object_type = outObj;
	amm_of_in_obj = 0;
	amm_of_out_obj = 0;
	Prod_delay = proddelay;
	Prod_process = 0;
	Price = 0;
	Suppliers = 0;
	amm_Suppliers = 0;
	Salary = salary;
	Index = index;
	Owner = own;
	Margin = 1.19f;
	SoldProd = 0;
	ProducedProd = 0;
	OldSoldProd = 0;
	OldProducedProd = 0;
	n_workers = 0;
	wanted_workers = 1;
	max_workers = 1;
	Prod_in_progress = 0;
	FactoryType = typ_fact;
	not_enough_resources = false;
	Max_products = 6 * 30 * 1000 / Prod_delay;

	if (inObj == NULL)
	{
		factoring = true;
		Price = (float)(1.5 * Salary * Prod_delay / 1000.0);
	}
	else
		factoring = false;
}

int FactoryClass::buy_products(FactoryClass * fac, int wanted)
{
	int output;
	float supp_price = fac->GetPriceOfProducts();
	int ammount = fac->GetAmmountOfOutputProducts();

	if (ammount >= wanted)
	{
		if (Owner->GetWallet()->GetMoney() >= supp_price * wanted)
		{
			float bill = (float)fac->SellProducts(5);
			amm_of_in_obj += wanted;
			output = wanted;
			if (bill > Owner->GetWallet()->GetMoney())
			{
				Owner->add_debt(fac->Owner, bill);
			}
			else
			{
				Owner->GetWallet()->Give_money(bill, fac->GetOwner()->GetWallet());
			}
		}
		else
		{
			int available = Owner->GetWallet()->GetMoney() / supp_price;
			if (available <= 0)
				available = 1;
			if (available > 0)
			{
				float bill = fac->SellProducts(available);
				amm_of_in_obj += available;
				output = available;
				if (bill > Owner->GetWallet()->GetMoney())
				{
					Owner->add_debt(fac->Owner, bill);
				}
				else
				{
					Owner->GetWallet()->Give_money(bill, fac->GetOwner()->GetWallet());
				}
			}
		}
	}
	else
	{
		if (Owner->GetWallet()->GetMoney() >= supp_price * ammount)
		{
			float bill = fac->SellProducts(ammount);
			amm_of_in_obj += ammount;
			output = ammount;
			if (bill > Owner->GetWallet()->GetMoney())
			{
				Owner->add_debt(fac->Owner, bill);
			}
			else
			{
				Owner->GetWallet()->Give_money(bill, fac->GetOwner()->GetWallet());
			}
		}
		else
		{
			int available = Owner->GetWallet()->GetMoney() / supp_price;
			if (available <= 0)
				available = 1;
			if (available > 0)
			{
				float bill = fac->SellProducts(available);
				amm_of_in_obj += available;
				output = available;
				if (bill > Owner->GetWallet()->GetMoney())
				{
					Owner->add_debt(fac->Owner, bill);
				}
				else
				{
					Owner->GetWallet()->Give_money(bill, fac->GetOwner()->GetWallet());
				}
			}
		}
	}
	return output;
}

FactoryClass::~FactoryClass()
{
	while(n_workers)
		FireEmployee();
}

float FactoryClass::GetSalary()
{
	return Salary;
}

FactoryClass::FactoryClass()
{
	UnitType = OBJTYPES::FACTORY;
}

bool FactoryClass::IsLookingForEmployee()
{
	if (n_workers < wanted_workers)
		return true;
	return false;
}

void FactoryClass::Update(int input)
{
	if (factoring)
		Prod_process += input * n_workers;
	if (Input_object_type)
	{
		if (Prod_process >= Prod_delay)
		{
			int w = Prod_process / Prod_delay;
			Prod_process -= Prod_delay * w;
			amm_of_out_obj += Prod_in_progress;
			ProducedProd += Prod_in_progress;
			factoring = false;
			if (amm_of_in_obj && amm_of_out_obj < Max_products * n_workers)
			{
				not_enough_resources = false;
				if (amm_of_in_obj >= w)
				{
					amm_of_in_obj -= w;
					Prod_in_progress = w;
				}
				else
				{
					Prod_in_progress = amm_of_in_obj;
					amm_of_in_obj = 0;
				}
				factoring = true;
			}

			if (!amm_of_in_obj)
				not_enough_resources = true;
		}
		else
		{
			if (!factoring)
			{
				if (amm_of_in_obj && amm_of_out_obj < 6 * 30 * 1000 / Prod_delay)
				{
					amm_of_in_obj--;
					factoring = true;
				}
			}
			else if (not_enough_resources && amm_of_in_obj)
				not_enough_resources = false;
		}

		float lowest_price = -1;
		for (int i = 0; i < *amm_Suppliers; i++)
		{
			float supp_price = (*Suppliers)[i]->GetPriceOfProducts();
			int ammount = (*Suppliers)[i]->GetAmmountOfOutputProducts();
			if ((lowest_price == -1 || lowest_price > supp_price) && ammount)
			{
				Price = (float)((*Suppliers)[i]->GetPriceOfProducts() + (Salary * Prod_delay / 1000.0));
				Price *= Margin;
				lowest_price = supp_price;
			}
		}

		if (amm_of_in_obj < 3)
		{
			for (int i = 0; i < *amm_Suppliers; i++)
			{
				float supp_price = (*Suppliers)[i]->GetPriceOfProducts();
				int ammount = (*Suppliers)[i]->GetAmmountOfOutputProducts();

				if (lowest_price != supp_price || !ammount)
					continue;

				buy_products((*Suppliers)[i], 5);

				for (int j = 0; j < n_workers; j++)
				{
					if (Workers[j]->isinfactory())
					{
						Workers[j]->GoToFactory((*Suppliers)[i]);
					}
				}
			}
		}
	}
	else
	{
		Price = (Salary * Prod_delay / 1000.0) * Margin;
		if (Prod_process >= Prod_delay)
		{
			int w = Prod_process / Prod_delay;
			Prod_process -= Prod_delay * w;
			amm_of_out_obj += w;
			ProducedProd += w;
		}
		if (amm_of_out_obj >= Max_products * n_workers)
			factoring = false;
		else
			factoring = true;
	}
}

void FactoryClass::EndOfMonth(float smallest_sallary)
{
	OldSoldProd = SoldProd;
	SoldProd = 0;
	OldProducedProd = ProducedProd;
	ProducedProd = 0;
	if (wanted_workers > n_workers && (ProducedProd - SoldProd < 0 || amm_of_out_obj <= 5)) //kiedy podnosimy zarobki
		if (smallest_sallary > Salary)
			Salary = smallest_sallary;
		else
			Salary *= 1.1;

	if (amm_of_out_obj && !OldSoldProd && !OldProducedProd) //sytuacja kiedy od miesiaca nic nie sprzedano i nie wyprodukowano, lecz jest pelny magazyn.
															//Zwolnienie pracownika i zmniejszenie marzy i placy.
	{
		if (Margin - 0.01 >= 1.1)
		{
			Margin -= 0.01;
		}
		if (Salary > 1.1)
			Salary /= 1.1;
		if (wanted_workers)
		{
			wanted_workers--;
			if (wanted_workers < n_workers)
				FireEmployee();
		}
	}

	if ((amm_of_out_obj >= n_workers * 3 * 30 * 1000 / Prod_delay && OldSoldProd < OldProducedProd)) //sytuacja kiedy produkcja jest wieksza nizsprzedaz.
																																		//obnizenie placy pracowikow i zwolnienie pracownika
	{
		/*if (wanted_workers)
		{
			wanted_workers--;
			if (wanted_workers < n_workers)
				FireEmployee();
		}*/
		if (Margin > 1.2)
			Margin -= 0.01;

		if (Salary > 1.1)
			Salary /= 1.1;
	}
		
	if ((Max_products * n_workers <= amm_of_out_obj && n_workers) || (!amm_of_in_obj && Input_object_type)) //magazyn pelny lub brak produktow. Wstrzymywanie produkcji
	{
		wanted_workers = 0;
		while (wanted_workers < n_workers)
			FireEmployee();
		return;
	}

	if (amm_of_out_obj <= 2 * 30 * 1000 / Prod_delay && (OldProducedProd || !n_workers) && OldProducedProd <= OldSoldProd) //kiedy towaru jest malo podnosimy marze
	{
		Margin += 0.01;
		if ((Margin > 1.2 || !n_workers) && !not_enough_resources && !IsLookingForEmployee())
		{
			if (max_workers < wanted_workers)
				wanted_workers++;
		}
	}

	if (not_enough_resources || (amm_of_out_obj > 0.9 * Max_products * n_workers && n_workers))
	{
		if (Salary > 1.1 && !not_enough_resources)
			Salary /= 1.1;
		if (n_workers > 0)
		{
			wanted_workers--;
			if (wanted_workers < n_workers)
				FireEmployee();
		}
	}

	if (!wanted_workers && amm_of_out_obj < Max_products)
	{
		if (max_workers < wanted_workers)
			wanted_workers++;
	}
}

UINT FactoryClass::GetOutputProductsinfo()
{
	return Output_object_type;
}

UINT FactoryClass::GetInputProductsinfo()
{
	return Input_object_type;
}

UINT FactoryClass::GetFactoryType()
{
	return FactoryType;
}

int FactoryClass::GetAmmountOfOutputProducts()
{
	return amm_of_out_obj;
}

int FactoryClass::GetAmmountOfMoney()
{
	return Owner->GetWallet()->GetMoney();
}

int FactoryClass::GetAmmountOfInputProducts()
{
	return amm_of_in_obj;
}

float FactoryClass::GetPriceOfProducts()
{
	return Price;
}

float FactoryClass::GetMargin()
{
	return Margin;
}

int FactoryClass::GetAmmountOfWorkers()
{
	return n_workers;
}

int FactoryClass::GetAmmountOfWantedWorkers()
{
	return wanted_workers;
}

EmployeeClass * FactoryClass::GetOwner()
{
	return Owner;
}

void FactoryClass::Payment(EmployeeClass* emp, int hours)
{
	float bill = Salary * hours / 1000;
	float Owner_money = Owner->GetWallet()->GetMoney();
	if (Owner_money < bill)
	{
		Owner->add_debt(emp, bill - Owner_money);
		Owner->GetWallet()->Give_money(Owner_money, emp->GetWallet());
	}
	else
	{
		Owner->GetWallet()->Give_money(bill, emp->GetWallet());
	}
}

void FactoryClass::AddEmployee(EmployeeClass * input)
{
	EmployeeClass** buffor = new EmployeeClass*[n_workers + 1];
	for (int i = 0; i < n_workers; i++)
	{
		buffor[i] = Workers[i];
	}
	buffor[n_workers] = input;
	if (n_workers)
		delete[] Workers;
	Workers = buffor;
	n_workers++;
}

void FactoryClass::FireEmployee(EmployeeClass * input)
{
	EmployeeClass** buffor = 0;
	int j = 0;
	if (n_workers - 1)
		buffor = new EmployeeClass*[n_workers - 1];
	for (int i = 0; i < n_workers; i++)
	{
		if (i == 0 && input == 0)
		{
			Workers[0]->Fire();
			continue;
		}
		if (input == Workers[i])
		{
			Workers[i]->Fire();
			continue;
		}
		buffor[j] = Workers[i];
		j++;
	}
	delete[] Workers;
	Workers = buffor;
	n_workers--;
}

void FactoryClass::SetSuppler(int * num_of_supplayers, FactoryClass *** suppliers)
{
	amm_Suppliers = num_of_supplayers;
	Suppliers = suppliers;
}

void FactoryClass::SetParameters(float margin, float salary)
{
	Margin = margin;
	Salary = salary;
}

void FactoryClass::SetEnter(Cityclass * city)
{
	int site = -1; //0 - gora, 1 - prawo, 2 - dow, 3 - lewo
	const int n_streets = city->get_n_streets();
	Cityclass::Street* streets = city->get_streets();
	double smallest_range = 4000;
	double range;
	for (int i = 0; i < n_streets; i++)
	{
		if (streets[i].firstcor.x == streets[i].secondcor.x)
		{
			range = sqrt(pow(streets[i].firstcor.x - Position.x, 2));
			if (streets[i].firstcor.y < Position.y && streets[i].secondcor.y > Position.y && smallest_range > range)
			{
				smallest_range = range;
				if (streets[i].firstcor.x > Position.x)
					site = 1;
				else
					site = 3;
			}
		}
		else
		{
			range = sqrt(pow(streets[i].firstcor.y - Position.y, 2));
			if (streets[i].firstcor.x < Position.x && streets[i].secondcor.x > Position.x && smallest_range > range)
			{
				smallest_range = range;
				if (streets[i].firstcor.y > Position.y)
					site = 2;
				else
					site = 0;
			}
		}
	}

	//jeszcze glowne ulice miasta

	range = sqrt(pow(city->GetCenter().x - Position.x, 2));
	if (smallest_range > range)
	{
		smallest_range = range;
		if (city->GetCenter().x > Position.x)
			site = 1;
		else
			site = 3;
	}

	range = sqrt(pow(city->GetCenter().y - Position.y, 2));
	if (smallest_range > range)
	{
		smallest_range = range;
		if (city->GetCenter().y > Position.y)
			site = 2;
		else
			site = 0;
	}

	switch (site)
	{
	case 0:
	{
		Enter.x = Position.x;
		Enter.y = StartCorner.y - 20;
		break;
	}
	case 1:
	{
		Enter.x = StartCorner.x + Size.width + 20;
		Enter.y = Position.y;
		break;
	}
	case 2:
	{
		Enter.x = Position.x;
		Enter.y = StartCorner.y + Size.height + 20;
		break;
	}
	case 3:
	{
		Enter.x = StartCorner.x - 20;
		Enter.y = Position.y;
		break;
	}
	}
}

D2D1_POINT_2F FactoryClass::GetEnter()
{
	return Enter;
}

void FactoryClass::GiveProducts(int i)
{
	amm_of_in_obj += i;
}

int FactoryClass::SellProducts(int ammount)
{
	int bill = ammount * Price;
	amm_of_out_obj -= ammount;
	SoldProd += ammount;
	return bill;
}

D2D1_POINT_2L FactoryClass::GetIndex()
{
	return Index;
}

void FactoryClass::SetIndex(D2D1_POINT_2L input)
{
	Index = input;
}
