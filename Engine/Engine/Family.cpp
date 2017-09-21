#include "Family.h"

FamilyClass::FamilyClass()
{
	n_members = 0;
	income = 0;
	life_status = 0;
}

void FamilyClass::AddMember(EmployeeClass * input)
{
	EmployeeClass** buffor = new EmployeeClass*[n_members + 1];

	for (int i = 0; i < n_members; i++)
	{
		buffor[i] = Members[i];
	}
	buffor[n_members] = input;
	if (n_members)
		delete[] Members;
	Members = buffor;
	n_members++;
	input->FindFamily(this);
}

void FamilyClass::RemoveMember(EmployeeClass * input)
{
	EmployeeClass** buffor = 0;
	int j = 0;
	if (n_members > 1)
		buffor = new EmployeeClass*[n_members - 1];
	for (int i = 0; i < n_members; i++)
	{
		if (Members[i] == input)
			continue;
		buffor[j] = Members[i];
		j++;
	}
	delete[] Members;
	n_members--;
	if (n_members == 1)
	{
		buffor[0]->RemoveFromFamily();
		wallet.Give_money(wallet.GetMoney(), buffor[0]->GetWallet());
		n_members = 0;
		delete[] buffor;
	}
	Members = buffor;

}

EmployeeClass * FamilyClass::GetEmployeebyIndex(int ind)
{
	return Members[ind];
}

int FamilyClass::Get_n_adults_members()
{
	int out = 0;
	for (int i = 0; i < n_members; i++)
		if (Members[i]->IsAdult())
			out++;
	return out;
}

int FamilyClass::Get_n_members()
{
	return n_members;
}

void FamilyClass::End_of_month()
{
	if (life_status < 1.0)
	{
		for (int i = 0; i < n_members; i++)
		{
			if (!Members[i]->IsAdult())
			{
				Members[i]->RemoveFromFamily();
				RemoveMember(Members[i]);
				i--;
			}
		}
	}
}

Wallet* FamilyClass::GetWallet()
{
	return &wallet;
}
