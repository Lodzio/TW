#include "Employee.h"
#include "Factory.h"
#include "Family.h"
#include "city.h"
#include "Pathfinding.h"

void EmployeeClass::Buy()
{
	const int upl_hun = 80;
	const float lowl_mon = 3;
	const float upl_mon = 10;
	float lowest_bread_price = -1;
	float lowest_cutlery_price = -1;
	if (isinfamily)
	{
		family->GetWallet()->Give_money(family->GetWallet()->GetMoney() / family->Get_n_members(), &wallet);
	}

	for (int i = 0; i < *amm_of_shops; i++)
	{
		int amm_of_prod = (*List_of_shops)[i]->GetAmmountOfOutputProducts();
		float supp_prize = (*List_of_shops)[i]->GetPriceOfProducts();
		if ((supp_prize < lowest_bread_price || lowest_bread_price == -1) && amm_of_prod && (*List_of_shops)[i]->GetOutputProductsinfo() == FactoryClass::Bread)
			lowest_bread_price = supp_prize;
		if ((supp_prize < lowest_cutlery_price || lowest_cutlery_price == -1) && amm_of_prod && (*List_of_shops)[i]->GetOutputProductsinfo() == FactoryClass::cutlery)
			lowest_cutlery_price = supp_prize;
	}

	for (int i = 0; i < *amm_of_shops; i++)
	{
		int amm_of_prod = (*List_of_shops)[i]->GetAmmountOfOutputProducts();
		float supp_prize = (*List_of_shops)[i]->GetPriceOfProducts();
		if (supp_prize != lowest_bread_price || !amm_of_prod || (*List_of_shops)[i]->GetOutputProductsinfo() != FactoryClass::Bread)
			continue;
		int hung_supply = 100 - (((wallet.GetMoney() / (*List_of_shops)[i]->GetPriceOfProducts()) - lowl_mon) * (upl_hun / (upl_mon - lowl_mon)));

		if (isinfamily)
		{
			float new_status = (wallet.GetMoney() / lowest_bread_price) - lowl_mon;
			if (new_status < family->life_status)
			{
					family->life_status = new_status;
			}
			else
				family->life_status += 0.01 * (new_status - family->life_status);
		}

		if (hung_supply < 20)
			hung_supply = 20;
		else if (hung_supply > 100)
			hung_supply = 100;
			
		if (hung_supply + Hungry <= 100)
		{
			if ((*List_of_shops)[i]->GetPriceOfProducts() < wallet.GetMoney())
			{
				float bill = (*List_of_shops)[i]->SellProducts(1);
				wallet.Give_money(bill, (*List_of_shops)[i]->GetOwner()->GetWallet());
				Hungry += hung_supply;
				break;
			}
		}
	}
	
	for (int i = 0; i < *amm_of_shops; i++)
	{
		int amm_of_prod = (*List_of_shops)[i]->GetAmmountOfOutputProducts();
		float supp_prize = (*List_of_shops)[i]->GetPriceOfProducts();
		if (supp_prize != lowest_cutlery_price || !amm_of_prod || (*List_of_shops)[i]->GetOutputProductsinfo() != FactoryClass::cutlery)
			continue;
		if (Cutlerys <= 0)
		{
			if ((*List_of_shops)[i]->GetPriceOfProducts() < wallet.GetMoney())
			{
				float bill = (*List_of_shops)[i]->SellProducts(1);
				wallet.Give_money(bill, (*List_of_shops)[i]->GetOwner()->GetWallet());
				Cutlerys = 10;
				break;
			}
		}
	}

	if (isinfamily)
	{
		wallet.Give_money(wallet.GetMoney(), family->GetWallet());
	}

}

void EmployeeClass::add_borrower(EmployeeClass * input)
{
	struct_borrower* buffor = new struct_borrower[n_borrower + 1];
	for (int i = 0; i < n_borrower; i++)
	{
		buffor[i] = borrower[i];
	}
	buffor[n_borrower].owner = input;
	buffor[n_borrower].ammount = 0;
	if (n_borrower)
		delete[] borrower;
	borrower = buffor;
	n_borrower++;
}

void EmployeeClass::CalcPosition(int timeDiff)
{
	if (!is_in_factory)
	{
		if (Mode.UnitMode == InClassModeClass::MODE_IDLE)
		{
			if (employee_mode == IDLE)
			{
				is_in_factory = true;
			}
			else if (employee_mode == GO_FOR_PORODUCTS)
			{
				ComeBackToFactory();
				employee_mode = IDLE;
			}
		}
		UnitClass::CalcPosition(timeDiff);
	}
}

bool EmployeeClass::IsInDebt()
{
	if (n_borrower)
		return true;
	else
		return false;
}

EmployeeClass::~EmployeeClass()
{
	return;
}

void EmployeeClass::remove_borrower(EmployeeClass * input)
{
	if (!n_borrower)
		return;
	struct_borrower* buffor = 0;
	if (n_borrower > 1)
		buffor = new struct_borrower[n_borrower - 1];
	int j = 0;
	for (int i = 0; i < n_borrower; i++)
	{
		if (input == borrower[i].owner)
			continue;
		if (j == n_borrower - 1)
		{
			delete[] buffor;
			return;
		}
		buffor[j] = borrower[i];
		j++;
	}
	if (n_borrower)
		delete[] borrower;
	borrower = buffor;
	n_borrower--;
}

void EmployeeClass::Update_borrowers()
{
	for (int i = 0; i < n_borrower; i++)
	{
		if (isinfamily)
		{
			if (family->GetWallet()->GetMoney() >= borrower[i].ammount)
			{
				family->GetWallet()->Give_money(borrower[i].ammount, borrower[i].owner->GetWallet());
				borrower[i].ammount -= borrower[i].ammount;
				if (borrower[i].ammount == 0)
				{
					remove_borrower(borrower[i].owner);
					i--;
				}
			}
			else if (family->GetWallet()->GetMoney() < borrower[i].ammount)
			{

				borrower[i].ammount -= family->GetWallet()->GetMoney();
				family->GetWallet()->Give_money(family->GetWallet()->GetMoney(), borrower[i].owner->GetWallet());
			}

			if (family->GetWallet()->GetMoney() == 0)
				break;
		}
		else
		{
			if (wallet.GetMoney() >= borrower[i].ammount)
			{
				wallet.Give_money(borrower[i].ammount, borrower[i].owner->GetWallet());
				borrower[i].ammount -= borrower[i].ammount;
				if (borrower[i].ammount == 0)
				{
					remove_borrower(borrower[i].owner);
					i--;
				}
			}
			else if (wallet.GetMoney() < borrower[i].ammount)
			{
				borrower[i].ammount -= wallet.GetMoney();
				wallet.Give_money(wallet.GetMoney(), borrower[i].owner->GetWallet());
			}

			if (wallet.GetMoney() == 0)
				break;
		}
	}
}

void EmployeeClass::add_debt(EmployeeClass * emp, float to_borrow)
{
	bool existing = false;
	for (int i = 0; i < n_borrower; i++)
	{
		if (borrower[i].owner == emp)
		{
			existing = true;
			borrower[i].ammount += to_borrow;
		}
	}

	if (!existing)
	{
		add_borrower(emp);
		borrower[n_borrower - 1].ammount += to_borrow;
	}
}

FamilyClass * EmployeeClass::GetFamily()
{
	return family;
}

bool EmployeeClass::IsAdult()
{
	return adult;
}

void EmployeeClass::GiveAdult()
{
	adult = true;
}

void EmployeeClass::FindFamily(FamilyClass * input)
{
	isinfamily = true;
	family = input;
	wallet.Give_money(wallet.GetMoney(), family->GetWallet());
}

int EmployeeClass::GetHungry()
{
	return Hungry;
}

Wallet* EmployeeClass::GetWallet()
{
	if (isinfamily)
	{
		return family->GetWallet();
	}
	return &wallet;
}

void EmployeeClass::Init_employee()
{
	employee_mode = IDLE;
	Working = false;
	Employed = 0;
	hours_worked = false;
	amm_of_shops = 0;
	List_of_shops = 0;
	Hungry = 100;
	Workless_month = 0;
	isinfamily = false;
	adult = false;
	growing = 0;
	n_borrower = 0;
	is_in_factory = true;
}

void EmployeeClass::SetShops(FactoryClass *** Shops, int * ammofshops)
{
	amm_of_shops = ammofshops;
	List_of_shops = Shops;
}

void EmployeeClass::Update(int input)
{
	Hungry -= 2 * (input / 1000.0);
	if (Cutlerys > 0)
		Cutlerys -= (input / 1000.0);

	if (!adult)
	{
		const int growing_time = 30 * 12;
		growing += input / 1000.0;
		if (growing >= growing_time)
			adult = true;
	}
	else if (Working)
	{
		hours_worked += input;
	}

	Buy();
	Update_borrowers();
}

void EmployeeClass::EndOfMonth()
{
	if (Working)
	{
		Workless_month = 0;
		Payment();
	}
	else
	{
		if (adult)
			Workless_month++;
	}
}

void EmployeeClass::Payment()
{
	Employed->Payment(this, hours_worked);
	hours_worked = 0;
}

void EmployeeClass::LookForJob(FactoryClass ** list, int ammount, float cost_of_life)
{
	float highest_salary = -1;
	for (int i = 0; i < ammount; i++)
	{
		if (!list[i]->IsLookingForEmployee())
			continue;
		if (list[i]->GetSalary() > highest_salary)
			highest_salary = list[i]->GetSalary();
	}

	if (cost_of_life * 1.1 >= highest_salary * 30)
		return;

	for (int i = 0; i < ammount; i++)
	{
		if (!list[i]->IsLookingForEmployee())
			continue;
		if (list[i]->GetSalary() == highest_salary)
		{
			if (Working)
			{
				if (Employed->GetSalary() < highest_salary)
				{
					Employed->FireEmployee(this);
					Employed = list[i];
					Employed->AddEmployee(this);
					Working = true;
					return;
				}
			}
			else
			{
				Employed = list[i];
				Employed->AddEmployee(this);
				Working = true;
				return;
			}
		}
	}
}

void EmployeeClass::Fire()
{
	Employed = NULL;
	Working = false;
}

bool EmployeeClass::IsInFamily()
{
	return isinfamily;
}

void EmployeeClass::Dead()
{
	if (isinfamily)
		family->RemoveMember(this);
	if (Working)
		Employed->FireEmployee(this);
}

void EmployeeClass::RemoveFromFamily()
{
	isinfamily = false;
}

int EmployeeClass::MonthsWithoutJob()
{
	return Workless_month;
}

void EmployeeClass::GoToFactory(FactoryClass * Fac)
{
	if (!Working)
		return;
	D2D1_POINT_2F A, B;
	A = Employed->GetEnter();
	B = Fac->GetEnter();
	Object::SetPosition(A);
	SetTarget(B);
	employee_mode = GO_FOR_PORODUCTS;
	is_in_factory = false;
}

bool EmployeeClass::isinfactory()
{
	return is_in_factory;
}

void EmployeeClass::ComeBackToFactory()
{
	D2D1_POINT_2F B;
	B = Employed->GetEnter();

	SetTarget(B);
}
