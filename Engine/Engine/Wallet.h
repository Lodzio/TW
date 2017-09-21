#pragma once

class Wallet
{
private:
	double _money;
public:
	~Wallet();
	Wallet() : _money(0){};
	void Give_money(double ammount, Wallet* target);
	void Put_in_money(double ammount);
	double GetMoney();
};