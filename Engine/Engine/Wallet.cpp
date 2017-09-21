#include "Wallet.h"

Wallet::~Wallet()
{
	int s = 0;
	if (_money != 0)
		1 / s;
}

void Wallet::Give_money(double ammount, Wallet * target)
{
	target->_money += ammount;
	_money -= ammount;
}

void Wallet::Put_in_money(double ammount)
{
	_money += ammount;
}

double Wallet::GetMoney()
{
	return _money;
}
