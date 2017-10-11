#include "Wallet.h"
#include <iostream>

Wallet::~Wallet()
{
	int s = 0;
	if (_money != 0)
		1 / s;
	if (_money < 0)
		_sleep(1);
}

Wallet::Wallet()
{
	_money = 0;
}

void Wallet::Give_money(double ammount, Wallet * target)
{
	target->_money += ammount;
	_money -= ammount;
	if (_money < 0)
		_sleep(1);
}

void Wallet::Put_in_money(double ammount)
{
	_money += ammount;
	if (_money < 0)
		_sleep(1);
}

double Wallet::GetMoney()
{
	if (_money < 0)
		_sleep(1);
	return _money;
}
